#include "types.h"
#include "time.h"
#include "math.h"

#include <stdio.h>

void AI_CheckMovement(anima_t Anima) {
    // Get gametime to compare with update time
    int HundrethsTS = Current_Tsecond * 100;
    HundrethsTS /= 100;
    float GameTime = Current_Second + (float)HundrethsTS/100;

    if (GameTime == Anima.UpdateTime) {
        u16_t Calculation = Math_SeedRandom(420, 20);

        // Move if the die said we could
        if (Calculation <= Anima.AiLevel) {
            Anima.OnMove();
        }

        Anima.UpdateTime += Anima.UpdateInterval;
    }
        
    return;
}