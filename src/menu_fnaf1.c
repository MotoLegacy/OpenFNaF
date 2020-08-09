// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "engine/defs.h"
#include <stdio.h>
#include <string.h>

// Game
extern gamestate_t Game;

// Text Elements
textelement_t Text_Title;
textelement_t Text_NewGame;
textelement_t Text_Continue;
textelement_t Text_Selector;

// Dummy 'Room' for Menu
gameroom_t Menu_Room;

// Freddy Head
char* Current_Head;

// fixme
u16_t Dummy_Night;

// 
// G_Main(night)
// Game-Specific Main function, start menu.
//
func_t G_Main(u16_t night) {
    // Game.State 0 = menu
    Game.State = 0;

    // Store night
    Dummy_Night = night;

    // Dummy 'Room' for Menu
    Scroll_Method = SCROLL_DISABLED;
    Menu_Room.Graphic = "assets/textures/menu/bg/0.png";
    Current_Head = "assets/textures/menu/bg/0.png";
    Graphics_UpdateRoom(Menu_Room);

    // Game Title
    Text_Title.Text = "Five\nNights\nat\nFreddy's";
    Text_Title.XAnchor = UI_ANCHOR_LEFT;
    Text_Title.YAnchor = UI_ANCHOR_TOP;
    Text_Title.Color = COLOR_WHITE;
    Text_Title.Size = 35;
    Text_Title.XPosPercent = 10;
    Text_Title.YPosPercent = 10;
    Graphics_RegisterTextElement(&Text_Title);

    // New Game
    Text_NewGame.Text = "New Game";
    Text_NewGame.XAnchor = UI_ANCHOR_LEFT;
    Text_NewGame.YAnchor = UI_ANCHOR_TOP;
    Text_NewGame.Color = COLOR_WHITE;
    Text_NewGame.Size = 35;
    Text_NewGame.XPosPercent = 10;
    Text_NewGame.YPosPercent = 50;
    Graphics_RegisterTextElement(&Text_NewGame);

    // Menu Theme
    Sound_Stream(0, "assets/sounds/menu/theme.ogg", TRUE, 1, 100);

    // Head Timer
    Time_FrameDelay(0, 3);
}

//
// G_MenuLoop()
// Called by G_GameLoop, menu-specific loop.
//
func_t G_MenuLoop(void) {
    // Freddy's Head change
    if (Time_FrameReady(3)) {
        // Decide which image to use
        int Head_Value = Math_SeedRandom(3);

        // An override to make sure Head 0 outweighs the other 3
        if (Math_GenerateChance(85)) {
            Head_Value = 0;
        }

        // Get the image directory
        char* Head_Name = "assets/textures/menu/bg/";
        Head_Name = append_int_to_char(Head_Name, Head_Value);
        Head_Name = append_char_to_char(Head_Name, ".png");


        Menu_Room.Graphic = Head_Name;
        Graphics_UpdateRoom(Menu_Room);

        Time_FrameDelay(100, 3);
    }

    // TEMP: Press Left Mouse to Start
    if (Input_ButtonPressed(Mouse_Left)) {
        Sound_Stream(0, "assets/sounds/null.ogg", TRUE, 1, 0);
        Text_NewGame.Text = "";
        Text_Title.Text = "";
        Graphics_UpdateTextElement(&Text_Title);
        Graphics_UpdateTextElement(&Text_NewGame);
        G_GameMain(Dummy_Night);
    }
}