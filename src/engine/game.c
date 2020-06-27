// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "defs.h"
#include <stdio.h>
#include <time.h>

#define FRAMES_PER_SECOND       60

bool Game_Running = FALSE;

//
// Game_Intialize
// Set up our back-end and run our game loop
//
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

    // Seed the Random Number Generator
    Math_SetSeed(clock());

    // Load our Font System
    Graphics_InitializeFont();

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
            // Clear Window
            Window_Clear();
            
            // Draw Current Room before All Else
            Graphics_DrawRoom(Current_Room);

            // Lastly, Update any UI Elements (including Text)
            Graphics_DrawTextElements();
            Graphics_DrawUI();

            // Update our Window
            if (!OPT_NORENDER)
                Window_Update();

            // Set time until next frame is ready
            Time_FrameDelay(1000/FRAMES_PER_SECOND, 0);
        }
    }

    // Clean All Sounds
    Sound_Clean();

    // Close Window
    Window_Close();
}

//
// Game_GetTime
// Returns the in-game time
//
float Game_GetTime() {
    return (Current_Second + ((float)Current_Tsecond/100));
}


//
// Game_ManualScroll
// Handles Determining RoomPan Distance
//
int LeftScreenThreshold;
void Game_ManualScroll(vec2_t mouse) {
    // center buffer and pan dist. calculation
    int deadzone = 50;
    int distance = mouse.x - I_GAME_WIDTH/2;
    LeftScreenThreshold = I_GAME_WIDTH - Graphics_GetTextureWidth(RoomTexture);

    // Scroll Left
    if (mouse.x < ((I_GAME_WIDTH/2) - deadzone)) {
        RoomPanX += -distance/(I_GAME_WIDTH/8);
    } 
    // Scroll Right
    else if (mouse.x > ((I_GAME_WIDTH/2) + deadzone)) {
        RoomPanX -= distance/(I_GAME_WIDTH/8);
    }

    // Scroll Borders
    if (RoomPanX >= 0)
        RoomPanX = 0;

    if (RoomPanX <= LeftScreenThreshold)
        RoomPanX = LeftScreenThreshold;      
}

//
// Game_AutoScroll
// Automatic Scrolling
// (~8s movement, linger for 2 on edge)
// FIXME - This is MEGA UGLY
//
bool Other_Way = FALSE;
bool Scroll_Lingering = FALSE;
void Game_AutoScroll() {
    LeftScreenThreshold = I_GAME_WIDTH - Graphics_GetTextureWidth(RoomTexture);
    float PanAmount = (float)I_GAME_WIDTH/(I_GAME_WIDTH*2.5);

    if (!Scroll_Lingering) {
        // Left to Right
        if (!Other_Way) {
            CameraPanX += PanAmount;

            // Edge Check
            if (CameraPanX >= 0) {
                Scroll_Lingering = TRUE;
                Time_FrameDelay(2000, 2); // 2 second linger
            }
        }
        // Right to Left
        else {
            CameraPanX -= PanAmount;

            // Edge Check
            if (CameraPanX <= LeftScreenThreshold) {
                Scroll_Lingering = TRUE;
                Time_FrameDelay(2000, 2); // 2 second linger
            }
        }
    } else {
        // if we've lingered long enough
        if (Time_FrameReady(2)) {
            Scroll_Lingering = FALSE;
            Other_Way = !Other_Way;
        }
    }    
}