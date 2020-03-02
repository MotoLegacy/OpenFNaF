#include "types.h"

#include <time.h>


u16_t Current_Tsecond;
u16_t Current_Second;
u32_t Delay_Time[5];

clock_t Start_Time;

void Time_Tick() {
    Current_Tsecond++;

    if (Current_Tsecond > 99) {
        Current_Second++;
        Current_Tsecond = 0;
    }
}

bool Time_FrameReady(unsigned int id) {
    if (clock() > Delay_Time[id])
        return TRUE;
    
    return FALSE;
}

void Time_FrameDelay(unsigned int mseconds, unsigned int id) {
    Delay_Time[id] = clock() + mseconds * 1000;
}