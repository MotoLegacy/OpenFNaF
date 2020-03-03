///
/// save.c - Saving, loading, etc. Keep compatibility with MMF saves!
///

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "save.h"

// Saves/freddy
char* Save_GetDirectory() {
    char* SaveDirectory[32];

    strcpy(SaveDirectory, "Saves/");
    strcat(SaveDirectory, SAVE_TITLE);

    return SaveDirectory;
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