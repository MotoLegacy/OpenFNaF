// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "defs.h"
#include <stdio.h>

int Current_Precache_Amount = 0;
int Channels[MAX_SOUND_CHANNELS];

streamelement_t StreamElements[MAX_SOUND_CHANNELS];
soundelement_t SoundElements[MAX_PRECACHED_SOUNDS];

void Sound_FreeChannelStream(int Channel)
{
    Sound_StopStream(StreamElements[Channel].Stream);
    Channels[Channel] = FALSE;
}

void Sound_Clean()
{
    for (int i = 0; i < MAX_SOUND_CHANNELS; ++i)
    {
        if (Channels[i] == TRUE)
            Sound_FreeChannelStream(i);
    }
}

void Sound_Stream(int Channel, char* Sound, bool Loop, float Pitch, float Volume)
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
    Sound_SetStreamLoop(StreamElements[Channel].Stream, Loop);
    Sound_SetStreamPitch(StreamElements[Channel].Stream, Pitch);
    Sound_SetStreamVolume(StreamElements[Channel].Stream, Volume);

    // Play
    Sound_PlayStream(StreamElements[Channel].Stream);

    // Occupy the Channel
    Channels[Channel] = TRUE;
}

void Sound_Precache(char* Directory)
{
    if (Current_Precache_Amount >= MAX_PRECACHED_SOUNDS) {
        Print_Normal("Sound_Precache: Too many Precached Sounds!\n");
    }

    // Try To Load Sound, Fail if not exist.
    sndbuffer_t* SoundBuffer;
    SoundBuffer = Sound_LoadSound(Directory);

    if (!SoundBuffer) {
        Print_Normal("Sound_Precache: Tried to load a sound that doesn't exist! (%s)\n", Directory);
        return;
    }

    // Bind the Sound to the SoundBuffer
    sound_t* Sound;
    Sound_BindSoundToBuffer(Sound, SoundBuffer);

    // Add to Sound list
    SoundElements[Current_Precache_Amount].Sound = Sound;
    SoundElements[Current_Precache_Amount].SoundBuffer = SoundBuffer;

    // Increment Precache list
    ++Current_Precache_Amount;
}