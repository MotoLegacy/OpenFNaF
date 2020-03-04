// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include <string.h>

#include "types.h"

bool OPT_NORENDER = FALSE;

void Options_ParseCMD(int argc, char* argv[]) {
    for(int i = 0; i <= (argc - 1); i++) {
        // No renderer/window/etc.
        if (strcmp("-no-render", argv[i]) == 0) {
            OPT_NORENDER = TRUE;
        }
    }
}