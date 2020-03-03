#ifndef _SAVE_H_
#define _SAVE_H_

#include "version.h"

//FNAF 1
#ifdef GAME_FNAF1
  #define SAVE_TITLE   "freddy"
#endif

//FNAF 2
#ifdef GAME_FNAF2
  #define SAVE_TITLE   "freddy2"
#endif

//FNAF 3
#ifdef GAME_FNAF3
  #define SAVE_TITLE   "freddy3"
#endif

extern void Save_Create(void);

#endif