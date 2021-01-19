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
// pc_sound.c - Sound Streaming, loading, etc. for PC
//

#include "../defs.h"

void Sound_PlayStream(stream_t* Stream, int Channel) {
    PlayMusicStream(*Stream);
}

void Sound_StopStream(stream_t* Stream) {
    StopMusicStream(*Stream);
    UnloadMusicStream(*Stream);
}

stream_t* Sound_StreamFromFile(char* File) {
    stream_t* TempStreamPtr = (stream_t*)malloc(sizeof(stream_t));

    if (TempStreamPtr != NULL)
        *TempStreamPtr = LoadMusicStream(File);

    return TempStreamPtr;
}

sound_t* Sound_LoadSound(char* Directory) {
    sound_t TempSound;
    sound_t* TempSoundPtr;
    TempSound = LoadSound(Directory);
    TempSoundPtr = malloc(sizeof(sound_t));

    TempSoundPtr->stream = TempSound.stream;
    TempSoundPtr->sampleCount = TempSound.sampleCount;

    return TempSoundPtr;
}

void Sound_PlaySound(sound_t* Sound, int Channel) {
    PlaySound(*Sound);
}

void Sound_StopSound(sound_t* Sound) {
    StopSound(*Sound);
}

void Sound_Delete(sound_t* Sound) {
    UnloadSound(*Sound);
}

void Sound_SetStreamLoop(stream_t* Stream, boolean Loop) {
    // TODO
}

void Sound_SetStreamPitch(stream_t* Stream, float Pitch) {
    SetMusicPitch(*Stream, Pitch);
}

void Sound_SetStreamVolume(stream_t* Stream, float Volume) {
    SetMusicVolume(*Stream, Volume);
}

void Sound_SetSoundLoop(sound_t* Sound, boolean Loop) {
    // TODO
}

void Sound_SetSoundPitch(sound_t* Sound, float Pitch) {
    SetSoundPitch(*Sound, Pitch);
}

void Sound_SetSoundVolume(sound_t* Sound, float Volume) {
    SetSoundVolume(*Sound, Volume);
}