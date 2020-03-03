#include "game.h"
#include "types.h"
#include "time.h"
#include "window.h"
#include "options.h"
#include "save.h"

#include <stdio.h>

#define FRAMES_PER_SECOND       60

void Game_Initialize(void) {
    bool Running;

    u8_t hour;
    hour = 0;

    Running = TRUE;

    if (!Save_Exist()) {
        Save_Create();
    }

    // TODO: Proper saving, just be night one for now.
    G_Main(Save_GetValue("level"));

    // Basically just initialize our framedelays
    Time_FrameDelay(1, 0); // Frames per Second
    Time_FrameDelay(1, 1); // Actual Game Timer

    while(Running) {

        // Non-graphics stuff
        if (Time_FrameReady(1)) {
            // Run our Game's loop
            G_GameLoop();

            // Increment Game Time
            Time_Tick();

            // Next Game Time iteration
            Time_FrameDelay(10, 1);
        }

        // Check if we should allow game time advance
        if (Current_Second >= SECONDS_PER_HOUR * (hour + 1)) {
            G_AdvanceTime();
            hour++;
        }

        // New Frame
        if (Time_FrameReady(0)) {
            // Update our Window
            if (!OPT_NORENDER)
                Window_Update();

            // Set time until next frame is ready
            Time_FrameDelay(1000/FRAMES_PER_SECOND, 0);
        }
    }
}

float Game_GetTime() {
    return (Current_Second + ((float)Current_Tsecond/100));
}
