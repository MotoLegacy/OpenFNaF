// Input States
#define INPUT_BUTTON_DOWN       0
#define INPUT_KEYBOARD_DOWN     1

// PSP Inputs
#define INPUT_PSP_CROSS         0
#define INPUT_PSP_SQUARE        1
#define INPUT_PSP_TRIANGLE      2
#define INPUT_PSP_CIRCLE        3
#define INPUT_PSP_UP            4
#define INPUT_PSP_DOWN          5
#define INPUT_PSP_LEFT          6
#define INPUT_PSP_RIGHT         7
#define INPUT_PSP_LTRIGGER      8
#define INPUT_PSP_RTRIGGER      9
#define INPUT_PSP_START         10
#define INPUT_PSP_SELECT        11

// Keyboard Inputs
#define INPUT_KEY_A             65
#define INPUT_KEY_B             66
#define INPUT_KEY_C             67
#define INPUT_KEY_D             68
#define INPUT_KEY_E             69
#define INPUT_KEY_F             70
#define INPUT_KEY_G             71
#define INPUT_KEY_H             72
#define INPUT_KEY_I             73
#define INPUT_KEY_J             74
#define INPUT_KEY_K             75
#define INPUT_KEY_L             76
#define INPUT_KEY_M             77
#define INPUT_KEY_N             78
#define INPUT_KEY_O             79
#define INPUT_KEY_P             80
#define INPUT_KEY_Q             81
#define INPUT_KEY_R             82
#define INPUT_KEY_S             83
#define INPUT_KEY_T             84
#define INPUT_KEY_U             85
#define INPUT_KEY_V             86
#define INPUT_KEY_W             87
#define INPUT_KEY_X             88
#define INPUT_KEY_Y             89
#define INPUT_KEY_Z             90
#define INPUT_KEY_0             320
#define INPUT_KEY_1             321
#define INPUT_KEY_2             322
#define INPUT_KEY_3             323
#define INPUT_KEY_4             324
#define INPUT_KEY_5             325
#define INPUT_KEY_6             326
#define INPUT_KEY_7             327
#define INPUT_KEY_8             328
#define INPUT_KEY_9             329
#define INPUT_KEY_LCTRL         341
#define INPUT_KEY_LSHIFT        340
#define INPUT_KEY_LALT          342
#define INPUT_KEY_RCTRL         345
#define INPUT_KEY_RSHIFT        344
#define INPUT_KEY_RALT          346

extern void Input_Initialize();
extern void Input_Loop();

#ifndef DESKTOP
extern int Input_ButtonPressedDown(int button);
#endif

#ifdef DESKTOP
extern int Input_KeyboardDown(int key);
#endif