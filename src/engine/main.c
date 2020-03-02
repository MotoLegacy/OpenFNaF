#include <string.h>

#include "types.h"
#include "window.h"
#include "game.h"
#include "version.h"

bool OPT_NORENDER = FALSE;

int main(int argc, char* argv[]) {
    // Parse command line arguments
    for(int i = 0; i <= (argc - 1); i++) {
        if (strcmp("-no-render", argv[i]) == 0) {
            OPT_NORENDER = TRUE;
        }
    }

    // Initialize Game window
    if(!OPT_NORENDER)
        Window_Initialize(I_GAME_WIDTH, I_GAME_HEIGHT, I_GAME_TITLE, argc, argv);

    // Init Game handler
    Game_Initialize();

    return 0;
}
