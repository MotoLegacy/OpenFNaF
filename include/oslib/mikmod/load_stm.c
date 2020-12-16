/*

 Name: LOAD_STM.C

 Description:
 ScreamTracker 2 (STM) module Loader - Version 1.oOo Release 2 
 A Coding Nightmare by Rao and Air Richter of HaRDCoDE
 You can now play all of those wonderful old C.C. Catch STM's!

 Portability:
 All systems - all compilers (hopefully)

 If this module is found to not be portable to any particular platform,
 please contact Jake Stine at dracoirs@epix.net (see MIKMOD.TXT for
 more information on contacting the author). 

*/

#include <string.h>
#include <ctype.h>
#include "mikmod.h"


typedef struct STMNOTE
{   UBYTE note,insvol,volcmd,cmdinf;
} STMNOTE;


/* Raw STM sampleinfo struct: */

typedef struct STMSAMPLE
{  CHAR  filename[12];   /* Can't have long comments - just filename comments :) */
   UBYTE unused;         /* 0x00 */
   UBYTE instdisk;       /* Instrument disk */
   UWORD reserved;       /* ISA in memory when in ST 2 */
   UWORD length;         /* Sample length */
   UWORD loopbeg;        /* Loop start point */
   UWORD loopend;        /* Loop end point */
   UBYTE volume;         /* Volume */
   UBYTE reserved2;      /* More reserved crap */
   UWORD c2spd;          /* Good old c2spd */
   UBYTE reserved3[4];   /* Yet more of PSi's reserved crap */
   UWORD isa;            /* Internal Segment Address -> */
                         /*    contrary to the tech specs, this is NOT actually */
                         /*    written to the stm file. */
} STMSAMPLE;

/* Raw STM header struct: */

typedef struct STMHEADER
{  CHAR  songname[20];
   CHAR  trackername[8];   /* !SCREAM! for ST 2.xx  */
   UBYTE unused;           /* 0x1A  */
   UBYTE filetype;         /* 1=song, 2=module (only 2 is supported, of course) :)  */
   UBYTE ver_major;        /* Like 2  */
   UBYTE ver_minor;        /* "ditto"  */
   UBYTE inittempo;        /* initspeed= stm inittempo>>4  */
   UBYTE  numpat;          /* number of patterns  */
   UBYTE   globalvol;      /* <- WoW! a RiGHT TRiANGLE =8*)  */
   UBYTE    reserved[13];  /* More of PSi's internal crap  */
   STMSAMPLE sample[31];   /* STM sample data */
   UBYTE patorder[128];    /* Docs say 64 - actually 128 */
} STMHEADER;


static STMNOTE *stmbuf = NULL;
static STMHEADER *mh = NULL;

static CHAR  STM_Version[] = "Screamtracker 2";


BOOL STM_Test(void)
{
   UBYTE str[9],filetype;

   _mm_fseek(modfp,21,SEEK_SET);
   _mm_read_UBYTES(str,9,modfp);
   filetype = _mm_read_UBYTE(modfp);
   if(!memcmp(str,"!SCREAM!",8) || (filetype!=2)) /* STM Module = filetype 2 */
      return 0;
   return 1;
}



BOOL STM_Init(void)
{
    if(!(mh=(STMHEADER *)_mm_calloc(1,sizeof(STMHEADER)))) return 0;
    if(!(stmbuf=(STMNOTE *)_mm_calloc(64U*of.numchn,sizeof(STMNOTE)))) return 0;

    return 1;
}

void STM_Cleanup(void)
{
    if(mh!=NULL) free(mh);
    if(stmbuf!=NULL) free(stmbuf);

    stmbuf = NULL;
    mh = NULL;
}



