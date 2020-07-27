// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// Typedefs
typedef sfMusic         stream_t;
typedef sfSound         sound_t;
typedef sfSoundBuffer   sndbuffer_t;

// Wrapped Functions
extern stream_t* Sound_StreamFromFile(char* Sound);
extern void Sound_SetStreamLoop(stream_t* Stream, bool Loop);
extern void Sound_SetStreamPitch(stream_t* Stream, float Pitch);
extern void Sound_SetStreamVolume(stream_t* Stream, float Volume);
extern void Sound_PlayStream(stream_t* Stream);
extern void Sound_StopStream(stream_t* Stream);
extern sndbuffer_t* Sound_LoadSound(char* Directory);
extern sound_t* Sound_Create();
extern void Sound_BindSoundToBuffer(sound_t* Sound, sndbuffer_t* SoundBuffer);
extern void Sound_SetSoundLoop(sound_t* Sound, bool Loop);
extern void Sound_SetSoundPitch(sound_t* Sound, float Pitch);
extern void Sound_SetSoundVolume(sound_t* Sound, float Volume);
extern void Sound_PlaySound(sound_t* Sound);
extern void Sound_StopSound(sound_t* Sound);