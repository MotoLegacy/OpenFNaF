/*

 Name: LOAD_M15.C

 Description:
 15 instrument MOD loader
 Also supports Ultimate Sound Tracker (old M15 format)

 Portability:
 All systems - all compilers (hopefully)

 If this module is found to not be portable to any particular platform,
 please contact Jake Stine at dracoirs@epix.net (see MIKMOD.TXT for
 more information on contacting the author).

*/

#include <string.h>
#include "mikmod.h"

/*************************************************************************
*************************************************************************/


typedef struct MSAMPINFO       /* sample header as it appears in a module */
{   CHAR  samplename[22];
    UWORD length;
    UBYTE finetune;
    UBYTE volume;
    UWORD reppos;
    UWORD replen;
} MSAMPINFO;


typedef struct MODULEHEADER          /* verbatim module header */
{   CHAR       songname[20];         /* the songname.. */
    MSAMPINFO  samples[15];          /* all sampleinfo */
    UBYTE      songlength;           /* number of patterns used */
    UBYTE      magic1;               /* should be 127 */
    UBYTE      positions[128];       /* which pattern to play at pos */
} MODULEHEADER;


typedef struct MODNOTE
{   UBYTE a,b,c,d;
} MODNOTE;


/*************************************************************************
*************************************************************************/

static MODULEHEADER *mh = NULL;       /* raw as-is module header */
static MODNOTE *patbuf = NULL;
static BOOL ust_loader = 0;          /* if TRUE, load as a ust module. */
static CHAR nulls[3] = {0,0,0};

static BOOL LoadModuleHeader(MODULEHEADER *mh)
{
    int t;

    _mm_read_string(mh->songname,20,modfp);

    for(t=0; t<15; t++)
    {   MSAMPINFO *s = &mh->samples[t];
        _mm_read_string(s->samplename,22,modfp);
        s->length   =_mm_read_M_UWORD(modfp);
        s->finetune =_mm_read_UBYTE(modfp);
        s->volume   =_mm_read_UBYTE(modfp);
        s->reppos   =_mm_read_M_UWORD(modfp);
        s->replen   =_mm_read_M_UWORD(modfp);
    }

    mh->songlength  =_mm_read_UBYTE(modfp);
    mh->magic1      =_mm_read_UBYTE(modfp);               /* should be 127 */
    _mm_read_UBYTES(mh->positions,128,modfp);

    return(!_mm_feof(modfp));
}


static int CheckPatternType(int numpat)

/* Checks the patterns in the modfile for UST / 15-inst indications. */
/* For example, if an effect 3xx is found, it is assumed that the song */
/* is 15-inst.  If a 1xx effect has dat greater than 0x20, it is UST. */
/*  Returns:  0 indecisive; 1 = UST; 2 = 15-inst */

{
    int   t;
    UBYTE eff, dat;

    ust_loader = 1;

    for(t=0; t<numpat*(64U*4); t++)
    {   /* Load the pattern into the temp buffer */
        /* and convert it */

        _mm_read_UBYTE(modfp);      /* read note */
        _mm_read_UBYTE(modfp);      /* read inst */
        eff = _mm_read_UBYTE(modfp);
        dat = _mm_read_UBYTE(modfp);
        
        if((eff==3) && (dat!=0) || (eff >= 2))   return 2;
        if(eff==1)
        {   if(dat > 0x1f) return 1;
            if(dat < 0x3)  return 2;
        }
        if((eff==2) && (dat > 0x1f)) return 1;
    }

    return 0;
}


BOOL M15_Test(void)
{
    int          t, numpat;
    MODULEHEADER mh;

    ust_loader = 0;

    if(!LoadModuleHeader(&mh)) return 0;
    if(mh.magic1>127) return 0;

    for(t=0; t<15; t++)
    {   /* all finetunes should be zero */
        if(mh.samples[t].finetune != 0) return 0;

        /* all volumes should be <= 64 */
        if(mh.samples[t].volume > 64) return 0;

        /* all instrument names should begin with s, st-, or a number */
        if(mh.samples[t].samplename[0] == 's')
        {   if((memcmp(mh.samples[t].samplename,"st-",3) != 0) &&
               (memcmp(mh.samples[t].samplename,"ST-",3) != 0) &&
               (memcmp(mh.samples[t].samplename,nulls,3) != 0))
                ust_loader = 1;
        } else if((mh.samples[t].samplename[0] < '0') || (mh.samples[t].samplename[0] > '9'))
            ust_loader = 1;

        if(mh.samples[t].length > 4999)
        {   ust_loader = 0;
            if(mh.samples[t].length > 32768) return 0;
        }

        if(!ust_loader) return 1;

        if(((mh.samples[t].reppos) + mh.samples[t].replen) > (mh.samples[t].length + 10))
        {   ust_loader = 1;
            return 1;
        }
        
    }

    for(numpat=0, t=0; t<mh.songlength; t++)
    {   if(mh.positions[t] > numpat)
            numpat = mh.positions[t];
    }

    numpat++;
    switch(CheckPatternType(numpat))
    {   case 0:   /* indecisive, so check more clues... */

        break;

        case 1:  ust_loader = 1;   break;
        case 2:  ust_loader = 0;   break;
    }
 
    return 1;
}


BOOL M15_Init(void)
{
    if(!(mh=(MODULEHEADER *)_mm_calloc(1,sizeof(MODULEHEADER)))) return 0;
    return 1;
}


void M15_Cleanup(void)
{
    if(mh!=NULL) free(mh);
    if(patbuf!=NULL) free(patbuf);

    mh = NULL;
    patbuf = NULL;
}


