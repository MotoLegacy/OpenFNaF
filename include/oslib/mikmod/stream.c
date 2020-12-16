/*
 File: STREAM.C

 Description:
 Streaming Audio module for MikMod.

 Portability:
 All compilers - All systems

*/


#include "mikmod.h"


int   stream_bufsize = 100;  /* in 1/100th seconds */

static BOOL    is_streaming = 0;
static MSTREAM *firststream = NULL;

static BOOL active;

VIRTUAL_FILE  *stream_fp;
SLONG stream_seekpos;
SLONG stream_reppos;

void MikMod_RegisterStream(MSTREAM *stream)
{
    MSTREAM *cruise = firststream;

    if(cruise!=NULL)
    {   while(cruise->next!=NULL)  cruise = cruise->next;
        cruise->next = stream;
    } else
        firststream = stream;
}


BOOL Stream_PlayFN(CHAR *filename)
{
    return 0;
}


BOOL Stream_PlayFP(VIRTUAL_FILE *fp)
{
    BOOL    ok;
    MSTREAM *l;

    stream_fp = fp;
    _mm_errno = 0;
    _mm_critical = 0;

    _mm_iobase_setcur(stream_fp);

    /* Try to find a loader that recognizes the stream */

    for(l=firststream; l!=NULL; l=l->next)
    {   _mm_rewind(stream_fp);
        if(l->Test()) break;
    }

    if(l==NULL)
    {   _mm_errno = MMERR_NOT_A_STREAM;
        if(_mm_errorhandler!=NULL) _mm_errorhandler();
        _mm_iobase_revert();
        return 1;
    }

    /* init stream loader and load the header */
    if(l->Init())
    {   _mm_rewind(stream_fp);
        ok = l->Load();
    }

    /* free loader allocations */
    l->Cleanup();

    if(!ok)
    {   _mm_iobase_revert();
        return 1;
    }

    /*loadbuf = _mm_malloc(8192); */

    _mm_iobase_revert();
    active = 1;
    return 0;
}


BOOL Stream_Init(ULONG speed, UWORD flags)

/* size = size of buffer in 1/100th seconds */

{
    if(is_streaming) md_driver->StreamExit();
    if(md_driver->StreamInit(speed,flags)) return 1;
    is_streaming = 1;

    return 0;
}


void Stream_Exit(void)
{
    if(is_streaming) md_driver->StreamExit();
    is_streaming = 0;
}


static ULONG last = 0;

void Stream_Update(void)
{
    ULONG curr;
    ULONG todo;

    curr = md_driver->StreamGetPosition();
    if(curr==last) return;

    if(curr>last)
    {   todo = curr-last;
        /*SL_LoadStream(,vstream.infmt,vstream.flags,todo,stream_fp); */
        last += todo;
        /*if(last>=vstream.size) last = 0; */
    } else
    {   /*todo = vstream.size-last; */
        /*SL_LoadStream(&vstream.buffer[last],vstream.infmt,vstream.flags,todo,stream_fp); */
        /*SL_LoadStream(vstream.buffer,vstream.infmt,vstream.flags,curr,stream_fp); */
        last = curr;
    }
}

