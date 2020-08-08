// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "defs.h"
#include <stdio.h>

keydata_t KeyFunctions[MAX_KEYFUNCTIONS];
int Current_Key;

// Handle game input registration
void Input_RegisterKey(keycode_t key, void (*func)) {
    KeyFunctions[Current_Key].key = key;
    KeyFunctions[Current_Key].func = func;

    Current_Key++;
}

// Check game-defined key presses
void Input_CheckKeyboard() {
    // Escape is a global, non-controllable key.. always quit.
    if (Input_ButtonPressed(Key_Escape)) {
        Game_Running = FALSE;
    }

    // Check our game-defined keys
    for (int i = 0; i < Current_Key; ++i) {
        if (Input_ButtonPressed(KeyFunctions[i].key) && KeyFunctions[i].pressed == FALSE) {
            KeyFunctions[i].func();
            KeyFunctions[i].pressed = TRUE;
        } else if (!Input_ButtonPressed(KeyFunctions[i].key)) {
            // Key lifted, ready to press again!
            KeyFunctions[i].pressed = FALSE;
        }      
    }
}

// Mouse-Related functions
void Input_CheckMouse() {
    vec2_t mouse = Input_GetCursorPosition(GameWindow);

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

    // Check UI Elements
    int MinX;
    int MaxX;
    int MinY;
    int MaxY;
    for (int i = 0; i < Current_Element; ++i) {
        // Just CULL here if it isnt visible
        if (!UIElements[i]->Visible)
            return;

        // Set Up Boundaries
        MinX = Graphics_GetSpriteXPosition(UIElements[i]->Sprite);
        MaxX = MinX + Graphics_GetTextureWidth(UIElements[i]->Texture);
        MinY =  Graphics_GetSpriteYPosition(UIElements[i]->Sprite);
        MaxY = MinY + Graphics_GetTextureHeight(UIElements[i]->Texture);
        // Our Cursor is on the X Pos
        if (mouse.x >= MinX && mouse.x <= MaxX) {
            // Our Cursor is on the Y Pos
            if (mouse.y >= MinY &&  mouse.y <= MaxY) {
                // Remove Spam
                if (UIElements[i]->Activated)
                    return;
                
                // Check for Click if needbe
                if (UIElements[i]->Need_Clicked) {
                    if (Input_ButtonPressed(Mouse_Left)) {
                        UIElements[i]->func();
                        UIElements[i]->Activated = TRUE;
                    }
                } else {
                    UIElements[i]->func();
                    UIElements[i]->Activated = TRUE;
                }
            } else {
                UIElements[i]->Activated = FALSE;
            }
        } else {
            UIElements[i]->Activated = FALSE;
        }
    }
}

void Input_CheckButtons() {
    Input_CheckKeyboard();
    Input_CheckMouse();
}