// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"

// Game-specific
extern void G_Main(u16_t night);
extern void G_GameLoop(void);
extern void G_AdvanceTime(void);

// For handling Game
void Game_Initialize(void);
float Game_GetTime();

extern bool Game_Running;

extern bool Allow_Mouse_Scrolling;

#endif
