
#include "mikmod.h"

#if (defined(__GNUC__) || defined(SGI))
#include <sys/types.h>
#else
#include <io.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#define WAVBUFFERSIZE 65536

static VIRTUAL_FILE *wavout;

static SBYTE *WAV_DMABUF;
static ULONG dumpsize;

static BOOL WAV_IsThere(void)
{
    return 1;
}


static BOOL WAV_Init(void)
{
    if(NULL == (wavout = _mm_fopen("music.wav", "wb"))) return 1;
    if(NULL == (WAV_DMABUF = _mm_malloc(WAVBUFFERSIZE))) return 1;

    md_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;

    if(VC_Init()) return 1;
    
    _mm_write_string("RIFF    WAVEfmt ",wavout);
    _mm_write_I_ULONG(16,wavout);     /* length of this RIFF block crap */

    _mm_write_I_UWORD(1, wavout);    /* microsoft format type */
    _mm_write_I_UWORD((md_mode & DMODE_STEREO) ? 2 : 1, wavout);
    _mm_write_I_ULONG(md_mixfreq, wavout);
    _mm_write_I_ULONG(md_mixfreq * ((md_mode & DMODE_STEREO) ? 2 : 1) *
                      ((md_mode & DMODE_16BITS) ? 2 : 1), wavout);

    _mm_write_I_UWORD(((md_mode & DMODE_16BITS) ? 2 : 1) * 
                      ((md_mode & DMODE_STEREO) ? 2 : 1), wavout);    /* block alignment (8/16 bit) */

    _mm_write_I_UWORD((md_mode & DMODE_16BITS) ? 16 : 8,wavout);

    _mm_write_string("data",wavout);

    dumpsize = 0;

    return 0;
}


static void WAV_Exit(void)
{
    VC_Exit();

    /* write in the actual sizes now */

    if(wavout!=NULL)
    {   _mm_fseek(wavout,4,SEEK_SET);
        _mm_write_I_ULONG(dumpsize + 32, wavout);
        _mm_fseek(wavout,40,SEEK_SET);
        _mm_write_I_ULONG(dumpsize, wavout);

        _mm_fclose(wavout);

        if(WAV_DMABUF != NULL) free(WAV_DMABUF);
    }
}


static void WAV_Update(void)
{
    VC_WriteBytes(WAV_DMABUF, WAVBUFFERSIZE);
    VirtualFileWrite(WAV_DMABUF, 1, WAVBUFFERSIZE, wavout);
    dumpsize += WAVBUFFERSIZE;
}


static BOOL WAV_Reset(void)
{
    return 0;
}


MDRIVER drv_wav =
{   NULL,
    "music.wav file",
    "WAV [music.wav] file output driver v1.0",
    0,255,
    WAV_IsThere,
    VC_SampleLoad,
    VC_SampleUnload,
    VC_SampleSpace,
    VC_SampleLength,
    WAV_Init,
    WAV_Exit,
    WAV_Reset,
    VC_SetNumVoices,
    VC_PlayStart,
    VC_PlayStop,
    WAV_Update,
    VC_VoiceSetVolume,
    VC_VoiceSetFrequency,
    VC_VoiceSetPanning,
    VC_VoicePlay,
    VC_VoiceStop,
    VC_VoiceStopped,
    VC_VoiceReleaseSustain,
    VC_VoiceGetPosition,
    NULL
};

