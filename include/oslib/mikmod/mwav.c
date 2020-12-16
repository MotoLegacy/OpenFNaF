/*

 Name:  MWAV.C

 Description:
 WAV sample loader
 Stereo .WAV files are not yet supported as samples.

 Portability:
 All compilers -- All systems (hopefully)

*/

#include <string.h>
#include "mikmod.h"


typedef struct WAV
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
    UWORD cbSize;
} WAV;


SAMPLE *WAV_LoadFP(VIRTUAL_FILE *fp)
{
    SAMPLE     *si;
    static WAV  wh;
    static CHAR dID[4];

    /* read wav header */

    _mm_read_string(wh.rID,4,fp);
    wh.rLen = _mm_read_I_ULONG(fp);
    _mm_read_string(wh.wID,4,fp);

    while(1)
    {   _mm_read_string(wh.fID,4,fp);
        wh.fLen = _mm_read_I_ULONG(fp);
        if(memcmp(wh.fID,"fmt ",4) == 0) break;
        _mm_fseek(fp,wh.fLen,SEEK_CUR);
    }

    if( _mm_feof(fp) ||
        memcmp(wh.rID,"RIFF",4) ||
        memcmp(wh.wID,"WAVE",4))
    {
        _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return NULL;
    }

    wh.wFormatTag      = _mm_read_I_UWORD(fp);
    wh.nChannels       = _mm_read_I_UWORD(fp);
    wh.nSamplesPerSec  = _mm_read_I_ULONG(fp);
    wh.nAvgBytesPerSec = _mm_read_I_ULONG(fp);
    wh.nBlockAlign     = _mm_read_I_UWORD(fp);
    wh.nFormatSpecific = _mm_read_I_UWORD(fp);

    /* check it */

    if(_mm_feof(fp))
    {   _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return NULL;
    }

    /* skip other crap */

    _mm_fseek(fp,wh.fLen-16,SEEK_CUR);
    _mm_read_string(dID,4,fp);

    if(memcmp(dID,"data",4))
    {   _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return NULL;
    }

    if(wh.nChannels > 1)
    {   _mm_errno = MMERR_UNKNOWN_WAVE_TYPE;
        return NULL;
    }

/*  printf("wFormatTag: %x\n",wh.wFormatTag); */
/*  printf("blockalign: %x\n",wh.nBlockAlign); */
/*  prinff("nFormatSpc: %x\n",wh.nFormatSpecific); */

    if((si=(SAMPLE *)_mm_calloc(1,sizeof(SAMPLE)))==NULL) return NULL;

    si->speed  = wh.nSamplesPerSec;
    si->volume = 64;
    si->length = _mm_read_I_ULONG(fp);

    if(wh.nBlockAlign == 2)
    {   si->flags    = SF_16BITS | SF_SIGNED;
        si->length >>= 1;
    }

    SL_RegisterSample(si,MD_SNDFX,fp);

    return si;
}


SAMPLE *WAV_LoadFN(CHAR *filename)
{
    VIRTUAL_FILE   *fp;
    SAMPLE *si;

    if(!(md_mode & DMODE_SOFT_SNDFX)) return NULL;
    if((fp=_mm_fopen(filename,"rb"))==NULL) return NULL;

    si = WAV_LoadFP(fp);
    SL_LoadSamples();
    _mm_fclose(fp);

    return si;
}


void WAV_Free(SAMPLE *si)
{
    if(si!=NULL)
    {   MD_SampleUnLoad(si->handle);
        free(si);
    }
}

