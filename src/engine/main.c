#include "window.h"
#include "game.h"
#include "version.h"

int main(int argc, char* argv[]) {
    // Initialize Game window
    //Window_Initialize(I_GAME_WIDTH, I_GAME_HEIGHT, I_GAME_TITLE, argc, argv);

    // Init Game handler
    Game_Initialize();

    return 0;
}