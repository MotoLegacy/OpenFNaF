#include "game.h"
#include "types.h"
#include "window.h"

#include <time.h>

#define FRAMES_PER_SECOND       60

int Current_Frame;
int Current_Second;

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

        // Update our Frame and Second counter
        // The Frame Counter works with AI handler
        // to do movement checks
        //
        // Note that these frames go up to 99, because
        // I'm bad.
        // - Moto
        Current_Frame++;

        if (Current_Frame >= 100) {
            Current_Second++;
            Current_Frame = 0;
        }

        if (Current_Second == 6)
            Current_Second = 1;

        //printf("frame %f\n", (double)Current_Frame/60);

        // Wait until FPS cap says we can do more
        delay(FRAMES_PER_SECOND * 300); // no clue why this is 300 lol
    }
}
