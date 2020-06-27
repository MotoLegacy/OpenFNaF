// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "defs.h"

int Channels[MAX_SOUND_CHANNELS];
streamelement_t StreamElements[MAX_SOUND_CHANNELS];

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