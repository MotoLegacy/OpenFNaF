/*

 Name: LOAD_669.C

 Description:
 Tran's 669 module loader

 Portability:
 All systems - all compilers (hopefully)

 If this module is found to not be portable to any particular platform,
 please contact Jake Stine at dracoirs@epix.net (see MIKMOD.TXT for
 more information on contacting the author).

*/

#include <string.h>
#include "mikmod.h"


/* Raw 669 header struct: */

typedef struct S69HEADER
{   UBYTE  marker[2];
    CHAR   message[108];
    UBYTE  nos;
    UBYTE  nop;
    UBYTE  looporder;
    UBYTE  orders[0x80];
    UBYTE  tempos[0x80];
    UBYTE  breaks[0x80];
} S69HEADER;


/* Raw 669 sampleinfo struct: */

typedef struct S69SAMPLE
{   CHAR   filename[13];
    SLONG  length;
    SLONG  loopbeg;
    SLONG  loopend;
} S69SAMPLE;


/* Raw 669 Note struct */

typedef struct S69NOTE
{   UBYTE a,b,c;
} S69NOTE;


static S69NOTE *s69pat = NULL;
static S69HEADER *mh   = NULL;

static CHAR *S69_Version[] =
{   "669",
    "Extended 669"
};


BOOL S69_Test(void)
{
    UBYTE id[2];

    if(!_mm_read_UBYTES(id,2,modfp)) return 0;
    if(!memcmp(id,"if",2) || !memcmp(id,"JN",2))
    {   _mm_fseek(modfp,108,SEEK_CUR);
        if(_mm_read_UBYTE(modfp) > 64) return 0;
        if(_mm_read_UBYTE(modfp) > 128) return 0;
        if(_mm_read_UBYTE(modfp) > 120) return 0;
        return 1;
    }
    return 0;
}


BOOL S69_Init(void)
{
    if(!(s69pat=(S69NOTE *)_mm_malloc(64*8*sizeof(S69NOTE)))) return 0;
    if(!(mh=(S69HEADER *)_mm_calloc(1,sizeof(S69HEADER)))) return 0;
    return 1;
}


void S69_Cleanup(void)
{
    if(s69pat!=NULL) free(s69pat);
    if(mh!=NULL) free(mh);

    mh     = NULL;
    s69pat = NULL;
}


BOOL S69_LoadPatterns(void)
{
    int     t,s,q,tracks=0,t2,t3;
    UBYTE   note,inst,vol,a,b,c, lo;
    S69NOTE *cur;

    if(!AllocPatterns()) return 0;
    if(!AllocTracks())   return 0;

    for(t=0; t<of.numpat; t++)
    {   of.pattrows[t] = mh->breaks[t]+1;

        /* Load the pattern into the temp buffer */
        /* and convert it into the 3-byte format */

        cur = s69pat;
        for(t2=64; t2; t2--)
        {   for(t3=8; t3; t3--, cur++)
            {   cur->a = _mm_read_UBYTE(modfp);
                cur->b = _mm_read_UBYTE(modfp);
                cur->c = _mm_read_UBYTE(modfp);
            }
        }

        if(_mm_feof(modfp))
        {   _mm_errno = MMERR_LOADING_PATTERN;
            return 0;
        }

        for(s=0; s<8; s++)
        {   UniReset();
            UniPTEffect(0xf,75);   /* was 78 */
            UniPTEffect(0xf,3);

            for(q=0; q<64; q++)
            {   a = s69pat[(q*8)+s].a;
                b = s69pat[(q*8)+s].b;
                c = s69pat[(q*8)+s].c;

                note = a >> 2;
                inst = ((a & 0x3) << 4) | ((b & 0xf0) >> 4);
                vol  = b & 0xf;

                if(note < 0x3e)
                {   UniInstrument(inst);
                    UniNote(note+24);
                }

                if(note < 0x3f) UniPTEffect(0xc,vol<<2);

                lo = c & 0xf;
                switch(c >> 4)
                {   case 0:
                        UniPTEffect(0x1,lo);
                    break;

                    case 1:
                        UniPTEffect(0x2,lo);
                    break;
                   
                    case 2:
                        UniPTEffect(0x3,lo);
                    break;

                    case 4:
                        UniPTEffect(0x4,lo);
                    break;
                }
                UniNewline();
            }
            if(!(of.tracks[tracks++]=UniDup())) return 0;
        }
    }
    return 1;
}


BOOL S69_Load(void)
{
    int         t;
    S69SAMPLE   s;
    SAMPLE     *q;

    /* try to read module header        */

    _mm_read_UBYTES(mh->marker,2,modfp);
    _mm_read_UBYTES((UBYTE *)mh->message,108,modfp);
    mh->nos = _mm_read_UBYTE(modfp);
    mh->nop = _mm_read_UBYTE(modfp);
    mh->looporder = _mm_read_UBYTE(modfp);
    _mm_read_UBYTES(mh->orders,0x80,modfp);
    _mm_read_UBYTES(mh->tempos,0x80,modfp);
    _mm_read_UBYTES(mh->breaks,0x80,modfp);

    /* set module variables */

    of.initspeed = 6;
    of.inittempo = 125;
    of.songname  = DupStr(mh->message,108);
    of.modtype   = strdup(S69_Version[memcmp(mh->marker,"JN",2)==0]);
    of.numchn    = 8;
    of.numpat    = mh->nop;
    of.numins    = of.numsmp = mh->nos;
    of.numtrk    = of.numchn*of.numpat;
    of.flags     = UF_XMPERIODS; /* | UF_LINEAR; */

    if(!AllocPositions(0x80)) return 0;
    for(t=0; t<0x80; t++)
    {   if(mh->orders[t]==0xff) break;
        of.positions[t] = mh->orders[t];
    }

    of.numpos = t;

    if(!AllocSamples()) return 0;
    q = of.samples;

    for(t=0; t<of.numins; t++)
    {   /* try to read sample info */

        _mm_read_UBYTES((UBYTE *)s.filename,13,modfp);
        s.length   = _mm_read_I_SLONG(modfp);
        s.loopbeg  = _mm_read_I_SLONG(modfp);
        s.loopend  = _mm_read_I_SLONG(modfp);

        if((s.length < 0) || (s.loopbeg < -1) || (s.loopend < -1))
        {   _mm_errno = MMERR_LOADING_HEADER;
            return 0;
        }

        q->samplename = DupStr(s.filename,13);

        q->seekpos   = 0;
        q->speed     = 0;
        q->length    = s.length;
        q->loopstart = s.loopbeg;
        q->loopend   = (s.loopend<s.length) ? s.loopend : s.length;
        q->flags     = (s.loopbeg<s.loopend) ? SF_LOOP : 0;
        q->volume    = 64;

        q++;
    }

    if(!S69_LoadPatterns()) return 0;

    return 1;
}


MLOADER load_669 =
{   NULL,
    "669",
    "Portable 669 loader v0.1",
    S69_Init,
    S69_Test,
    S69_Load,
    S69_Cleanup,
    NULL
};


