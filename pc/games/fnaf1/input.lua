--[[
=============
  input.lua
=============
OpenFNaF Input wrapper in LUA
]]

-- Input Modes
INPUT_BUTTON_DOWN       = 0
INPUT_KEYBOARD_DOWN     = 1

-- Controller
INPUT_CROSS             = 0
INPUT_SQUARE            = 1
INPUT_TRIANGLE          = 2
INPUT_CIRCLE            = 3
INPUT_UP                = 4
INPUT_DOWN              = 5
INPUT_LEFT              = 6
INPUT_RIGHT             = 7
INPUT_LTRIGGER          = 8
INPUT_RTRIGGER          = 9
INPUT_START             = 10
INPUT_SELECT            = 11

-- Keyboard
INPUT_KEY_A             = 0
INPUT_KEY_B             = 1
INPUT_KEY_C             = 2
INPUT_KEY_D             = 3
INPUT_KEY_E             = 4
INPUT_KEY_F             = 5
INPUT_KEY_G             = 6
INPUT_KEY_H             = 7
INPUT_KEY_I             = 8
INPUT_KEY_J             = 9
INPUT_KEY_K             = 10
INPUT_KEY_L             = 11
INPUT_KEY_M             = 12
INPUT_KEY_N             = 13
INPUT_KEY_O             = 14
INPUT_KEY_P             = 15
INPUT_KEY_Q             = 16
INPUT_KEY_R             = 17
INPUT_KEY_S             = 18
INPUT_KEY_T             = 19
INPUT_KEY_U             = 20
INPUT_KEY_V             = 21
INPUT_KEY_W             = 22
INPUT_KEY_X             = 23
INPUT_KEY_Y             = 24
INPUT_KEY_Z             = 25
INPUT_KEY_0             = 26
INPUT_KEY_1             = 27
INPUT_KEY_2             = 28
INPUT_KEY_3             = 29
INPUT_KEY_4             = 30
INPUT_KEY_5             = 31
INPUT_KEY_6             = 32
INPUT_KEY_7             = 33
INPUT_KEY_8             = 34
INPUT_KEY_9             = 35
INPUT_KEY_LCTRL         = 36
INPUT_KEY_LSHIFT        = 37
INPUT_KEY_LALT          = 38
-- skip 39 because its the winkey and that's a lame key
INPUT_KEY_RCTRL         = 40
INPUT_KEY_RSHIFT        = 41
INPUT_KEYRALT           = 42

function Input_ButtonDown(button)
    if OF_GetInput(button, INPUT_BUTTON_DOWN) ~= 0 then
        return true
    else
        return false
    end
end

function Input_KeyDown(key)
    if OF_GetInput(key, INPUT_KEYBOARD_DOWN) ~= 0 then
        return true
    else
        return false
    end
end