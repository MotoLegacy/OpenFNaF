#include "game.h"
#include "types.h"
#include "window.h"

void Game_Initialize(void) {
    bool Running;
    Running = TRUE;

    G_Main();
    
    while(Running) {
        // Run our Game's loop
        G_GameLoop();

        // Update our Window at the end of every frame
        Window_Update();
    }
}