/*
Old (amiga) noteinfo:

 _____byte 1_____   byte2_    _____byte 3_____   byte4_
/                \ /      \  /                \ /      \
0000          0000-00000000  0000          0000-00000000

Upper four    12 bits for    Lower four    Effect command.
bits of sam-  note period.   bits of sam-
ple number.                  ple number.

*/


static void M15_ConvertNote(MODNOTE *n)
{
    UBYTE instrument,effect,effdat,note;
    UWORD period;

    /* extract the various information from the 4 bytes that */
    /* make up a single note */

    instrument = (n->a&0x10)|(n->c>>4);
    period     = (((UWORD)n->a&0xf)<<8)+n->b;
    effect     = n->c&0xf;
    effdat     = n->d;

    /* Convert the period to a note number */

    note=0;
    if(period != 0)
    {   for(note=0; note<60; note++)
            if(period >= npertab[note]) break;
        note++;
        if(note==61) note = 0;
    }

    if(instrument!=0) UniInstrument(instrument-1);
    if(note!=0) UniNote(note+23);

    /* Convert pattern jump from Dec to Hex */
    if(effect == 0xd)
        effdat = (((effdat&0xf0)>>4)*10)+(effdat&0xf);

    if(ust_loader)
    {   switch(effect)
        {   case 0:  break;
            case 1:
                UniPTEffect(0,effdat);
            break;

            case 2:  
                if(effdat&0xf) UniPTEffect(1,effdat&0xf);
                if(effdat>>2)  UniPTEffect(2,effdat>>2);
            break;

            case 3:  break;

            default:
                UniPTEffect(effect,effdat);
            break;
        }
    } else UniPTEffect(effect,effdat);
}


static UBYTE *M15_ConvertTrack(MODNOTE *n)
{
    int t;

    UniReset();
    for(t=0; t<64; t++)
    {   M15_ConvertNote(n);
        UniNewline();
        n += 4;
    }
    return UniDup();
}



static BOOL M15_LoadPatterns(void)
/*  Loads all patterns of a modfile and converts them into the */
/*  3 byte format. */
{
    int t,s,tracks=0;

    if(!AllocPatterns()) return 0;
    if(!AllocTracks()) return 0;

    /* Allocate temporary buffer for loading */
    /* and converting the patterns */

    if(!(patbuf=(MODNOTE *)_mm_calloc(64U*4,sizeof(MODNOTE)))) return 0;

    for(t=0; t<of.numpat; t++)
    {   /* Load the pattern into the temp buffer */
        /* and convert it */

        for(s=0; s<(64U*4); s++)
        {   patbuf[s].a=_mm_read_UBYTE(modfp);
            patbuf[s].b=_mm_read_UBYTE(modfp);
            patbuf[s].c=_mm_read_UBYTE(modfp);
            patbuf[s].d=_mm_read_UBYTE(modfp);
        }

        for(s=0; s<4; s++)
            if(!(of.tracks[tracks++]=M15_ConvertTrack(patbuf+s))) return 0;
    }

    return 1;
}


BOOL M15_Load(void)
{
    int        t;
    SAMPLE     *q;
    MSAMPINFO  *s;           /* old module sampleinfo */

    /* try to read module header */

    if(!LoadModuleHeader(mh))
    {   _mm_errno = MMERR_LOADING_HEADER;
        return 0;
    }


    if(ust_loader)
        of.modtype = strdup("Ultimate Soundtracker");
    else
        of.modtype = strdup("Soundtracker");

    /* set module variables */

    of.initspeed = 6;
    of.inittempo = 125;
    of.numchn    = 4;                           /* get number of channels */
    of.songname  = DupStr(mh->songname,20);     /* make a cstr of songname */
    of.numpos    = mh->songlength;              /* copy the songlength */

    if(!AllocPositions(of.numpos)) return 0;
    for(t=0; t<of.numpos; t++)
        of.positions[t] = mh->positions[t];


    /* Count the number of patterns */

    of.numpat = 0;

    for(t=0; t<of.numpos; t++)
    {   if(of.positions[t] > of.numpat)
            of.numpat = of.positions[t];
    }
    of.numpat++;
    of.numtrk = of.numpat*4;

    /* Finally, init the sampleinfo structures */

    of.numins = of.numsmp = 15;
    if(!AllocSamples()) return 0;

    s = mh->samples;          /* init source pointer */
    q = of.samples;
    
    for(t=0; t<of.numins; t++)
    {   /* convert the samplename */
        q->samplename = DupStr(s->samplename,22);

        /* init the sampleinfo variables and */
        /* convert the size pointers to longword format */

        q->speed     = finetune[s->finetune&0xf];
        q->volume    = s->volume;
        if(ust_loader)
            q->loopstart = s->reppos;
        else
            q->loopstart = s->reppos<<1;
        q->loopend   = q->loopstart+(s->replen<<1);
        q->length    = s->length<<1;

        q->flags = SF_SIGNED | SF_UST_LOOP;
        if(s->replen>1) q->flags |= SF_LOOP;

        /* fix replen if repend>length */

        if(q->loopend>q->length) q->loopend = q->length;

        s++;    /* point to next source sampleinfo */
        q++;
    }

    if(!M15_LoadPatterns()) return 0;

    ust_loader = 0;
    return 1;
}


CHAR *M15_LoadTitle(void)
{
   CHAR s[20];

   _mm_fseek(modfp,0,SEEK_SET);
   if(!_mm_fread(s,22,1,modfp)) return NULL;

   return(DupStr(s,20));
}


MLOADER load_m15 =
{   NULL,
    "15-instrument module",
    "Portable MOD-15 loader v0.1",
    M15_Init,
    M15_Test,
    M15_Load,
    M15_Cleanup,
    M15_LoadTitle
};
