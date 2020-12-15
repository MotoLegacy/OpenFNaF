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
// psp_ini.c - PSP Game INI Parsing
//

#include "../defs.h"

gamedata_t INI_Games[8];
int         INI_GameCount;

//
// INI_FetchGameInformation(directory)
// Attempts to open game.ini and read information
// from it.
//
void INI_FetchGameInformation(char* directory) {
    // Fully establish our game directory
    char* INIPath;
    INIPath = malloc(sizeof(char)*32);
    strcpy(INIPath, "games/");
    strcat(INIPath, directory);
    strcat(INIPath, "/game.ini");

    // Attempt to open the File
    int INIFile;
    INIFile = sceIoOpen(INIPath, PSP_O_RDONLY, 0777);

    // No game.ini file, ignore this path
    if (INIFile < 0) {
        return;
    }

    // It exists, so let's occupy a game slot
    INI_Games[INI_GameCount].occupied = TRUE;

    // Buffer setup
    char* buffer = malloc(sizeof(INIFile));

    // Read the file
    sceIoRead(INIFile, buffer, 255);

    // Begin parsing the file
    strtok(buffer, "\n");
    while(buffer != NULL) {
        // Name
        if (strncmp(buffer, "name=", strlen("name=")) == 0) {
            memmove(buffer, buffer+5, strlen(buffer));
            strcpy(INI_Games[INI_GameCount].name, buffer);
        }
        // Supports PSP
        if (strncmp(buffer, "supports_psp=", strlen("supports_psp=")) == 0) {
            int value = 0;
            sscanf(buffer + strlen("supports_psp="), "%d", &value);
            INI_Games[INI_GameCount].supports_psp = value;
        }
        // Supports PC
        if (strncmp(buffer, "supports_pc=", strlen("supports_pc=")) == 0) {
            int value = 0;
            sscanf(buffer + strlen("supports_pc="), "%d", &value);
            INI_Games[INI_GameCount].supports_pc = value;
        }
        // Window Width
        if (strncmp(buffer, "window_width=", strlen("window_width=")) == 0) {
            int value = 0;
            sscanf(buffer + strlen("window_width="), "%d", &value);
            INI_Games[INI_GameCount].window_width = value;
        }
        // Window Width
        if (strncmp(buffer, "window_height=", strlen("window_height=")) == 0) {
            int value = 0;
            sscanf(buffer + strlen("window_height="), "%d", &value);
            INI_Games[INI_GameCount].window_height = value;
        }
        // Continue parsing
        buffer = strtok(NULL, "\n");
    }

    // Close the File
    sceIoClose(INIFile);

    // Increment the gamecount
    INI_GameCount++;
}

//
// INI_Initialize()
// Find Game folders, then call FetGameInformation()
//
void INI_Initialize() {
    // Grab the games/ directory with sceIoDopen
    int directory = sceIoDopen("games/");

    // Initialize gamecount
    INI_GameCount = 0;

    for (int i = 0; i < 8; i++) {
        INI_Games[i].occupied = FALSE;
        INI_Games[i].supports_psp = FALSE;
        INI_Games[i].supports_pc = FALSE;
        INI_Games[i].name = malloc(sizeof(char)*64);
    }

    // If the directory exists
    if (directory > 0) {
        SceIoDirent dir;

        // Look through everything in the directory
        while (sceIoDread(directory, &dir) > 0) {
            // If it's another directory
            if (dir.d_stat.st_attr & FIO_SO_IFDIR) {
                if (dir.d_name[0] != '.') {
                    // Try to fetch game information
                    INI_FetchGameInformation(dir.d_name);
                }
            }
        }
        sceIoDclose(directory);
    }
}