#include "game.h"
#include "types.h"
#include "window.h"

#include <time.h>

#define FRAMES_PER_SECOND       60

// Really no better way to put this unless we have more time funcs. in the
// future.. time.c when
void delay(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void Game_Initialize(void) {
    bool Running;

    Running = TRUE;

    G_Main();
    
    while(Running) {
        // Run our Game's loop
        G_GameLoop();

        // Update our Window at the end of every frame
        Window_Update();

        // Wait until FPS cap says we can do more
        delay(FRAMES_PER_SECOND * 300); // no clue why this is 300 lol
    }
}
