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
// sound.c - Sound wrapper
//

#include "defs.h"

int Current_Precache_Amount = 0;
int Channels[MAX_SOUND_CHANNELS];

streamelement_t StreamElements[MAX_SOUND_CHANNELS];

void Sound_FreeChannelStream(int Channel)
{
    Sound_StopStream(StreamElements[Channel].Stream);
    Channels[Channel] = FALSE;
}

void Sound_Stream(int Channel, char* Sound, boolean Loop, float Pitch, float Volume) 
{
    // Quit if channel provided is too high
    if (Channel >= MAX_SOUND_CHANNELS) {
        Print_Normal("ERR: Tried to Stream Sound in Channel %i when max is %i!\n", Channel, MAX_SOUND_CHANNELS - 1);
        return;
    }

    // Free whatever is in this channel if its occupied already.
    if (Channels[Channel] == TRUE)
        Sound_FreeChannelStream(Channel);

    // Load
    StreamElements[Channel].Stream = Sound_StreamFromFile(Sound);

    // Quit if sound doesnt exist
    if (!StreamElements[Channel].Stream) {
        Print_Normal("ERR: Sound '%s' does not exist or is invalid!\n", Sound);
        return;
    }

    // Set Fields
#ifndef PSP
    Sound_SetStreamLoop(StreamElements[Channel].Stream, Loop);
    Sound_SetStreamPitch(StreamElements[Channel].Stream, Pitch);
    Sound_SetStreamVolume(StreamElements[Channel].Stream, Volume);
#endif

    // Play
    Sound_PlayStream(StreamElements[Channel].Stream, Channel);

    // Occupy the Channel
    Channels[Channel] = TRUE;
}