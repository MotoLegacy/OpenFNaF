// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include <stdio.h>
#include <SFML/System.h>
#include <SFML/Window.h>

#include "game.h"
#include "types.h"

// The Only Keys we use are
// Escape: Close Game
// LCTRL: Flashlight (FNaF 2 ONLY!)
void Input_CheckKeyboard() {
    if (sfKeyboard_isKeyPressed(sfKeyEscape)) {
        Game_Running = FALSE;
    }
}

void Input_CheckButtons() {
    Input_CheckKeyboard();
}