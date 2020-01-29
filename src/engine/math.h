#ifndef _MATH_H_
#define _MATH_H_

#include <stdlib.h>
#include "types.h"

//
// prototypes
//

bool    Math_GenerateChance(u64_t percent);
u64_t   Math_SeedRandom(u64_t seed, u64_t max);
u8_t    Math_Prng(void);

#endif