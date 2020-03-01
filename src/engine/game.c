#include <stdio.h>

#include "game.h"
#include "types.h"
#include "time.h"
#include "window.h"

#define FRAMES_PER_SECOND       30

void Game_Initialize(void) {
    bool Running;

    int hour;
    hour = 1;

    int i = 0;

    Running = TRUE;

    // TODO: Proper saving, just be night one for now.
    G_Main(1);
    
    while(Running) {
        // Run our Game's loop
        G_GameLoop();

        // Increment Game Time
        Time_Tick();
        i++;

        // Check if we should allow game time advance
        if (Current_Second >= SECONDS_PER_HOUR * hour) {
            G_AdvanceTime();
            hour++;
        }

        // Update our Window at the end of every frame
        Window_Update();

        // Limit interations of non-time calculating code, per FPS cap
        delay(1000/FRAMES_PER_SECOND);
    }
}
