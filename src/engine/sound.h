// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// Max Number of Sounds that can be played at once (8)
#define MAX_SOUND_CHANNELS      7

// Max Number of Sounds that can be Precached
#define MAX_PRECACHED_SOUNDS    32


typedef struct {
    stream_t* Stream;
} streamelement_t;

typedef struct {
    sound_t* Sound;
    sndbuffer_t* SoundBuffer;
} soundelement_t;

extern void Sound_Stream(int Channel, char* Sound, bool Loop, float Pitch, float Volume);
extern void Sound_Clean();
extern void Sound_Precache(char* Directory);