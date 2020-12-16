//
// MIT License
//
// Copyright (c) 2020 MotoLegacy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

//
// time_manager.c - single threaded timer
//

#include "defs.h"


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

boolean Time_FrameReady(u16_t id) {
    if (clock() > Delay_Time[id])
        return TRUE;
    
    return FALSE;
}

void Time_FrameDelay(unsigned short mseconds, u16_t id) {
    Delay_Time[id] = clock() + mseconds * 1000;
}