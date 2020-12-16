/*

 Name:  LOAD_MTM.C

 Description:
 MTM module loader

 Portability:
 All systems - all compilers (hopefully)

 If this module is found to not be portable to any particular platform,
 please contact Jake Stine at dracoirs@epix.net (see MIKMOD.TXT for
 more information on contacting the author).

*/

#include <string.h>
#include "mikmod.h"

/**************************************************************************
**************************************************************************/


typedef struct MTMSAMPLE
{   CHAR  samplename[22];
    ULONG length;
    ULONG reppos;
    ULONG repend;
    UBYTE finetune;
    UBYTE volume;
    UBYTE attribute;
} MTMSAMPLE;



typedef struct MTMHEADER
{   UBYTE id[3];                    /* MTM file marker */
    UBYTE version;                  /* upper major, lower nibble minor version number */
    char  songname[20];             /* ASCIIZ songname */
    UWORD numtracks;                /* number of tracks saved */
    UBYTE lastpattern;              /* last pattern number saved */
    UBYTE lastorder;                /* last order number to play (songlength-1) */
    UWORD commentsize;              /* length of comment field */
    UBYTE numsamples;               /* number of samples saved  */
    UBYTE attribute;                /* attribute byte (unused) */
    UBYTE beatspertrack;            /* */
    UBYTE numchannels;              /* number of channels used  */
    UBYTE panpos[32];               /* voice pan positions */
} MTMHEADER;


typedef struct MTMNOTE
{   UBYTE a,b,c;
} MTMNOTE;


/**************************************************************************
**************************************************************************/


static MTMHEADER *mh = NULL;
static MTMNOTE *mtmtrk = NULL;
static UWORD pat[32];

char MTM_Version[] = "MTM";



BOOL MTM_Test(void)
{
    UBYTE id[3];
    if(!_mm_read_UBYTES(id,3,modfp)) return 0;
    if(!memcmp(id,"MTM",3)) return 1;
    return 0;
}


BOOL MTM_Init(void)
{
    if(!(mtmtrk=(MTMNOTE *)_mm_calloc(64,sizeof(MTMNOTE)))) return 0;
    if(!(mh=(MTMHEADER *)_mm_calloc(1,sizeof(MTMHEADER)))) return 0;

    return 1;
}


void MTM_Cleanup(void)
{
    if(mtmtrk!=NULL) free(mtmtrk);
    if(mh!=NULL) free(mh);

    mtmtrk = NULL;
    mh = NULL;
}


UBYTE *MTM_Convert(void)
{
    int t;
    UBYTE a,b,c,inst,note,eff,dat;

    UniReset();
    for(t=0; t<64; t++)
    {   a = mtmtrk[t].a;
        b = mtmtrk[t].b;
        c = mtmtrk[t].c;

        inst = ((a&0x3)<<4)|(b>>4);
        note = a>>2;

        eff = b&0xf;
        dat = c;

        if(inst!=0)  UniInstrument(inst-1);
        if(note!=0)  UniNote(note+24);

        /* mtm bug bugfix: when the effect is volslide, */
        /* slide-up _always_ overrides slide-dn. */

        if(eff==0xa && dat&0xf0) dat&=0xf0;

        /* Convert pattern jump from Dec to Hex */
        if(eff == 0xd)
            dat = (((dat&0xf0)>>4)*10)+(dat&0xf);
        UniPTEffect(eff,dat);
        UniNewline();
    }
    return UniDup();
}


