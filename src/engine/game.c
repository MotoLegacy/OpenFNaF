#include "game.h"
#include "time.h"
#include "types.h"
#include "window.h"

#define FRAMES_PER_SECOND       60

void Game_Initialize(void) {
    bool Running;

    int hour;
    hour = 1;

    Running = TRUE;

    // TODO: Proper saving, just be night one for now.
    G_Main(1);
    
    while(Running) {
        // Run our Game's loop
        G_GameLoop();

        // Increment Game Time
        Time_Tick();

        // Check if we should allow game time advance
        if (Current_Second >= SECONDS_PER_HOUR * hour) {
            G_AdvanceTime();
            hour++;
        }

        // Update our Window at the end of every frame
        Window_Update();

        // Wait until FPS cap says we can do more
        delay(FRAMES_PER_SECOND * 300); // no clue why this is 300 lol
    }
}
