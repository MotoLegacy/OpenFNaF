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
// pc_gameloader.c - the game loader/chooser for UNIX/PC
//

#include "../defs.h"

void Game_InitializeLoader() {
    gamedata_t  supported_games[8];
    boolean     game_selected = FALSE;
    int         iterer = 0;
    int         selection;

    // Grab a list of only the games supporting PC
    for (int i = 0; i < 8; i++) {
        if (INI_Games[i].supports_pc == TRUE) {
            supported_games[iterer].occupied = TRUE;
            supported_games[iterer].window_width = INI_Games[i].window_width;
            supported_games[iterer].window_height = INI_Games[i].window_height;
            supported_games[iterer].name = INI_Games[i].name;
            supported_games[iterer].game_path = INI_Games[i].game_path;
            iterer++;
        }
    }

    // Print out the game selection
    while(game_selected == FALSE) {
        printf("Select a Supported Game from the list\n");
        for (int i = 0; i < iterer; i++) {
            printf("%d. %s\n", i, supported_games[i].name);
        }
        scanf("%d", &selection);

        if (selection < 0 || selection >= iterer) {
            printf("Invalid Selection.\n");
            printf("======================\n");
        } else {
            game_selected = TRUE;
        }
    }

    // Initialize the raylib Window
    if (!OPT_NORENDER)
        Window_Initialize(supported_games[selection].window_width, supported_games[selection].window_height, supported_games[selection].name);


    // Initialize the Game Handler
    Game_Initialize(supported_games[selection]);
}