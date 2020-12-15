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
// ini.c - Game INI Parsing
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

    FILE* INIFile;
    INIFile = fopen(INIPath, "r");

    // No game.ini file, ignore this path
    if (INIFile == NULL) {
        return;
    }

    // Buffer setup
    char buffer[64];
    int bufferlength = sizeof(buffer);

    // Begin parsing
    while(fgets(buffer, bufferlength, INIFile)) {
        // Name
        if (strncmp(buffer, "name=", strlen("name=")) == 0) {
            memmove(buffer, buffer+5, strlen(buffer));
            printf("Found Game: %s", buffer);
        }
    }
    fclose(INIFile);
}

//
// INI_Initialize()
// Find Game folders, then call FetGameInformation()
//
void INI_Initialize() {
    // Grab the games/ directory with dirent
    struct dirent* cwdirent;
    DIR* cwd = opendir("games/");

    // Loop through all directories found by opendir()
    while ((cwdirent = readdir(cwd)) != NULL) {
        // If we found a directory, and it is not backwards from our root
        if (cwdirent->d_type == DT_DIR && strcmp(".", cwdirent->d_name) != 0 && strcmp("..", cwdirent->d_name) != 0)
            // Try to fetch game information
            INI_FetchGameInformation(cwdirent->d_name);
    }
}