void STM_ConvertNote(STMNOTE *n)
{
    UBYTE note,ins,vol,cmd,inf;

    /* extract the various information from the 4 bytes that */
    /*  make up a single note */

    note = n->note;
    ins  = n->insvol>>3;
    vol  = (n->insvol&7)+(n->volcmd>>1);
    cmd  = n->volcmd&15;
    inf  = n->cmdinf;

    if(ins!=0 && ins<32)  UniInstrument(ins-1);

  /* special values of [SBYTE0] are handled here -> */
  /* we have no idea if these strange values will ever be encountered. */
  /* but it appears as though stms sound correct. */
  if(note==254 || note==252) UniPTEffect(0xc,0); /* <- note off command (???) */
     else
  /* if note < 251, then all three bytes are stored in the file */
  if(note<251) UniNote((((note>>4)+2)*12)+(note&0xf));      /* <- normal note and up the octave by two */

    if(vol<65)
        UniPTEffect(0xc,vol);

    if(cmd!=255){
        switch(cmd){

            case 1:                 /* Axx set speed to xx and add 0x1c to fix StoOoPiD STM 2.x */
                UniPTEffect(0xf,inf>>4);
                break;

            case 2:                 /* Bxx position jump */
                UniPTEffect(0xb,inf);
                break;

            case 3:                 /* Cxx patternbreak to row xx */
                UniPTEffect(0xd,(((inf&0xf0)>>4)*10)+(inf&0xf));
                break;

            case 4:                 /* Dxy volumeslide */
                UniWrite(UNI_S3MEFFECTD);
                UniWrite(inf);
                break;

            case 5:                 /* Exy toneslide down */
                UniWrite(UNI_S3MEFFECTE);
                UniWrite(inf);
                break;

            case 6:                 /* Fxy toneslide up */
                UniWrite(UNI_S3MEFFECTF);
                UniWrite(inf);
                break;

            case 7:                 /* Gxx Tone portamento,speed xx */
                UniPTEffect(0x3,inf);
                break;

            case 8:                 /* Hxy vibrato */
                UniPTEffect(0x4,inf);
                break;

            case 9:                 /* Ixy tremor, ontime x, offtime y */
                UniWrite(UNI_S3MEFFECTI);
                UniWrite(inf);
                break;

            case 0xa:               /* Jxy arpeggio */
                UniPTEffect(0x0,inf);
                break;

            case 0xb:               /* Kxy Dual command H00 & Dxy */
                UniPTEffect(0x4,0);
                UniWrite(UNI_S3MEFFECTD);
                UniWrite(inf);
                break;

            case 0xc:               /* Lxy Dual command G00 & Dxy */
                UniPTEffect(0x3,0);
                UniWrite(UNI_S3MEFFECTD);
                UniWrite(inf);
                break;

         /* Support all these above, since ST2 can LOAD these values */
         /* but can actually only play up to J - and J is only */
         /* half-way implemented in ST2 */

            case 0x18:      /* Xxx amiga command 8xx - What the hell, support panning. :) */
                UniPTEffect(0x8,inf);
                break;
        }
    }
}


UBYTE *STM_ConvertTrack(STMNOTE *n)
{
    int t;

    UniReset();
    for(t=0; t<64; t++)
    {   STM_ConvertNote(n);
        UniNewline();
        n+=of.numchn;
    }
    return UniDup();
}


BOOL STM_LoadPatterns(void)
{
    int t,s,tracks=0;

    if(!AllocPatterns()) return 0;
    if(!AllocTracks()) return 0;

    /* Allocate temporary buffer for loading */
    /* and converting the patterns */

    for(t=0; t<of.numpat; t++)
    {   for(s=0; s<(64U*of.numchn); s++)
        {   stmbuf[s].note   = _mm_read_UBYTE(modfp);
            stmbuf[s].insvol = _mm_read_UBYTE(modfp);
            stmbuf[s].volcmd = _mm_read_UBYTE(modfp);
            stmbuf[s].cmdinf = _mm_read_UBYTE(modfp);
        }

        if(_mm_feof(modfp))
        {   _mm_errno = MMERR_LOADING_PATTERN;
            return 0;
        }

        for(s=0;s<of.numchn;s++)
        {   if(!(of.tracks[tracks++]=STM_ConvertTrack(stmbuf+s))) return 0;
        }
    }

    return 1;
}



