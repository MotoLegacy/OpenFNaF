// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "types.h"
#ifdef DESKTOP
#include "pc/window.h"
#endif
#include "game.h"
#include "version.h"
#include "options.h"

int main(int argc, char* argv[]) {
    // Parse command line arguments
    Options_ParseCMD(argc, argv);

    // Initialize Game window
    if(!OPT_NORENDER)
        Window_Initialize(I_GAME_WIDTH, I_GAME_HEIGHT, I_GAME_TITLE);

    // Init Game handler
    Game_Initialize();

    return 0;
}
