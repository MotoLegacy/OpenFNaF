// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// Typedefs
typedef sfVector2i  vec2_t;
typedef sfKeyCode   keycode_t;

// Keys
typedef enum
{
    Key_Escape      = sfKeyEscape,
    Mouse_Left      = sfMouseLeft
} buttons_t;

extern bool Input_ButtonPressed(keycode_t key);
extern vec2_t Input_GetCursorPosition(window_t* Window);