// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#ifndef _MATH_MANAGER_H_
#define _MATH_MANAGER_H_

#include <stdlib.h>
#include "types.h"

//
// prototypes
//

bool    Math_GenerateChance(u64_t percent);
void    Math_SetSeed(u64_t dseed);
u64_t   Math_SeedRandom(u64_t max);
u8_t    Math_Prng(void);

#endif