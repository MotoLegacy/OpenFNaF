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
// main.c - initialization and primary entry point for the engine
//

#include "defs.h"

// PSP: Include Module info
#ifdef PSP
PSP_MODULE_INFO("OpenFNaF", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);
#endif

int main(int argc, char* argv[]) {
    #ifdef PSP
    pspDebugScreenInit();
    pspDebugScreenSetXY(0, 0);
    #endif

    // Discover Game INIs
    INI_Initialize();

    /*
    // Parse command line arguments
    Options_ParseCMD(argc, argv);

    // Initialize Game window
    if(!OPT_NORENDER)
        Window_Initialize(I_GAME_WIDTH, I_GAME_HEIGHT, I_GAME_TITLE);

    // Init Game handler
    Game_Initialize();
    */

    #ifdef PSP
    sceKernelDelayThread(15*1000000);
    sceKernelExitGame();
    #endif
    return 0;
}
