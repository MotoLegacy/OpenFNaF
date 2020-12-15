//
// MIT License
//
// Copyright (c) 2020 MotoLegacy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

//
// lua_virtual_machine.c - General Functions for the LUA Virtual Machine
//

#include "defs.h"
#include "lua_includes.h"
#include <assert.h>

lua_State* VMState;

//
// Lua_CompileFile(filename)
// Compiles the entire Lua Script
//
// TODO:
// * do we even need 'status'..???
void Lua_CompileFile(char* filename)
{
    luaL_loadfile(VMState, filename);
    lua_pcall(VMState, 0, LUA_MULTRET, 0);
}

//
// Lua_DivideAndAdd
// Divide parm0 by parm1, before adding parm2
// -----
// Called by the Lua VM
//
/*static int Lua_DivideAndAdd(lua_State* State)
{
    // Assert Params
    assert(lua_isnumber(State, 1));
    assert(lua_isnumber(State, 2));
    assert(lua_isnumber(State, 3));
    
    // Actually define them
    lua_Number parm0, parm1, parm2;
    parm0 = lua_tonumber(State, 1);
    parm1 = lua_tonumber(State, 2);
    parm2 = lua_tonumber(State, 3);

    // Perform Operation
    lua_Number result = (parm0 / parm1) + parm2;

    // Push back and return result count
    lua_pushnumber(State, result);
    return 1;
}*/

//
// Lua_LoadScript(string)
// Compiles a LUA script
// TODO:
// * prohibit going back a directory, contain everything
// -----
// Called by the Lua VM
//
static int  Lua_LoadScript(lua_State* State)
{
    // assert param
    assert(lua_isstring(State, 1));

    // Define it
    char* script;
    script = malloc(sizeof(char)*32);
    strcpy(script, Loaded_Game.game_path);
    strcat(script, lua_tostring(State, 1));
    Lua_CompileFile(script);

    return 0;
}

//
// Lua_PSPDebugPrintf(string)
// Calls pspDebugPrintf()
// -----
// Called by the Lua VM
//
static int Lua_PSPDebugPrintf(lua_State* State)
{
    // Assert Param
    assert(lua_isstring(State, 1));

    // Define it
    const char* parm0;
    parm0 = lua_tostring(State, 1);

    #ifdef PSP
    pspDebugScreenPrintf("%s", parm0);
    #endif

    return 1;
}

//
// Lua_GetPlatform
// Returns the platform as a string
// * "PSP"
// * "PC"
// -----
// Called by the Lua VM
//
static int Lua_GetPlatform(lua_State* State)
{
#ifdef PSP
    lua_pushstring(State, "PSP");
    return 1;
#endif
#ifdef DESKTOP
    lua_pushstring(State, "PC");
    return 1;
#endif
}

//
// Lua_GetInput(button, mode)
// Check if the key requested by
// LUA VM is pressed
// -----
// Called by the Lua VM
//
static int Lua_GetInput(lua_State* State)
{
    // Assert params
    // FIXME: asserts.. not working?
    //assert(lua_isnumber(State, 1));
    //assert(lua_isnumber(State, 2));

    int button = (int)lua_tonumber(State, 1);
    int inputmode = (int)lua_tonumber(State, 2);
    int result = 0;

    // Check which input mode we're looking for, send the response
    switch (inputmode) {
        case INPUT_BUTTON_DOWN:
#ifndef DESKTOP
            result = Input_ButtonPressedDown(button);
#endif
            break;
        case INPUT_KEYBOARD_DOWN:
#ifdef DESKTOP
            result = Input_KeyboardDown(button);
#endif
            break;
        default:
            // Just push 0 if this is undefined behavior
            result = 0;
            break;
    }

    // Push it real good
    lua_pushnumber(State, (lua_Number)result);

    // One result
    return 1;
}

//
// Lua_LinkFunctions
// Link Lua and C Functions
//
void Lua_LinkFunctions()
{
    // PSPDebugPrintf()
    lua_pushcclosure(VMState, Lua_PSPDebugPrintf, 0);
    lua_setglobal(VMState, "PSPDebugPrintf");
    // OF_GetPlatform()
    lua_pushcclosure(VMState, Lua_GetPlatform, 0);
    lua_setglobal(VMState, "OF_GetPlatform");
    // OF_LoadScript()
    lua_pushcclosure(VMState, Lua_LoadScript, 0);
    lua_setglobal(VMState, "OF_LoadScript");
    // OF_GetInput()
    lua_pushcclosure(VMState, Lua_GetInput, 0);
    lua_setglobal(VMState, "OF_GetInput");
}

//
// Lua_VMPanic
// Called whenever the VM Panics
// TODO:
// * Print some valuable information
// * Kill rest of engine to avoid BAD THINGS
//
void Lua_VMPanic()
{
    printf("the vm panicked!!!!! o no\n");
}

//
// Lua_InitiaulizeVM
// Basic Initialization of the Virtual Machine
//
int Lua_InitializeVM()
{
    // Open the VM
    VMState = luaL_newstate();

    if (VMState) {
        // Load Standard Libraries
        luaL_openlibs(VMState);

        // Call to link Lua and C Functions
        Lua_LinkFunctions();

        // Panic Link
        lua_atpanic(VMState, (lua_CFunction)Lua_VMPanic);

        // return good number
        return 1;
    }

    // hey jordan? why didn't the vm initialize?
    return 0;
}