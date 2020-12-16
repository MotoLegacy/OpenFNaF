/*

Name: LOAD_UNI.C

Description:
UNIMOD (mikmod's internal format) module loader.
Currently only supports UniMOD 06 - the internal format for MikMod 3.0.

Portability:
All systems - all compilers (hopefully)

*/

#include <string.h>
#include <stdlib.h>
#include "mikmod.h"

enum {OSL_MB_OK=1, OSL_MB_CANCEL, OSL_MB_YES, OSL_MB_NO, OSL_MB_QUIT};
enum {OSL_KEY_SELECT=1, OSL_KEY_START=4, OSL_KEY_UP=5, OSL_KEY_RIGHT=6, OSL_KEY_DOWN=7, OSL_KEY_LEFT=8, OSL_KEY_L=9, OSL_KEY_R=10,
		OSL_KEY_TRIANGLE=13, OSL_KEY_CIRCLE=14, OSL_KEY_CROSS=15, OSL_KEY_SQUARE=16, OSL_KEY_HOME=17, OSL_KEY_HOLD=18, OSL_KEY_NOTE=24};
extern unsigned int oslMessageBox(const char *text, const char *title, unsigned int flags);
#define oslMake3Buttons(b1,a1,b2,a2,b3,a3)		((b1)|((a1)<<5)|((b2)<<9)|((a2)<<14)|((b3)<<18)|((a3)<<23))
#define oslDebug(format...)		({ char __str2[1000], __str[1000]; sprintf(__str2, "Debug (%s:%i,%s)",__FUNCTION__,__LINE__,__FILE__); sprintf(__str , ##format); oslMessageBox(__str, __str2, oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_OK,OSL_KEY_TRIANGLE,OSL_MB_QUIT,0,0)); })


BOOL UNI_Test(void)
{
    UBYTE id[4];

    _mm_read_UBYTES(id,4,modfp);
    if(!memcmp(id, "UN06", 4)) return 1;

    return 0;
}


BOOL UNI_Init(void)
{
    return 1;
}


void UNI_Cleanup(void)
{
}


UBYTE *TrkRead(void)
{
    UBYTE  *t;
    UWORD  len;

    len = _mm_read_M_UWORD(modfp);
    t   = (UBYTE *)malloc(len);
    _mm_read_UBYTES(t,len,modfp);

    return t;
}


