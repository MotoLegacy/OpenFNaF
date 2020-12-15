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
// psp_gameloader.c - the game loader/chooser for PSP
//

#include "../defs.h"


void Game_InitializeLoader() {
    gamedata_t  supported_games[8];
    bool        Game_Selected   = FALSE;
    bool        redraw_screen   = TRUE;
    bool        down_released;
    bool        up_released;
    int         selected        = 0;
    int         iterer          = 0;

    // Grab a list of only the games supporting PSP
    for (int i = 0; i < 8; i++) {
        if (INI_Games[i].supports_psp == TRUE) {
            supported_games[iterer].occupied = TRUE;
            supported_games[iterer].name = INI_Games[i].name;
            iterer++;
        }
    }

    // Initialize the controller
    SceCtrlData pad;
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    // Main selection loop
    while(Game_Selected == FALSE) {
        if (redraw_screen == TRUE) {
            // Clear screen
            pspDebugScreenClear();

            // Header
            printf("Select a Supported Game from the list (Dpad to move, X to select)\n");

            // Draw the supported game list (with a '> ' if it's what's selected currently)
            for(int i = 0; i < iterer; i++) {
                if (supported_games[i].occupied == TRUE) {
                    if (selected == i)
                        printf("> %s\n", supported_games[i].name);
                    else
                        printf("%s\n", supported_games[i].name);
                }
            }
            redraw_screen = FALSE;
        }

        // Read input
        sceCtrlReadBufferPositive(&pad, 1);
        if ((pad.Buttons & PSP_CTRL_DOWN) && down_released) {
            selected++;
            if (selected >= iterer)
                selected = 0;
            redraw_screen = TRUE;
            down_released = FALSE;
		} else {
            down_released = TRUE;
        }
        if ((pad.Buttons & PSP_CTRL_UP) && up_released) {
            selected--;
            if (selected < 0)
                selected = iterer - 1;
            redraw_screen = TRUE;
            up_released = FALSE;
        } else {
            up_released = TRUE;
        }
        if (pad.Buttons & PSP_CTRL_CROSS) {
            Game_Selected = TRUE;
        }
    }
}