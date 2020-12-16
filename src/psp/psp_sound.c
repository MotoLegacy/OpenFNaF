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
// psp_sound.c - Sound Streaming, loading, etc. for PSP
//

#include "../defs.h"

void Sound_Initialize() {
    oslInit(0);
    oslInitAudio();
    oslInitAudioME(OSL_FMT_MP3);
}

void Sound_PlayStream(stream_t* Stream, int Channel) {
    oslPlaySound(Stream, Channel);
}

void Sound_StopStream(stream_t* Stream) {
    oslStopSound(Stream);
    oslDeleteSound(Stream);
}

stream_t* Sound_StreamFromFile(char* File) {
    return oslLoadSoundFileMP3(File, OSL_FMT_STREAM);
}

sound_t* Sound_LoadSound(char* Directory) {
    return oslLoadSoundFile(Directory, OSL_FMT_NONE);
}

void Sound_PlaySound(sound_t* Sound, int Channel) {
    oslPlaySound(Sound, Channel);
}

void Sound_StopSound(sound_t* Sound) {
    oslStopSound(Sound);
}

void Sound_Delete(sound_t* Sound) {
    oslDeleteSound(Sound);
}