// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

///
/// save.c - Saving, loading, etc. Keep compatibility with MMF saves!
///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "save.h"
#include "types.h"

// Saves/freddy
char* Save_GetDirectory() {
    char* SaveDirectory;

    SaveDirectory = malloc(sizeof(char)*32);

    strcpy(SaveDirectory, "Saves/");
    strcat(SaveDirectory, SAVE_TITLE);

    return SaveDirectory;
}

// Currently we create a new save, delete the old one,
// and then rename the new to old.. FIXME: find faster
// method
void Save_SetValue(char* target, u16_t value) {
    int bufferlength = 32;
    char buffer[bufferlength];

    bool foundValue = FALSE;

    FILE *save;
    FILE *newsave;

    char* line[32];
    char* templn[9];

    // Combine target and value to form line to write
    sprintf(templn, "%d", value);

    strcpy(line, target);
    strcat(line, "=");
    strcat(line, templn);
    strcat(line, "\n");

    save = fopen(Save_GetDirectory(), "r");
    newsave = fopen("Saves/temp.tmp", "w+"); 

    

    // See if the value is already in the file
    while(fgets(buffer, bufferlength, save)) {
        // Found target line
        if (strncmp(buffer, target, strlen(target)) == 0) {
            // we found it!
            foundValue = TRUE;
            fputs(line, newsave);
        } else { // Else just write the old line
            fputs(buffer, newsave); 
        }
    }


    // It didn't exist already, so add it to the bottom
    if (foundValue == FALSE)
        fputs(line, newsave);

    // Close Files
    fclose(save);
    fclose(newsave);

    // Remove save and replace with temp
    remove(Save_GetDirectory());
    rename("Saves/temp.tmp", Save_GetDirectory());
}

u16_t Save_GetValue(char* target) {
    int bufferlength = 32;
    char buffer[bufferlength];
    
    int value = 0;
    // Open Save file
    FILE *save;
    save = fopen(Save_GetDirectory(), "r");

    // Check save file for target
    while(fgets(buffer, bufferlength, save)) {
        // If we found our target line
        if (strncmp(buffer, target, strlen(target)) == 0) {
            sscanf(buffer+(strlen(target)+1), "%d", &value);
        }
    }

    fclose(save);

    // Return value
    return value;
}

bool Save_Exist() {
    // First check - Saves directory
    struct stat st = {0};

    if (stat("Saves", &st) == -1) return FALSE;

    // Second check - actual file
    if (stat(Save_GetDirectory(), &st) == -1) return FALSE;

    // They box exist, so return TRUE
    return TRUE;
}

void Save_Create(void) {
    // Establish chars for save names
    char* SaveTitle[16];

    strcpy(SaveTitle, "[");
    strcat(SaveTitle, SAVE_TITLE);
    strcat(SaveTitle, "]\n");

    // Create Saves directory if it does not already exist
    struct stat st = {0};

    if (stat("Saves", &st) == -1) {
        mkdir("Saves", 0700);
    }

    // Now create the actual save file
    FILE *save;
    
    save = fopen(Save_GetDirectory(), "w+");
    fputs(SaveTitle, save);
    fclose(save);
}
