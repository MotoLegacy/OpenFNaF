// Max Number of Sounds that can be played at once (8)
#define MAX_SOUND_CHANNELS      7

// Max Number of Sounds that can be Precached
#define MAX_PRECACHED_SOUNDS    32

#ifdef PSP
typedef OSL_SOUND stream_t;
#endif

#ifdef DESKTOP
typedef sfMusic   stream_t;
#endif

typedef struct {
    stream_t* Stream;
} streamelement_t;

#ifndef DESKTOP
extern void Sound_Initialize();
#endif
extern void Sound_Stream(int Channel, char* Sound, boolean Loop, float Pitch, float Volume);
extern void Sound_StopStream(stream_t* Stream);
extern void Sound_PlayStream(stream_t* Stream, int Channel);
extern stream_t* Sound_StreamFromFile(char* File);

#ifdef DESKTOP
extern void Sound_SetStreamLoop(stream_t* Stream, boolean Loop);
extern void Sound_SetStreamPitch(stream_t* Stream, float Pitch);
extern void Sound_SetStreamVolume(stream_t* Stream, float Volume);
#endif