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
INPUT_KEY_A             = 65
INPUT_KEY_B             = 66
INPUT_KEY_C             = 67
INPUT_KEY_D             = 68
INPUT_KEY_E             = 69
INPUT_KEY_F             = 70
INPUT_KEY_G             = 71
INPUT_KEY_H             = 72
INPUT_KEY_I             = 73
INPUT_KEY_J             = 74
INPUT_KEY_K             = 75
INPUT_KEY_L             = 76
INPUT_KEY_M             = 77
INPUT_KEY_N             = 78
INPUT_KEY_O             = 79
INPUT_KEY_P             = 80
INPUT_KEY_Q             = 81
INPUT_KEY_R             = 82
INPUT_KEY_S             = 83
INPUT_KEY_T             = 84
INPUT_KEY_U             = 85
INPUT_KEY_V             = 86
INPUT_KEY_W             = 87
INPUT_KEY_X             = 88
INPUT_KEY_Y             = 89
INPUT_KEY_Z             = 90
INPUT_KEY_0             = 320
INPUT_KEY_1             = 321
INPUT_KEY_2             = 322
INPUT_KEY_3             = 323
INPUT_KEY_4             = 324
INPUT_KEY_5             = 325
INPUT_KEY_6             = 326
INPUT_KEY_7             = 327
INPUT_KEY_8             = 328
INPUT_KEY_9             = 329
INPUT_KEY_LCTRL         = 341
INPUT_KEY_LSHIFT        = 340
INPUT_KEY_LALT          = 342
INPUT_KEY_RCTRL         = 345
INPUT_KEY_RSHIFT        = 344
INPUT_KEY_RALT          = 346

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