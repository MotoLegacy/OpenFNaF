#include <pspkernel.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <psputility.h>

#include "oslib.h"
#include "osk.h"

//SceUtilityOskData OskData;			//<-- STAS: this is not necessary now
//SceUtilityOskParams OskParams;		//<-- STAS: this is not necessary now
unsigned short *intext = NULL;
//unsigned short *outtext;				//<-- STAS: this is not necessary now
//unsigned short desc[128]  = { 0 };	//<-- STAS: this is not necessary now
unsigned short *desc = NULL;

//int OskActive = 0;					//<-- STAS: this is not necessary now
OSL_KEYBOARD* osl_osk = NULL;			//<-- STAS: static ptr to the currently active OSL_KEYBOARD


OSL_KEYBOARD* oslInitOskEx(int nData, int language)
{
    OSL_KEYBOARD* kbd;

    kbd = (OSL_KEYBOARD*) malloc(sizeof(OSL_KEYBOARD));
    if (!kbd)  return NULL;
    memset(kbd, 0, sizeof(OSL_KEYBOARD));

    kbd->oskParams.base.size = sizeof(SceUtilityOskParams);
    if (language >= 0)
       kbd->oskParams.base.language = language;
    else
       sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &kbd->oskParams.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &kbd->oskParams.base.buttonSwap);
    kbd->oskParams.base.graphicsThread = 17;
    kbd->oskParams.base.accessThread = 19;
    kbd->oskParams.base.fontThread = 18;
    kbd->oskParams.base.soundThread = 16;
    kbd->oskParams.datacount = oslMax(1,nData);
    kbd->oskParams.data = (SceUtilityOskData*) malloc(kbd->oskParams.datacount * sizeof(SceUtilityOskData));
    if (!kbd->oskParams.data) {
       free(kbd);
       return NULL;
    }

    memset(kbd->oskParams.data, 0, kbd->oskParams.datacount * sizeof(SceUtilityOskData));
    return kbd;
}

int oslInitOskDataEx(OSL_KEYBOARD *kbd, int idx,
                     unsigned short *desc, unsigned short *intext, int textLimit, int linesNumber)
{
    if (!kbd  ||  idx < 0  ||  idx >= kbd->oskParams.datacount)
       return 0;

    unsigned short *outtext = (unsigned short *) malloc((textLimit + 1)*sizeof(unsigned short));
    if (!outtext)
       return 0;

    memset(&kbd->oskParams.data[idx], 0, sizeof(SceUtilityOskData));
    kbd->oskParams.data[idx].language = PSP_UTILITY_OSK_LANGUAGE_DEFAULT;
    kbd->oskParams.data[idx].lines = linesNumber;
    kbd->oskParams.data[idx].unk_24 = 1;			    // set to 1
    kbd->oskParams.data[idx].desc = desc;
    kbd->oskParams.data[idx].intext = intext;
    kbd->oskParams.data[idx].outtextlength = textLimit;
    kbd->oskParams.data[idx].outtextlimit = textLimit;
    kbd->oskParams.data[idx].outtext = outtext;

    return 1;
}

int oslActivateOskEx(OSL_KEYBOARD *kbd, int waitcycle)
{
    if (!kbd) return -1;

    if (!osl_osk) {			// There is no active OSK - activate the given one
       osl_osk = kbd;
       int ret = sceUtilityOskInitStart(&kbd->oskParams);
       if (ret < 0) osl_osk = NULL;
       return ret;
    }
    else if (osl_osk == kbd)
       return 0;			// The given OSK is currently active...
    else if (!waitcycle)
       return -2;			// Other OSK is currently active...
    else {					// Wait for other OSK to finish and try again to activate the given one
       while (osl_osk) sceDisplayWaitVblankStart();
       return oslActivateOskEx(kbd, waitcycle);
    }
}

int oslOskIsActiveEx(OSL_KEYBOARD *kbd)
{
    return (kbd  &&  (kbd == osl_osk));
}

void oslDeActivateOskEx(OSL_KEYBOARD *kbd)
{
    if (oslOskIsActiveEx(kbd))
       osl_osk = NULL;
}

int oslOskGetResultEx(OSL_KEYBOARD *kbd, int idx)
{
    if (!kbd) return 0;
    else if (idx < 0  ||  idx >= kbd->oskParams.datacount)
       return kbd->oskParams.base.result;
    else
       return kbd->oskParams.data[idx].result;
}

