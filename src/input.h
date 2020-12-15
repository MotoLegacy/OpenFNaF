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
#define INPUT_KEY_A             0
#define INPUT_KEY_B             1
#define INPUT_KEY_C             2
#define INPUT_KEY_D             3
#define INPUT_KEY_E             4
#define INPUT_KEY_F             5
#define INPUT_KEY_G             6
#define INPUT_KEY_H             7
#define INPUT_KEY_I             8
#define INPUT_KEY_J             9
#define INPUT_KEY_K             10
#define INPUT_KEY_L             11
#define INPUT_KEY_M             12
#define INPUT_KEY_N             13
#define INPUT_KEY_O             14
#define INPUT_KEY_P             15
#define INPUT_KEY_Q             16
#define INPUT_KEY_R             17
#define INPUT_KEY_S             18
#define INPUT_KEY_T             19
#define INPUT_KEY_U             20
#define INPUT_KEY_V             21
#define INPUT_KEY_W             22
#define INPUT_KEY_X             23
#define INPUT_KEY_Y             24
#define INPUT_KEY_Z             25
#define INPUT_KEY_0             26
#define INPUT_KEY_1             27
#define INPUT_KEY_2             28
#define INPUT_KEY_3             29
#define INPUT_KEY_4             30
#define INPUT_KEY_5             31
#define INPUT_KEY_6             32
#define INPUT_KEY_7             33
#define INPUT_KEY_8             34
#define INPUT_KEY_9             35
#define INPUT_KEY_LCTRL         36
#define INPUT_KEY_LSHIFT        37
#define INPUT_KEY_LALT          38
// skip 39 because its the winkey and that's a lame key
#define INPUT_KEY_RCTRL         40
#define INPUT_KEY_RSHIFT        41
#define INPUT_KEYRALT           42

extern void Input_Initialize();
extern void Input_Loop();

#ifndef DESKTOP
extern int Input_ButtonPressedDown(int button);
#endif

#ifdef DESKTOP
extern int Input_KeyboardDown(int key);
#endif