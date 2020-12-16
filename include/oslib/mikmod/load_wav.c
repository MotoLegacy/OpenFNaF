/*

 Name:  LOAD_WAV.C

 Description:
 WAV Streaming Audio Loader / Player

 Portability:
 All compilers -- All systems (hopefully)

*/

#include "mikmod.h"
#include <string.h>


typedef struct
{   CHAR  rID[4];
    ULONG rLen;
    CHAR  wID[4];
    CHAR  fID[4];
    ULONG fLen;
    UWORD wFormatTag;
    UWORD nChannels;
    ULONG nSamplesPerSec;
    ULONG nAvgBytesPerSec;
    UWORD nBlockAlign;
    UWORD nFormatSpecific;
} WAV;


BOOL WAV_Load(void)
{
    SAMPLE     *si;
    static WAV  wh;
    static CHAR dID[4];

    /* read wav header */

    _mm_read_string(wh.rID,4,stream_fp);
    wh.rLen            = _mm_read_I_ULONG(stream_fp);
    _mm_read_string(wh.wID,4,stream_fp);
    _mm_read_string(wh.fID,4,stream_fp);
    wh.fLen            = _mm_read_I_ULONG(stream_fp);
    wh.wFormatTag      = _mm_read_I_UWORD(stream_fp);
    wh.nChannels       = _mm_read_I_UWORD(stream_fp);
    wh.nSamplesPerSec  = _mm_read_I_ULONG(stream_fp);
    wh.nAvgBytesPerSec = _mm_read_I_ULONG(stream_fp);
    wh.nBlockAlign     = _mm_read_I_UWORD(stream_fp);
    wh.nFormatSpecific = _mm_read_I_UWORD(stream_fp);

    /* check it */

    if( _mm_feof(stream_fp) ||
        memcmp(wh.rID,"RIFF",4) ||
        memcmp(wh.wID,"WAVE",4) ||
        memcmp(wh.fID,"fmt ",4) )
    {
        _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return 0;
    }

    /* skip other crap */

    _mm_fseek(stream_fp,wh.fLen-16,SEEK_CUR);
    _mm_read_string(dID,4,stream_fp);

    if( memcmp(dID,"data",4) )
    {   _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return 0;
    }

    if(wh.nChannels > 1)
    {   _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return 0;
    }

/*  printf("wFormatTag: %x\n",wh.wFormatTag); */
/*  printf("blockalign: %x\n",wh.nBlockAlign); */
/*  prinff("nFormatSpc: %x\n",wh.nFormatSpecific); */

    if((si=(SAMPLE *)_mm_calloc(1,sizeof(SAMPLE)))==NULL) return 0;

    si->speed  = wh.nSamplesPerSec/wh.nChannels;
    si->volume = 64;

    si->length = _mm_read_I_ULONG(stream_fp);

    if(wh.nBlockAlign==2)
    {   si->flags = SF_16BITS | SF_SIGNED;
        si->length>>=1;
    }

    return 0;
}


