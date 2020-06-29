// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "../defs.h"

stream_t* Sound_StreamFromFile(char* Sound)
{
    return sfMusic_createFromFile(Sound);
}

void Sound_SetStreamLoop(stream_t* Stream, bool Loop)
{
    sfMusic_setLoop(Stream, Loop);
}

void Sound_SetStreamPitch(stream_t* Stream, float Pitch)
{
    sfMusic_setPitch(Stream, Pitch);
}

void Sound_SetStreamVolume(stream_t* Stream, float Volume)
{
    sfMusic_setVolume(Stream, Volume);
}

void Sound_PlayStream(stream_t* Stream)
{
    sfMusic_play(Stream);
}

void Sound_StopStream(stream_t* Stream)
{
    sfMusic_stop(Stream);
    sfMusic_destroy(Stream);
}

sndbuffer_t* Sound_LoadSound(char* Directory)
{
    return sfSoundBuffer_createFromFile(Directory);
}

void Sound_BindSoundToBuffer(sound_t* Sound, sndbuffer_t* SoundBuffer)
{
    sfSound_setBuffer(Sound, SoundBuffer);
}