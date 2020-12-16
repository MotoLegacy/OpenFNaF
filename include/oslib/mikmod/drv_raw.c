/*

Name:
DRV_RAW.C

Description:
Mikmod driver for output to a file called MUSIC.RAW

MS-DOS Programmers:
 !! DO NOT CALL MD_UPDATE FROM A INTERRUPT IF YOU USE THIS DRIVER !!

Portability:

MSDOS:  BC(y)   Watcom(y)   DJGPP(y)
Win95:  BC(y)
Linux:  y
SGI:    CC(y)   GCC(y)

(y) - yes
(n) - no (not possible or not useful)
(?) - may be possible, but not tested

*/

#include "mikmod.h"

#if (defined(__GNUC__) || defined(SGI))
#include <sys/types.h>
#else
#include <io.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#define RAWBUFFERSIZE 8192

static int rawout;

static SBYTE RAW_DMABUF[RAWBUFFERSIZE];


static BOOL RAW_IsThere(void)
{
    return 1;
}


static BOOL RAW_Init(void)
{
    if(-1 == (rawout = open("music.raw", 
#if (!defined(__GNUC__) && (!defined(SGI)))
                O_BINARY | 
#endif
                O_RDWR | O_TRUNC | O_CREAT, S_IREAD | S_IWRITE)))
        return 1;

    md_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;

    if(VC_Init()) return 1;

    return 0;
}


static void RAW_Exit(void)
{
    VC_Exit();
    close(rawout);
}


static void RAW_Update(void)
{
    VC_WriteBytes(RAW_DMABUF, RAWBUFFERSIZE);
    write(rawout, RAW_DMABUF, RAWBUFFERSIZE);
}


static BOOL RAW_Reset(void)
{
    return 0;
}


MDRIVER drv_raw =
{   NULL,
    "music.raw file",
    "RAW [music.raw] file output driver v1.0",
    0,255,
    RAW_IsThere,
    VC_SampleLoad,
    VC_SampleUnload,
    VC_SampleSpace,
    VC_SampleLength,
    RAW_Init,
    RAW_Exit,
    RAW_Reset,
    VC_SetNumVoices,
    VC_PlayStart,
    VC_PlayStop,
    RAW_Update,
    VC_VoiceSetVolume,
    VC_VoiceSetFrequency,
    VC_VoiceSetPanning,
    VC_VoicePlay,
    VC_VoiceStop,
    VC_VoiceStopped,
    VC_VoiceReleaseSustain,
    VC_VoiceGetPosition,
    VC_VoiceRealVolume
};