BOOL MTM_Load(void)
{
    MTMSAMPLE s;
    SAMPLE *q;

    int t,u;

    /* try to read module header  */

    _mm_read_UBYTES(mh->id,3,modfp);
    mh->version     =_mm_read_UBYTE(modfp);
    _mm_read_string(mh->songname,20,modfp);
    mh->numtracks   =_mm_read_I_UWORD(modfp);
    mh->lastpattern =_mm_read_UBYTE(modfp);
    mh->lastorder   =_mm_read_UBYTE(modfp);
    mh->commentsize =_mm_read_I_UWORD(modfp);
    mh->numsamples  =_mm_read_UBYTE(modfp);
    mh->attribute   =_mm_read_UBYTE(modfp);
    mh->beatspertrack=_mm_read_UBYTE(modfp);
    mh->numchannels =_mm_read_UBYTE(modfp);
    _mm_read_UBYTES(mh->panpos,32,modfp);

    if(_mm_feof(modfp))
    {   _mm_errno = MMERR_LOADING_HEADER;
        return 0;
    }

    /* set module variables */

    of.initspeed = 6;
    of.inittempo = 125;
    of.modtype   = strdup(MTM_Version);
    of.numchn    = mh->numchannels;
    of.numtrk    = mh->numtracks+1;         /* get number of channels */
    of.songname  = DupStr(mh->songname,20); /* make a cstr of songname */
    of.numpos    = mh->lastorder+1;         /* copy the songlength */
    of.numpat    = mh->lastpattern+1;
    for(t=0; t<32; t++) of.panning[t] = mh->panpos[t] << 4;

    of.numins = of.numsmp = mh->numsamples;
    if(!AllocSamples()) return 0;
    
    q = of.samples;
    
    for(t=0; t<of.numins; t++)
    {   /* try to read sample info */
        _mm_read_string(s.samplename,22,modfp);
        s.length    =_mm_read_I_ULONG(modfp);
        s.reppos    =_mm_read_I_ULONG(modfp);
        s.repend    =_mm_read_I_ULONG(modfp);
        s.finetune  =_mm_read_UBYTE(modfp);
        s.volume    =_mm_read_UBYTE(modfp);
        s.attribute =_mm_read_UBYTE(modfp);

        if(_mm_feof(modfp))
        {   _mm_errno = MMERR_LOADING_SAMPLEINFO; 
            return 0;
        }

        q->samplename = DupStr(s.samplename,22);
        q->seekpos   = 0;
        q->speed     = finetune[s.finetune];
        q->length    = s.length;
        q->loopstart = s.reppos;
        q->loopend   = s.repend;
        q->volume    = s.volume;

        if((s.repend-s.reppos) > 2) q->flags |= SF_LOOP;

        if(s.attribute & 1)
        {   /* If the sample is 16-bits, convert the length */
            /* and replen byte-values into sample-values */

            q->flags|=SF_16BITS;
            q->length>>=1;
            q->loopstart>>=1;
            q->loopend>>=1;
        }

        q++;
    }

    if(!AllocPositions(of.numpos)) return 0;
    for(t=0; t<of.numpos; t++)
        of.positions[t] = _mm_read_UBYTE(modfp);
    for(; t<128; t++)  _mm_read_UBYTE(modfp);

    if(_mm_feof(modfp))
    {   _mm_errno = MMERR_LOADING_HEADER;
        return 0;
    }

    if(!AllocTracks()) return 0;
    if(!AllocPatterns()) return 0;

    of.tracks[0] = MTM_Convert();             /* track 0 is empty */

    for(t=1; t<of.numtrk; t++)
    {   int s;

        for(s=0; s<64; s++)
        {   mtmtrk[s].a=_mm_read_UBYTE(modfp);
            mtmtrk[s].b=_mm_read_UBYTE(modfp);
            mtmtrk[s].c=_mm_read_UBYTE(modfp);
        }

        if(_mm_feof(modfp))
        {   _mm_errno = MMERR_LOADING_TRACK;
            return 0;
        }

        if(!(of.tracks[t]=MTM_Convert())) return 0;
    }

    for(t=0; t<of.numpat; t++)
    {   _mm_read_I_UWORDS(pat,32,modfp);

        for(u=0; u<of.numchn; u++)
            of.patterns[((long)t*of.numchn)+u]=pat[u];
    }

    /* read comment field */

    if(!ReadComment(mh->commentsize)) return 0;

    return 1;
}


CHAR *MTM_LoadTitle(void)
{
    CHAR s[20];

    _mm_fseek(modfp,4,SEEK_SET);
    if(!_mm_fread(s,20,1,modfp)) return NULL;
 
    return(DupStr(s,20));
}
                

MLOADER load_mtm =
{   NULL,
    "MTM",
    "Portable MTM loader v0.1",
    MTM_Init,
    MTM_Test,
    MTM_Load,
    MTM_Cleanup,
    MTM_LoadTitle
};

