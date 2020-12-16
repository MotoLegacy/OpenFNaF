#include <pspkernel.h>
#include <pspsdk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <psputility.h>
#include <psputility_netmodules.h>
#include <psputility_htmlviewer.h>
#include <psphttp.h>

#include "oslib.h"
#include "browser.h"

#define BROWSER_MEMORY (10*1024*1024)

static SceUID vpl;
static pspUtilityHtmlViewerParam params;
static int browserIsActive = 0;

int oslBrowserInit(char *url, char *downloadDir, int browserMemory, unsigned int displaymode, unsigned int options, unsigned int interfacemode, unsigned int connectmode){
	int res;
    if (browserMemory <= 0)
        browserMemory = BROWSER_MEMORY;

	vpl = sceKernelCreateVpl("BrowserVpl", PSP_MEMORY_PARTITION_USER, 0, browserMemory + 256, NULL);
	if (vpl < 0)
		return OSL_BROWSER_ERROR_MEMORY;

	memset(&params, 0, sizeof(pspUtilityHtmlViewerParam));

	params.base.size = sizeof(pspUtilityHtmlViewerParam);

	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &params.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &params.base.buttonSwap);

	params.base.graphicsThread = 17;
	params.base.accessThread = 19;
	params.base.fontThread = 18;
	params.base.soundThread = 16;
	params.memsize = browserMemory;
	params.initialurl = url;
	params.numtabs = 1;
	params.cookiemode = PSP_UTILITY_HTMLVIEWER_COOKIEMODE_DEFAULT;
	params.homeurl = url;
	params.textsize = PSP_UTILITY_HTMLVIEWER_TEXTSIZE_NORMAL;
	params.displaymode = displaymode;
	params.options = options;
	params.interfacemode = interfacemode;
	params.connectmode = connectmode;

	// WITHOUT 'ms0:' on the paths
	params.dldirname = downloadDir;

	res = sceKernelAllocateVpl(vpl, params.memsize, &params.memaddr, NULL);
	if (res < 0)
		return OSL_BROWSER_ERROR_MEMORY;

	res = sceUtilityHtmlViewerInitStart(&params);
	if (res < 0)
		return res;								//<-- STAS: Such a return value is more informative...
//		return OSL_BROWSER_ERROR_INIT;

	browserIsActive = 1;
	return 0;
}


void oslDrawBrowser()
{
	switch (sceUtilityHtmlViewerGetStatus()){
		case PSP_UTILITY_DIALOG_INIT:
			break;							//<-- STAS: We shouldn't show the browser in its INIT status!
		case PSP_UTILITY_DIALOG_VISIBLE:
			sceGuFinish();
			sceGuSync(0,0);
			sceUtilityHtmlViewerUpdate(1);
			sceGuStart(GU_DIRECT, osl_list);
			oslSetAlpha(OSL_FX_RGBA, 0xff);
			break;
		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityHtmlViewerShutdownStart();
			break;
		case PSP_UTILITY_DIALOG_NONE:
		case PSP_UTILITY_DIALOG_FINISHED:
			break;
	}
}


int oslGetBrowserStatus(){
	return sceUtilityHtmlViewerGetStatus();
}

int oslBrowserIsActive(){
    return browserIsActive;
}

void oslEndBrowser(){
	sceKernelFreeVpl(vpl, params.memaddr);
	sceKernelDeleteVpl(vpl);
	browserIsActive = 0;
}