BOOL UNI_Load(void)
{
    int t,v,w;
    INSTRUMENT *i;
    SAMPLE *s;

    /* UNI format version 3.0 (#6) */
    
    of.modtype    = strdup("MikMod UniFormat 3.0");

    _mm_fseek(modfp,5,SEEK_SET);    /* skip the header */
    
    of.flags      = _mm_read_M_UWORD(modfp);
    of.numchn     = _mm_read_UBYTE(modfp);
    of.numvoices  = _mm_read_UBYTE(modfp);
    of.numpos     = _mm_read_M_UWORD(modfp);
    of.numpat     = _mm_read_M_UWORD(modfp);
    of.numtrk     = _mm_read_M_UWORD(modfp);
    of.numins     = _mm_read_M_UWORD(modfp);
    of.numsmp     = _mm_read_M_UWORD(modfp);
    of.reppos     = _mm_read_M_UWORD(modfp);
    of.initspeed  = _mm_read_UBYTE(modfp);
    of.inittempo  = _mm_read_UBYTE(modfp);
    of.initvolume = _mm_read_UBYTE(modfp);

    if(_mm_feof(modfp))
    {   _mm_errno = MMERR_LOADING_HEADER;
        return 0;
    }

    of.songname = StringRead(modfp);
    of.composer = StringRead(modfp);
    of.comment  = StringRead(modfp);

    if(_mm_feof(modfp))
    {   _mm_errno = MMERR_LOADING_HEADER;
        return 0;
    }

    if(!AllocSamples())            return 0;
    if(!AllocTracks())             return 0;
    if(!AllocPatterns())           return 0;
    if(!AllocPositions(of.numpos)) return 0;

    _mm_read_UBYTES(of.positions,of.numpos,modfp);
    _mm_read_M_UWORDS(of.panning,of.numchn,modfp);
    _mm_read_UBYTES(of.chanvol,of.numchn,modfp);


    /* Load sample headers */

    s = of.samples;
    for(v=0; v<of.numsmp; v++, s++)
    {   s->flags      = _mm_read_M_UWORD(modfp);
        s->speed      = _mm_read_M_ULONG(modfp);
        s->volume     = _mm_read_UBYTE(modfp);
        s->panning    = _mm_read_M_UWORD(modfp);
        s->length     = _mm_read_M_ULONG(modfp);
        s->loopstart  = _mm_read_M_ULONG(modfp);
        s->loopend    = _mm_read_M_ULONG(modfp);
        s->susbegin   = _mm_read_M_ULONG(modfp);
        s->susend     = _mm_read_M_ULONG(modfp);

        s->globvol    = _mm_read_UBYTE(modfp);
        s->vibflags   = _mm_read_UBYTE(modfp);
        s->vibtype    = _mm_read_UBYTE(modfp);
        s->vibsweep   = _mm_read_UBYTE(modfp);
        s->vibdepth   = _mm_read_UBYTE(modfp);
        s->vibrate    = _mm_read_UBYTE(modfp);
        s->samplename = StringRead(modfp);

        if(_mm_feof(modfp))
        {   _mm_errno = MMERR_LOADING_HEADER;
            return 0;
        }
    }

    /* Load instruments */

    if(of.flags & UF_INST)
    {   if(!AllocInstruments()) return 0;
        i = of.instruments;

        for(v=0; v<of.numins; v++, i++)
        {   i->flags        = _mm_read_UBYTE(modfp);
            i->nnatype      = _mm_read_UBYTE(modfp);
            i->dca          = _mm_read_UBYTE(modfp);
            i->dct          = _mm_read_UBYTE(modfp);
            i->globvol      = _mm_read_UBYTE(modfp);
            i->panning      = _mm_read_M_UWORD(modfp);
            i->pitpansep    = _mm_read_UBYTE(modfp);
            i->pitpancenter = _mm_read_UBYTE(modfp);
            i->rvolvar      = _mm_read_UBYTE(modfp);
            i->rpanvar      = _mm_read_UBYTE(modfp);

            i->volfade      = _mm_read_M_UWORD(modfp);
    
            i->volflg       = _mm_read_UBYTE(modfp);
            i->volpts       = _mm_read_UBYTE(modfp);
            i->volsusbeg    = _mm_read_UBYTE(modfp);
            i->volsusend    = _mm_read_UBYTE(modfp);
            i->volbeg       = _mm_read_UBYTE(modfp);
            i->volend       = _mm_read_UBYTE(modfp);
    
            for(w=0; w<i->volpts; w++)
            {   i->volenv[w].pos = _mm_read_M_SWORD(modfp);
                i->volenv[w].val = _mm_read_M_SWORD(modfp);
            }
    
            i->panflg    = _mm_read_UBYTE(modfp);
            i->panpts    = _mm_read_UBYTE(modfp);
            i->pansusbeg = _mm_read_UBYTE(modfp);
            i->pansusend = _mm_read_UBYTE(modfp);
            i->panbeg    = _mm_read_UBYTE(modfp);
            i->panend    = _mm_read_UBYTE(modfp);
    
            for(w=0; w<i->panpts; w++)
            {   i->panenv[w].pos = _mm_read_M_SWORD(modfp);
                i->panenv[w].val = _mm_read_M_SWORD(modfp);
            }
    
            i->pitflg    = _mm_read_UBYTE(modfp);
            i->pitpts    = _mm_read_UBYTE(modfp);
            i->pitsusbeg = _mm_read_UBYTE(modfp);
            i->pitsusend = _mm_read_UBYTE(modfp);
            i->pitbeg    = _mm_read_UBYTE(modfp);
            i->pitend    = _mm_read_UBYTE(modfp);
    
            for(w=0; w<i->pitpts; w++)
            {   i->pitenv[w].pos = _mm_read_M_SWORD(modfp);
                i->pitenv[w].val = _mm_read_M_SWORD(modfp);
            }
    
            _mm_read_UBYTES(i->samplenumber, 120, modfp);
            _mm_read_UBYTES(i->samplenote, 120, modfp);
    
            i->insname = StringRead(modfp);

            if(_mm_feof(modfp))
            {   _mm_errno = MMERR_LOADING_HEADER;
                return 0;
            }
        }
    }

    /* Read patterns */

    _mm_read_M_UWORDS(of.pattrows,of.numpat,modfp);
    _mm_read_M_UWORDS(of.patterns,of.numpat*of.numchn,modfp);

    /* Read tracks */

    for(t=0; t<of.numtrk; t++)
        of.tracks[t] = TrkRead();

    return 1;
}


CHAR *UNI_LoadTitle(void)
{
    _mm_fseek(modfp,24,SEEK_SET);
    return(StringRead(modfp));
}


MLOADER load_uni =
{   NULL,
    "UNI",
    "Portable UniFormat 3.0 Loader",
    UNI_Init,
    UNI_Test,
    UNI_Load,
    UNI_Cleanup,
    UNI_LoadTitle
};

