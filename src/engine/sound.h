// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// Max Number of Sounds that can be played at once (8)
#define MAX_SOUND_CHANNELS      7


typedef struct {
    stream_t* Stream;
} streamelement_t;

extern void Sound_Stream(int Channel, char* Sound, bool Loop, float Pitch, float Volume);
extern void Sound_Clean();