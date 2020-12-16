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
soundelement_t SoundElements[MAX_PRECACHED_SOUNDS];

void Sound_FreeChannelStream(int Channel)
{
    Sound_StopStream(StreamElements[Channel].Stream);
    Channels[Channel] = FALSE;
}

void Sound_FreeChannel(int Channel, sound_t* Sound)
{
    Sound_StopSound(Sound);
    Channels[Channel] = FALSE;
}

#ifdef DESKTOP
void Sound_Clean()
{
    for (int i = 0; i < MAX_SOUND_CHANNELS; ++i)
    {
        if (Channels[i] == TRUE)
            Sound_FreeChannelStream(i);
    }
}
#endif

#ifdef PSP
void Sound_Clean()
{
    for (int i = 0; i < Current_Precache_Amount; i++) {
        Sound_Delete(SoundElements[i].Sound);
    }

    for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
        if (Channels[i] == TRUE && StreamElements[i].Stream) {
            Sound_StopStream(StreamElements[i].Stream);
            Sound_Delete(StreamElements[i].Stream);
        } else if (Channels[i] == TRUE && SoundElements[i].Sound) {
            Sound_StopSound(SoundElements[i].Sound);
        }
    }
}
#endif

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
    // TODO: Deal with this on PSP lol
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

void Sound_Precache(char* Directory)
{
    if (Current_Precache_Amount >= MAX_PRECACHED_SOUNDS) {
        Print_Normal("Sound_Precache: Too many Precached Sounds!\n");
    }

    // Try To Load Sound, Fail if not exist.
#ifdef DESKTOP
    sndbuffer_t* SoundBuffer;
    SoundBuffer = Sound_LoadSound(Directory);
#elif PSP
    sound_t* Sound;
    Sound = Sound_LoadSound(Directory);
#endif

#ifdef DESKTOP
    if (!SoundBuffer) {
#elif PSP
    if (!Sound) {
#endif
        Print_Normal("Sound_Precache: Tried to load a sound that doesn't exist! (%s)\n", Directory);
        return;
    }

    // Bind the Sound to the SoundBuffer
#ifdef DESKTOP
    sound_t* Sound;
    Sound = Sound_Create();
    Sound_BindSoundToBuffer(Sound, SoundBuffer);
#endif

    // Add to Sound list
    SoundElements[Current_Precache_Amount].Sound = Sound;
#ifdef DESKTOP
    SoundElements[Current_Precache_Amount].SoundBuffer = SoundBuffer;
#endif
    SoundElements[Current_Precache_Amount].File = Directory;

    // Increment Precache list
    ++Current_Precache_Amount;
}

void Sound_Play(int Channel, char* Directory, boolean Loop, float Pitch, float Volume)
{
    // Quit if channel provided is too high
    if (Channel >= MAX_SOUND_CHANNELS) {
        Print_Normal("ERR: Tried to Play Cached Sound in Channel %i when max is %i!\n", Channel, MAX_SOUND_CHANNELS - 1);
        return;
    }

    // Find the precache index associated with the file path.
    sound_t* Sound;
    for(int i = 0; i < Current_Precache_Amount; ++i) {
        if (strcmp(SoundElements[i].File, Directory) == 0) {
            Sound = SoundElements[i].Sound;
        }
    }

    // Ensure the Sound is valid
    if (!Sound) {
        Print_Normal("ERR: Sound '%s' does not exist, is invalid, or was not precached!\n", Sound);
        return;
    }

    // Free whatever is in this channel if its occupied already.
    if (Channels[Channel] == TRUE)
        Sound_FreeChannel(Channel, Sound);

    // Set Fields
    // TODO: Deal with this on PSP lol
#ifndef PSP
    Sound_SetSoundPitch(Sound, Pitch);
    Sound_SetSoundLoop(Sound, Loop);
    Sound_SetSoundVolume(Sound, Volume);
#endif

    // Play
    Sound_PlaySound(Sound, Channel);

    // Occupy the Channel
    Channels[Channel] = TRUE;
}