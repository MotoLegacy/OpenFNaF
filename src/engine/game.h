#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"

// Game-specific
extern void G_Main(u16_t night);
extern void G_GameLoop(void);
extern void G_AdvanceTime(void);

// For handling Game
void Game_Initialize(void);

#endif
