// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

extern u16_t Current_Tsecond;
extern u16_t Current_Second;

extern void Time_Tick();
extern void Time_FrameDelay(unsigned short mseconds, u16_t id);
extern bool Time_FrameReady(u16_t id);