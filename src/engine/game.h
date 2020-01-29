#ifndef _GAME_H_
#define _GAME_H_

// Game-specific
extern void G_Main(void);
extern void G_GameLoop(void);

extern int Current_Frame;
extern int Current_Second;

// For handling Game
void Game_Initialize(void);

#endif
