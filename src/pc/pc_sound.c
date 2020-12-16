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

stream_t* Sound_StreamFromFile(char* File)
{
    return sfMusic_createFromFile(File);
}

void Sound_SetStreamLoop(stream_t* Stream, boolean Loop)
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

void Sound_PlayStream(stream_t* Stream, int Channel)
{
    sfMusic_play(Stream);
}

void Sound_StopStream(stream_t* Stream)
{
    sfMusic_stop(Stream);
    sfMusic_destroy(Stream);
}