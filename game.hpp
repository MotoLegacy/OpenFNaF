#ifndef _GAME_H_
#define _GAME_H_

#include "version.hpp"

//function prototypes
void D_GameLoop(void);

//externs
extern int GM_NIGHT;
extern int GM_BEAR;
extern int GM_BUNNY;
extern int GM_BIRD;
extern int GM_FOX;

//macros

#ifdef GAME_FNAF1
  #define G_NUM_ANIMAS 5
#endif

#endif