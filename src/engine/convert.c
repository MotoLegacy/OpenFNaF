// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Append integer to chararray
// FIXME - ugly lol
char* append_int_to_char(char* str, int i) {
    char* tempc = malloc(128);
    char itoc[32];
    sprintf(itoc, "%d", i);
    strcpy(tempc, str);
    strcat(tempc, itoc);
    return tempc;
}

char* append_char_to_char(char* dest, char* src) {
    return strcat(dest, src);
}