BOOL STM_Load(void)
{
    int        t; 
    ULONG      MikMod_ISA; /* We MUST generate our own ISA - NOT stored in the stm */
    SAMPLE     *q;

    /* try to read stm header */

    _mm_read_string(mh->songname,20,modfp);
    _mm_read_string(mh->trackername,8,modfp);
    mh->unused      =_mm_read_UBYTE(modfp);
    mh->filetype    =_mm_read_UBYTE(modfp);
    mh->ver_major   =_mm_read_UBYTE(modfp);
    mh->ver_minor   =_mm_read_UBYTE(modfp);
    mh->inittempo   =_mm_read_UBYTE(modfp);
    mh->numpat      =_mm_read_UBYTE(modfp);
    mh->globalvol   =_mm_read_UBYTE(modfp);
    _mm_read_UBYTES(mh->reserved,13,modfp);

    for(t=0;t<31;t++)
    {   STMSAMPLE *s = &mh->sample[t];  /* STM sample data */

        _mm_read_string(s->filename,12,modfp);
        s->unused   =_mm_read_UBYTE(modfp);
        s->instdisk =_mm_read_UBYTE(modfp);
        s->reserved =_mm_read_I_UWORD(modfp);
        s->length   =_mm_read_I_UWORD(modfp);
        s->loopbeg  =_mm_read_I_UWORD(modfp);
        s->loopend  =_mm_read_I_UWORD(modfp);
        s->volume   =_mm_read_UBYTE(modfp);
        s->reserved2=_mm_read_UBYTE(modfp);
        s->c2spd    =_mm_read_I_UWORD(modfp);
        _mm_read_UBYTES(s->reserved3,4,modfp);
        s->isa      =_mm_read_I_UWORD(modfp);
    }
    _mm_read_UBYTES(mh->patorder,128,modfp);

    if(_mm_feof(modfp))
    {   _mm_errno = MMERR_LOADING_HEADER;
        return 0;
    }

    /* set module variables */

    of.modtype   = strdup(STM_Version);
    of.songname  = DupStr(mh->songname,20); /* make a cstr of songname */
    of.numpat    = mh->numpat;
    of.inittempo = 125;                     /* mh->inittempo+0x1c; */
    of.initspeed = mh->inittempo>>4;
    of.numchn    = 4;                       /* get number of channels */

    t=0;
    if(!AllocPositions(0x80)) return 0;
    while(mh->patorder[t]!=99)    /* 99 terminates the patorder list */
    {   of.positions[t] = mh->patorder[t];
        t++;
    }
    of.numpos = --t;
    of.numtrk = of.numpat*of.numchn;

    /* Finally, init the sampleinfo structures */
    of.numins = of.numsmp = 31;    /* always this */

    if(!AllocSamples()) return 0;
    if(!STM_LoadPatterns()) return 0;

    q = of.samples;

    MikMod_ISA = _mm_ftell(modfp);
    MikMod_ISA = (MikMod_ISA+15)&0xfffffff0;

    for(t=0; t<of.numins; t++)
    {   /* load sample info */

        q->samplename = DupStr(mh->sample[t].filename,12);
        q->speed      = mh->sample[t].c2spd;
        q->volume     = mh->sample[t].volume;
        q->length     = mh->sample[t].length;
        if (!mh->sample[t].volume || q->length==1 ) q->length = 0; /* if vol = 0 or length = 1, then no sample */
        q->loopstart  = mh->sample[t].loopbeg;
        q->loopend    = mh->sample[t].loopend;
        q->seekpos    = MikMod_ISA;

        MikMod_ISA+=q->length;
        MikMod_ISA = (MikMod_ISA+15)&0xfffffff0;

        /* Once again, contrary to the STM specs, all the sample data is */
        /* actually SIGNED! Sheesh */

        q->flags = SF_SIGNED;

        if(mh->sample[t].loopend>0 && mh->sample[t].loopend!=0xffff) q->flags|=SF_LOOP;

        /* fix replen if repend>length */

        if(q->loopend > q->length) q->loopend = q->length;

        q++;
    }

    return 1;
}


CHAR *STM_LoadTitle(void)
{
    CHAR s[20];

    _mm_fseek(modfp,0,SEEK_SET);
    if(!_mm_fread(s,20,1,modfp)) return NULL;

    return(DupStr(s,20));
}


MLOADER load_stm =
{   NULL,
    "STM",
    "Portable STM Loader - V 1.2",
    STM_Init,
    STM_Test,
    STM_Load,
    STM_Cleanup,
    STM_LoadTitle
};

