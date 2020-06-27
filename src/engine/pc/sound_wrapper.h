// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// Typedefs
typedef sfMusic stream_t;

// Wrapped Functions
extern stream_t* Sound_StreamFromFile(char* Sound);
extern void Sound_SetStreamLoop(stream_t* Stream, bool Loop);
extern void Sound_SetStreamPitch(stream_t* Stream, float Pitch);
extern void Sound_SetStreamVolume(stream_t* Stream, float Volume);
extern void Sound_PlayStream(stream_t* Stream);
extern void Sound_StopStream(stream_t* Stream);