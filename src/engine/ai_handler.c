#include "types.h"
#include "game.h"
#include "math.h"

// TEMP
#include <stdio.h>

void AI_CheckMovement(anima_t Anima) {
    double TimerDouble;

    // Get gametime in a double to easily compare with
    // anima's UpdateTime.
    TimerDouble = (double) Current_Second;
    TimerDouble += (double)(Current_Frame)/100;

    if (TimerDouble == Anima.UpdateTime) {
        u64_t Calculation = Math_SeedRandom(420, 20);

        if (Calculation <= Anima.AiLevel) {
            printf("shoula moved\n");
            // FIXME -- figure out how to execute this
            (&Anima.OnMove);
        }
    }
        
    return;
}