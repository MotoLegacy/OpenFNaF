// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "../defs.h"

bool Input_ButtonPressed(keycode_t key)
{
    // Mouse Button
    if (key == (keycode_t)Mouse_Left)
        return sfMouse_isButtonPressed(key);

    // Standard Keyboard
    return sfKeyboard_isKeyPressed(key);
}

vec2_t Input_GetCursorPosition(window_t* Window)
{
    return sfMouse_getPositionRenderWindow(Window);
}