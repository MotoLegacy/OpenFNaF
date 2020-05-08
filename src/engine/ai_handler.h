// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#ifndef _AIHANDLER_H_
#define _AIHANDLER_H_

#include "types.h"

// FIXME - possible move this to game.h?
extern anima_t Animas[G_NUM_ANIMAS];

extern void AI_CheckMovement(anima_t* Anima);
extern void AI_RegisterStateChange(anima_t* Anima, u16_t State);

#endif