unsigned short* oslOskOutTextEx(OSL_KEYBOARD *kbd, int idx)
{
    if (!kbd  ||  idx < 0  ||  idx >= kbd->oskParams.datacount)
       return NULL;
    else
       return kbd->oskParams.data[idx].outtext;
}


void oslEndOskEx(OSL_KEYBOARD *kbd)
{
    if (!kbd  || !oslOskIsActiveEx(kbd))   return;

    oslDeActivateOskEx(kbd);
    int i;
    for (i = 0; i < kbd->oskParams.datacount; i++) {
       free(kbd->oskParams.data[i].outtext);
    }
    free(kbd->oskParams.data);
    free(kbd);
}


void oslInitOsk(char *descStr, char *initialStr, int textLimit, int linesNumber, int language){
    int i = 0;

    if (intext || desc)   return;			//<-- STAS: Is the static OSK already initialized?

//    intext = (unsigned short *) malloc((textLimit + 1)*sizeof(unsigned short));	//<-- STAS: textLimit ISN'T right!
    intext = (unsigned short *) malloc((strlen(initialStr) + 1)*sizeof(unsigned short));
    if (!intext)
        return;

    desc = (unsigned short *) malloc((strlen(descStr) + 1)*sizeof(unsigned short));
    if (!desc) {
        oslEndOsk();
        return;
    }

    for (i=0; i<=strlen(initialStr); i++)
        intext[i] = (unsigned short)initialStr[i];

    for (i=0; i<=strlen(descStr); i++){
        desc[i] = (unsigned short)descStr[i];
    }

    OSL_KEYBOARD *kbd = oslInitOskEx(1, language);
    if (!kbd || !oslInitOskDataEx(kbd, 0, desc, intext, textLimit, linesNumber) || (oslActivateOskEx(kbd,0)<0)) {
        oslEndOsk();
    }
}

void oslDrawOsk()
{
    switch(sceUtilityOskGetStatus()){
//		case PSP_UTILITY_DIALOG_INIT:			//<-- STAS: sceUtilityOskUpdate should be called only in VISIBLE state!
        case PSP_UTILITY_DIALOG_VISIBLE :
            sceDisplayWaitVblankStart();
            sceDisplayWaitVblankStart();
			sceGuFinish();
			sceGuSync(0,0);
            sceUtilityOskUpdate(2);
			sceGuStart(GU_DIRECT, osl_list);
			oslSetAlpha(OSL_FX_RGBA, 0xff);
            break;
        case PSP_UTILITY_DIALOG_QUIT:
            sceUtilityOskShutdownStart();
            break;
//        case PSP_UTILITY_DIALOG_FINISHED:		//<-- STAS: nothing to do for the other states
//        case PSP_UTILITY_DIALOG_NONE:
        default:
			break;								//<-- STAS END -->
    }
}

int oslOskIsActive(){
    return oslOskIsActiveEx(osl_osk);
}

int oslGetOskStatus(){
    return sceUtilityOskGetStatus();
}


int oslOskGetResult(){
    return oslOskGetResultEx(osl_osk, -1);
}

void oslOskGetText(char *text){
    int i, j;
    j = 0;
    if (osl_osk)  for(i = 0; osl_osk->oskParams.data[0].outtext[i]; i++) {
        if (osl_osk->oskParams.data[0].outtext[i]!='\n' && osl_osk->oskParams.data[0].outtext[i]!='\r'){
            text[j] = osl_osk->oskParams.data[0].outtext[i];
            j++;
        }
    }
    text[j] = 0;
}

void oslOskGetTextUCS2(unsigned short *text){
    int i, j;
    j = 0;
    if (osl_osk)  for(i = 0; osl_osk->oskParams.data[0].outtext[i]; i++) {
        if (osl_osk->oskParams.data[0].outtext[i]!='\n' && osl_osk->oskParams.data[0].outtext[i]!='\r'){
            text[j] = osl_osk->oskParams.data[0].outtext[i];
            j++;
        }
    }
    text[j] = 0;
}

void oslEndOsk(){
    if (intext) free(intext);
    if (desc)   free(desc);
    intext  = NULL;
    desc    = NULL;
    oslEndOskEx(osl_osk);
}
