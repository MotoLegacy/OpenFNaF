#include "types.h"
#include "game.h"
#include "math.h"

// TEMP
#include <stdio.h>

void AI_CheckMovement(anima_t Anima) {
    float TimerFloat;

    // Get gametime in a float to easily compare with
    // anima's UpdateTime.
    TimerFloat = (float) Current_Second;
    TimerFloat += (float)(Current_Frame)/100;

    if (TimerFloat == Anima.UpdateTime) {
        u16_t Calculation = Math_SeedRandom(420, 20);

        if (Calculation <= Anima.AiLevel) {
            printf("shoula moved\n");
            // FIXME -- figure out how to execute this
            (&Anima.OnMove);
        }
    }
        
    return;
}