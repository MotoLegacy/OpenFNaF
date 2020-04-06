// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>

#include "game.h"
#include "types.h"
#include "window.h"
#include "version.h"

// The Only Keys we really use are
// Escape: Close Game
// LCTRL: Flashlight (FNaF 2 ONLY!)
void Input_CheckKeyboard() {
    if (sfKeyboard_isKeyPressed(sfKeyEscape)) {
        Game_Running = FALSE;
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