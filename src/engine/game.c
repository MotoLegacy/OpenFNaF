// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "game.h"
#include "types.h"
#include "time.h"
#include "window.h"
#include "options.h"
#include "save.h"
#include "input.h"

#include <stdio.h>

#define FRAMES_PER_SECOND       60

bool Game_Running = FALSE;

void Game_Initialize(void) {
    u8_t hour;
    hour = 0;

    Game_Running = TRUE;

    // Create save file and initialize some
    // needed fields
    if (!Save_Exist()) {
        Save_Create();
        Save_SetValue("level", 1);
    }

    // Execute game's main function with night
    G_Main(Save_GetValue("level"));

    // Basically just initialize our framedelays
    Time_FrameDelay(1, 0); // Frames per Second
    Time_FrameDelay(1, 1); // Actual Game Timer

    while(Game_Running) {

        // Non-graphics stuff
        if (Time_FrameReady(1)) {
            // Run our Game's loop
            G_GameLoop();

            // Increment Game Time
            Time_Tick();

            // Input Stuff
            Input_CheckButtons();

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

    Window_Close();
}

float Game_GetTime() {
    return (Current_Second + ((float)Current_Tsecond/100));
}
