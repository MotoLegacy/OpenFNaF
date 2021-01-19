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
// game.c - holds the main game loop
//

#include "defs.h"

#ifndef PSP
#define FRAMES_PER_SECOND       60
#endif

// PSP: Limit to 30 FPS
#ifdef PSP
#define FRAMES_PER_SECOND       30
#endif

boolean     Game_Running = FALSE;
gamedata_t  Loaded_Game;

//
// Game_Intialize(game)
// Set up our back-end and run our game loop
//
void Game_Initialize(gamedata_t game) {
    char*   main_script;
    u8_t    hour;

    hour = 0;
    Game_Running = TRUE;
    Loaded_Game = game;

    // Initialize our framedelays
    Time_FrameDelay(1, 0); // Frames per Second
    Time_FrameDelay(1, 1); // Actual Game Timer

#ifndef DESKTOP
    // Initialize our Input Looper
    Input_Initialize();
#endif

    // Seed the Random Number Generator
    Math_SetSeed(clock());

    // Initialize the Lua Virtual Machine
    Lua_InitializeVM();

    // Compile the game's main.lua file
    main_script = malloc(sizeof(char)*32);
    strcpy(main_script, game.game_path);
    strcat(main_script, "main.lua");
    Lua_CompileFile(main_script);

    // Run the main function
    lua_getglobal(VMState, "G_Main");
    lua_pcall(VMState, 0, 0, 0);

    Music music = LoadMusicStream("wow.mp3");
    PlayMusicStream(music);

    while(Game_Running) {
#ifdef DESKTOP
        Sound_Update();
#endif
        // Non-graphics stuff
        if (Time_FrameReady(1)) {

            // Run Game Loop function
            lua_getglobal(VMState, "G_GameLoop");
            lua_pcall(VMState, 0, 0, 0);

#ifndef DESKTOP
            // Check current Inputs
            Input_Loop();
#endif

            // Next Game Time iteration
            Time_FrameDelay(10, 1);
        }

        // New Frame
        if (Time_FrameReady(0)) {
            Graphics_StartDrawing();

#ifdef DESKTOP
            Window_Clear();
#endif

            // Run Draw Loop function
            lua_getglobal(VMState, "G_DrawLoop");
            lua_pcall(VMState, 0, 0, 0);

            // Set time until next frame is ready
            Time_FrameDelay(1000/FRAMES_PER_SECOND, 0);

            Graphics_StopDrawing();
        }

#ifdef DESKTOP
        if (WindowShouldClose())
            Window_Close();
#endif
    }

    Sound_Clean();

#ifdef PSP
    oslEndGfx();
	oslQuit();
#endif
}