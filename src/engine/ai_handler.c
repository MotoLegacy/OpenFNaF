// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "types.h"
#include "time.h"
#include "math.h"
#include "game.h"

#include <stdio.h>

void AI_CheckMovement(anima_t* Anima) {
    // Get gametime to compare with update time
    float GameTime = Game_GetTime();

    if (GameTime == Anima->UpdateTime) {
        u16_t Calculation = Math_SeedRandom(20);

        // Move if the die said we could
        if (Calculation <= Anima->AiLevel) {
            Anima->OnMove();
        }

        // Increase Update Time, round to nearest hundreth.
        Anima->UpdateTime += Anima->UpdateInterval;
        Anima->UpdateTime = floor(Anima->UpdateTime*100+0.5)/100;
    }
        
    return;
}
