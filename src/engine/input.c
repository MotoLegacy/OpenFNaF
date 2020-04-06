// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "input.h"
#include "game.h"
#include "version.h"

// 16 should be enough.. right?
keydata_t KeyFunctions[16];
int Current_Key;

// Handle game input registration
void Input_RegisterKey(sfKeyCode key, void (*func)) {
    KeyFunctions[Current_Key].key = key;
    KeyFunctions[Current_Key].func = func;

    Current_Key++;
}

// Check game-defined key presses
void Input_CheckKeyboard() {
    // Escape is a global, non-controllable key.. always quit.
    if (sfKeyboard_isKeyPressed(sfKeyEscape)) {
        Game_Running = FALSE;
    }

    // Check our game-defined keys
    for (int i = 0; i < Current_Key; i++) {
        if (sfKeyboard_isKeyPressed(KeyFunctions[i].key) && KeyFunctions[i].pressed == FALSE) {
            KeyFunctions[i].func();
            KeyFunctions[i].pressed = TRUE;
        } else if (!sfKeyboard_isKeyPressed(KeyFunctions[i].key)) {
            // Key lifted, ready to press again!
            KeyFunctions[i].pressed = FALSE;
        }      
    }
}

// Room Scrolling
void Input_CheckMouse() {
    sfVector2i mouse = sfMouse_getPositionRenderWindow(GameWindow);

    // Scrolling Stuff, Kill if we can't Scroll.
    if (!Allow_Mouse_Scrolling)
        return;

    //
    // 50px deadzone
    //

    // Scroll Left
    /*if (mouse.x < ((I_GAME_WIDTH/2) - 50)) {
        printf("SCROLL LEFT\n");
    } else if (mouse.x > ((I_GAME_WIDTH/2) + 50)) { // Scroll Right
        printf("SCROLL RIGHT\n");
    } else { // No Scroll
        printf("NO SCROLL\n");
    }*/
}

void Input_CheckButtons() {
    Input_CheckKeyboard();
    Input_CheckMouse();
}