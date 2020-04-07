// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "input.h"
#include "game.h"

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

// Mouse-Related functions
void Input_CheckMouse() {
    sfVector2i mouse = sfMouse_getPositionRenderWindow(GameWindow);

    // Scroll Stuff
    switch (Scroll_Method) {
        case SCROLL_AUTOMATIC:
            Game_AutoScroll();
            break;
        case SCROLL_MANUAL:
            Game_ManualScroll(mouse);
            break;
        default:
            break;
    }   
}

void Input_CheckButtons() {
    Input_CheckKeyboard();
    Input_CheckMouse();
}