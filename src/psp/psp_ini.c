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

gamedata_t* INI_Games[8];

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
            pspDebugScreenPrintf("Found Game: %s\n", buffer);
        }

        // Continue parsing
        buffer = strtok(NULL, "\n");
    }

    /*while(sceIoRead(INIFile, buffer, bufferlength)) {
        if (strncmp(buffer, "\0name=", strlen("\0name=")) == 0) {
            pspDebugScreenPrintf("gmaeline: %s\n", buffer);
            //memmove(buffer, buffer+5, strlen(buffer));
            //pspDebugScreenPrintf("Found Game: %s\n", buffer);
        }
    }*/

    // Close the File
    sceIoClose(INIFile);
}

//
// INI_Initialize()
// Find Game folders, then call FetGameInformation()
//
void INI_Initialize() {
    // Grab the games/ directory with sceIoDopen
    int directory = sceIoDopen("games/");

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