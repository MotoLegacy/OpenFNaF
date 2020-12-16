/*! \file oslib.h
    \brief Main Oldschool Library file.
*/

#ifndef _OSLIB_H_
#define _OSLIB_H_

#ifdef PSP
	#include <pspkernel.h>
	#include <pspdisplay.h>
	#include <pspdebug.h>
#else
	#define _CRT_SECURE_NO_DEPRECATE
	#include "emu.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifdef PSP
	#include <pspctrl.h>
	#include <psputility.h>
	#include <pspgu.h>
	#include <pspgum.h>
	#include <psppower.h>
	#include <pspiofilemgr.h>
	#include <psphprm.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup main Main

    Main OSLib section.
	@{
*/

/** @defgroup main_general General

	Contains general routines to initialize and terminate the library.
	@{
*/

/** OSLib version */
#define OSL_VERSION "MODv2 1.1.2"

extern int osl_intraInit;

/** Initializes the library. The options for the flags parameters are for advanced users only, let it to zero if you're beginning.

	\param flags
		Can be formed of one or more of the values from the #OSL_INITFLAGS enumeration (ORed together).
*/
extern void oslInit(int flags);

/** Flags for oslInit. */
enum OSL_INITFLAGS	{
	OSL_IF_USEOWNCALLBACKS=1,						//!< Does not set up the standard callbacks. You'll have to provide them by yourself as shown in almost all PSPSDK samples.
	OSL_IF_NOVBLANKIRQ=2,							//!< Does not reserve the VBLANK IRQ. You should set up your own VBL handler, which should call oslVblankNextFrame() if osl_vblankCounterActive is TRUE, else oslSyncFrame(Ex) will not work properly.
};

/** Exits the application immediately, returning to the XMB (PSP main menu) or to the calling application (e.g. IRShell).
*/
extern void oslQuit();

/** Sets the "no fail" mode. In this mode, if OSLib can't load a file (for example an image) it will display a gentleful message indicating to the user the name of the missing file, and quit immediately.

\code
//Our objects (will be loaded)
OSL_IMAGE *img;
OSL_FONT *font;
OSL_SOUND *sound;

//Initialize OSLib
oslInit(0);
//Set-up drawing on a 5650 (16-bit) screen
oslInitGfx(OSL_PF_5650, 1);
//Enable the no-fail feature
oslSetQuitOnLoadFailure(1);

//An error will be displayed if any of these files could not be loaded properly.
image = oslLoadImageFilePNG("test.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_5551);
font = oslLoadFontFile("font.oft");
sound = oslLoadSoundFileWAV("noise.wav", OSL_FMT_NONE);

//Here, we are sure everything is okay, we don't even need checking if they are NULL.
oslSetFont(font);
oslDrawImage(img);
oslPlaySound(sound, 0);
\endcode */
#define oslSetQuitOnLoadFailure(enabled)				(osl_noFail = (enabled))

/** Sets the function to call in case of failure to load something. Default displays a message box.

\code
//Our own function called when a loading has failed
void MyNoFailCallback(const char *filename, u32 reserved)		{
	//Display an error
	myDisplayMessage("Unable to locate the following file: %s\nPlease reinstall the application.", filename);
	//Always quit because we can't continue without these precious files
	oslQuit();
}

//Set-up no-fail.
oslSetQuitOnLoadFailure(1);
oslSetQuitOnLoadFailureCallback(MyNoFailCallback);
//This function will call MyNoFailCallback  because the specified file doesn't exist
oslLoadImageFileJPG("void:/notexisting.xyz", OSL_IN_RAM, OSL_PF_4BIT);
\endcode
*/
#define oslSetQuitOnLoadFailureCallback(function)		(osl_noFailCallback = function)

/** Sets an exit callback. This function is executed when the user chooses "Exit" from the HOME menu. By default, all what OSLib does is set osl_quit to True, so that you
know that you should quit (all your game loops should watch out for osl_quit). A typical but dirty callback may be to force quitting immediately using oslQuit. Example:

\code
int myExitCallback(int arg1, int arg2, void *common)
{
	oslQuit();
	return 0;
}

void main()			{
	oslSetExitCallback(myExitCallback);
	while(1);
}
\endcode */
#define oslSetExitCallback(function)					(osl_exitCallback = function)


//Don't access these
extern int osl_noFail;
extern void (*osl_noFailCallback)(const char *filename, u32 reserved);
extern void oslHandleLoadNoFailError(const char *filename);

/** @} */ // end of main_general


/** @defgroup main_sync Synchronization

	Contains routines for synchronizing your game.
	@{
*/

/** Sets the current fix frameskipping value.
	\param val
		1 = enables auto frameskip
		2 or more: enables fixed skipping (val-1 frames out of val are skipped)
*/
#define oslSetFrameskip(val) (osl_frameskip = val)
/** Sets the maximum frameskip value. If you set a too high value, the game may become unusable when lagging, if you set a too low value, frameskip is no more useful.
*/
#define oslSetMaxFrameskip(val) (osl_maxFrameskip = val)
/** Sets the VSync parameter.
	\param val
		0: Automatic VSync (enabled if the game is not lagging).
		1: VSync always enabled.
		+4: If you add 4, with a frameskip > 1, synchronizes to the wanted framerate, e.g. 2 -> 30 fps (DON'T USE THIS ANYMORE)
		+0: Else, fixes the minimum frameskip (e.g. 2: the game will run at 60 fps, 30 images per second, that is one frame out of two is skipped)
		+8: Alternative synchronization method without VSync
		+16: Do not swap buffers. Handy if you want temporary to "disable double buffering".
*/
#define oslSetVSync(val) (osl_vsyncEnabled = val)
/** Synchronization. Call it to the end of a frame. A frame is one step when animating your game. The PSP LCD refreshes at 60 images per second (and thus, you'll have to generate 60 frames per second).
	This function uses the frameskip & vsync parameters defined above.
	\return
		A boolean value indicating if you're in late.

	Note: When you're indicated to be in late, you should skip the rendering to speed up things, else your game will slow down. Usually the whole render is surrounded by a "if (!skip)" block, and when
	skip is true, the render is completely skipped. Skip is the return value from oslSyncFrame.

	Here is the structure of an automatically frameskipped program:
\code
//This variable holds whether a frame should be skipped or not
int skip = 0;
//Enable frameskipping, 0 frame out of 1 is skipped by default (60 fps)
oslSetFrameskip(1);
//But skip a maximum of 3 frames out of 4, else the game seems very laggy
oslSetMaxFrameskip(4);

//Our main loop
while(1)		{
	//If skip is 1, then we should skip rendering to speed up things
	if (!skip)		{
		//Begin drawing
		oslStartDrawing();
		//We can now draw as much as we want
		oslDrawFillRect(0, 0, 100, 100, RGBA(0, 0, 255, 128));
		oslDrawString(0, 0, "Hello!");
		//We have now finished with drawing
		oslEndDrawing();
	}

	//End of a frame
	oslEndFrame();
	//Synchronize to ensure the game runs at 60 fps
	skip = oslSyncFrame();
}
\endcode
*/
#define oslSyncFrame()	oslSyncFrameEx(osl_frameskip,osl_maxFrameskip,osl_vsyncEnabled)

/** Call this function when a frame has ended, just before a call to oslSyncFrame or oslWaitVSync or anything.
\b Important: If you are calling this function, you do not need to call #oslAudioVSync anymore. Newer programs should always call #oslEndFrame instead. */
extern void oslEndFrame();


/** This variable is set to TRUE to indicate the application should exit (happens when the user pressed HOME and selected Yes). You should always keep an eye on this variable, clean up everything and
then quit when this variable has become TRUE. */
extern int osl_quit;

//Document these
extern int osl_standByUnpermitted;
extern int (*osl_powerCallback)(int, int, void*);
extern int (*osl_exitCallback)(int, int, void*);
extern int osl_vblInterruptNumber;									//Num�ro de l'interruption VBLANK utilis�e

//Don't access these
extern int osl_maxFrameskip, osl_vsyncEnabled, osl_frameskip;
extern volatile int osl_vblCount, osl_vblCallCount, osl_vblankCounterActive, osl_skip;
extern volatile int osl_currentFrameRate;

/** Same as oslSyncFrame but with all parameters set in one call. This method is a bit cleaner because the other one stores parameters in global variables and oslSyncFrame calls oslSyncFrameEx with them
as arguments. */
extern int oslSyncFrameEx(int frameskip, int max_frameskip, int vsync);

/** Sets the framerate for oslSyncFrame(Ex). This can be any value from 1 to 60. For example, use 50 to simulate a PAL (european) game. */
static inline void oslSetFramerate(int framerate)		{
	if (framerate <= 60)
		osl_currentFrameRate = framerate;
}

/** Waits for the VSync (60 times per second), synchronizing your game. This is a simplier and less efficient method than oslSyncFrame. */
extern void oslWaitVSync();

/** @} */ // end of main_sync


/** @defgroup main_memory Memory

	Contains routines about memory (cache, copy, etc.).
	@{
*/

/** Copies data using the internal DMAC. Should be faster than a memcpy, but requires that data to be copied is no more in the cache, so usually you should issue a oslUncacheData on the source and
destination addresses else very strange bugs may happen. */
extern int sceDmacMemcpy(void *dest, const void *source, unsigned int size);				//Copie DMA
/** Same as sceDmacMemcpy. */
extern int sceDmacTryMemcpy(void *dest, const void *source, unsigned int size);
/** Does a memset using the 64-bit capabilities of the CPU. Faster than memcpy. */
extern void oslFasterMemset(u64 *dst, u64 *src, u32 length);

/** This routine will make sure that the data will no more reside in the CPU cache. The CPU does automatically put data in cache when you want to read or write to it, making subsequent accesses faster
(because the cache is way faster than conventional RAM). The problem is that if the GE (graphic processor) for example wants to access it, it will read the real memory, and thus if some data is still
sitting in the cache (not written to actual RAM yet) the GE will get incorrect data, making it crash or display corrupt graphics.

To avoid problems, you can either bypass the cache by writting to uncached addresses (oslGetUncachedPtr) or by flushing the data from cache before sending them to the GE (or DMA). This function,
oslUncacheData, will flush the actual data from cache, writing it to real memory. This does take some time so don't abuse and call it only when necessary.

	\param data
		Adress of the memory area to flush.
	\param size
		Size (in bytes) of the area.

Uncaching should be done after you've accessed raw image / palette / something contents. This includes oslSetImagePixel and accesses to the raw image data pointed to by OSL_IMAGE::data.

Note: There are specific functions to uncache palettes and images, simplier to use. You should rather use them instead.
*/
#define oslUncacheData(data,size)		sceKernelDcacheWritebackInvalidateRange(data, size);

/** Returns a pointer to an uncached address. In this case, cache will be bypassed and you'll not have to call oslUncache[something] after modifying data. However performance is usually worse and caching
is a good thing, so do it if you know what you are doing.

Important: NEVER mix cached and uncached addresses when accessing somewhere. Either always access them as cached or unached. If you mix them some weird bugs may appear
because you could for example write directly in memory bypassing the cache, but some data is still sitting in the cache. So, once the cache is full, data will be flushed and written to the memory, replacing the
data you wrote in an uncached way. You will not understand why your data gets corrupted, and as this will maybe happen seconds later in special circumstances, so it will be impossible to detect or
debug. */
#define oslGetUncachedPtr(adr)			((void*)((int)(adr)|0x40000000))
/** Get a pointer to cached data. Same remark as above. */
#define oslGetCachedPtr(adr)			((void*)((int)(adr)&(~0x40000000)))

#ifdef PSP
	/** Flushes the whole cache. This is slow, absolutely avoid it! Use oslUncacheData instead if possible. */
	extern void oslFlushDataCache();
#else
	extern void oslFlushDataCache();
#endif

/** Allocates a memory block, ensuring it is aligned.
	\param alignment
		Alignment in bytes
	\param size
		Size of the block
*/
void *memalign(size_t alignment, size_t size);

/** Structure for the return value of oslGetRamStatus. */
typedef struct		{
	int maxAvailable;					//!< Maximum available memory
	int maxBlockSize;					//!< Maximum linear memory available
} OSL_MEMSTATUS;

/** Gets info about currently available memory in main RAM. The return value is a #OSL_MEMSTATUS
structure which contains info about available memory.

Example:
\code
	//You can either store the result to a structure and access its members
	OSL_MEMSTATUS ram = oslGetRamStatus();
	oslDebug("%i bytes available", ram.maxAvailable);

	//Or directly use the return value from the function, like this
	oslDebug("%i bytes available", oslGetRamStatus().maxAvailable);
\endcode

\b Note: About the example above, just notice that each call to oslGetRamStatus costs a lot of time, so if you need to
retrieve several results (e.g. both maxAvailable and maxBlockSize) you should not use the second method as it will need two
calls to oslGetRamStatus. */
OSL_MEMSTATUS oslGetRamStatus();

/** @} */ // end of main_memory


/** @defgroup main_maths Maths

	Main OSLib section. Contains the main routines to initialize and terminate the library.
	@{
*/

/** Returns the absolute value of a number (that is, the positive part of the number).

\code
//Prints 2
oslPrintf("%i", oslAbs(2));
//Prints 2 as well (the minus sign is ignored in an absolute value)
oslPrintf("%i", oslAbs(-2));
\endcode */
#define oslAbs(x)					(((x)<0)?(-(x)):(x))

/** Returns the smallest value between the two. */
#define oslMin(x, y)				(((x)<(y))?(x):(y))
/** Returns the greatest value between the two.

\code
//Move the object 1 pixel to the left but make sure it will never go beyond the left corner of the screen.
posX = oslMax(posX - 1, 0);
\endcode */
#define oslMax(x, y)				(((x)>(y))?(x):(y))
/** Returns a value clamped between a minimum and maximum.

\code
int addColor(int colValue, int addValue)			{
	//We add addValue to colValue, but we must check the color component is always between 0 and 255.
	colValue = oslMinMax(colValue + addValue, 0, 255);
	return colValue;
}
\endcode */
#define oslMinMax(x, min, max)		((x) < (max) ? ((x) > (min) ? (x) : (min)) : (max))

/** Returns the number of objects in an array.

\code
OSL_VIRTUALFILENAME ram_files[] =		{
	{"ram:/file1.jpg", (void*)file1_jpg, file1_jpg_size, &VF_MEMORY},
	{"ram:/file2.png", (void*)file2_png, file2_png_size, &VF_MEMORY},
};

//This will display 2, as 2 elements are contained in the table ram_files.
oslPrintf("%i", oslNumberof(ram_files));
\endcode */
#define oslNumberof(n)				(sizeof(n)/sizeof(*(n)))

/** Calculates the sine of an angle in degrees multiplicated by a radius. Returns the result as a float. oslSin and oslCos use the VFPU to compute the result, and thus it's very fast and very precise.
	\param angle
		Angle in degrees (360� means a full circle)
	\param dist
		Radius of the circle
	\return
		sin(angle * PI / 180) * dist

\code
//Character coordinates
float cX, cY;
//Character speed
float cSpeed;
//Character angle
float cAngle;

//Move the character depending on its instant speed and angle
cX += oslCos(cAngle, cSpeed);
//In mathematics, positive y coordinates are directed upwards, in informatics it's directed
//downwards, as if we were writing on a sheet of paper, so we need to substract the sine.
cY -= oslSin(cAngle, cSpeed);
\endcode
*/
extern float oslSin(float angle, float dist);
/** Calculates the cosine of an angle in degrees multiplicated by a radius. Returns the result as a float.
	\param angle
		Angle in degrees (360� means a full circle)
	\param dist
		Radius of the circle
	\return
		cos(angle * PI / 180) * dist
*/
extern float oslCos(float angle, float dist);

/** Returns the sine of an angle in degrees (0 to 360) multiplicated by an integer radius. Returns the result as an integer. These routines use a precalculated lookup table to compute the result.
Don't think oslSini and oslCosi are much faster than oslSin and oslCos, it's not the case.

\code
//This code draws a line in a circle which has a radius of 20 pixels. You can play with the angle to move the line.
int angle = 40, radius = 20;
//Draw our line. Same remark as for oslSin applies.
oslDrawLine(240, 136, 240 + oslCosi(angle, radius), 136 - oslSini(angle, radius));
\endcode */
extern int oslSini(int angle, int dist);
/** Returns the cosine of an angle in degrees (0 to 360) multiplicated by an integer radius. Returns the result as an integer. */
extern int oslCosi(int angle, int dist);

/** Returns the next (upper) power of two of a number.

\code
//Prints 256, as 256 = 2^8 (2^7 = 128, too low).
oslPrintf("%i", oslGetNextPower2(200);
\endcode */
extern int oslGetNextPower2(int val);

//Don't access these!
//extern float osl_fsinus[361];
//extern float osl_fcosinus[361];
extern int osl_isinus[361];
extern int osl_icosinus[361];
extern void oslSetupFTrigo();

/** @} */ // end of main_maths

/** @defgroup main_debug Debug console

	Debugging features.
	@{
*/
#ifdef PSP
	/** Prints formatted text to the current position of the cursor. The format system is the same as printf (you can find the description in every good C tutorial or documentation). */
	#define oslPrintf(format...)		({ char __str[1000]; sprintf(__str , ##format); oslConsolePrint(__str); })
	/** Prints formatted text at a specific position on the screen.
\code
//Display the value of a variable on the top-left screen corner
oslPrintf_xy(0, 0, "Frame: %i", frameNumber);
\endcode */
	#define oslPrintf_xy(x,y, str, format...)		({			osl_consolePosX=x, osl_consolePosY=y;		\
																oslPrintf(str,##format); })
#else
	#define oslPrintf(...)		{ char __str[1000]; sprintf(__str , __VA_ARGS__); oslConsolePrint(__str); }
	#define oslPrintf_xy(x,y, ...)		{			osl_consolePosX=x, osl_consolePosY=y;		\
																oslPrintf(__VA_ARGS__); }
#endif

/** Clears the screen (to black) and resets the cursor to the top-left corner of the screen.

\code
oslCls();
oslPrintf("Welcome...\n");
\endcode */
#define oslCls()				 	(oslClearScreen(0), oslMoveTo(0,0))
/** Moves the cursor to a place on the screen. The x, y coordinates are indicated in terms of characters (by default 8x8 pixels, but depends on the current font). */
#define oslMoveTo(x,y)				(osl_consolePosX=x*osl_sceFont->charWidths['0'], osl_consolePosY=y*osl_sceFont->charHeight)

/** @} */ // end of main_debug


/** @defgroup main_misc Miscellaneous

	Miscellaneous.
	@{
*/
/** Does a benchmark action.
	\param startend
		- OSL_BENCH_INIT: Initialize the benchmark (done at beginning by OSLib)
		- OSL_BENCH_START: Start the measure from here
		- OSL_BENCH_END: End of a benchmark
		- OSL_BENCH_GET: Get the value (milliseconds between the call to start and end
		- OSL_BENCH_DISPLAY: Avoid
	\param slot
		0-3: User slots
		4-7: System slots (4 contains the CPU load in percent if you multiply its value by 6 and divide by 1000)

	\return
		The time elapsed between start and end in microseconds.
*/
extern int oslBenchmarkTestEx(int startend, int slot);			//Permet de choisir un slot (0-3: user, 4:7: system)
/** Same as oslBenchmarkTestEx but does a mean of 20 samples before returning a value. */
extern int oslMeanBenchmarkTestEx(int startend, int slot);		//Benchmark syst�me sur une moyenne de 20 �chantillons
/** Does a benchmark in the slot 0. Easier for testing. */
static inline int oslBenchmarkTest(int startend)		{
	return oslBenchmarkTestEx(startend, 0);
}
/** Displays the system benchmark results on the top-left corner of the screen. Useful for debugging: you know that if the first number approaches or exceeds 16.6 then your game has insufficient performance (for 60 fps). */
extern void oslSysBenchmarkDisplay();

//Undocumented
enum {OSL_BENCH_INIT=0, OSL_BENCH_START, OSL_BENCH_END, OSL_BENCH_GET, OSL_BENCH_GET_LAST, OSL_BENCH_DISPLAY};
#define OSL_BENCH_SLOTS 8
#define OSL_BENCH_SAMPLES 20

#ifndef PSP
	extern void Debug(const char *format, ...);
#endif
extern void oslVblankInterrupt(int sub, void *parg);


extern int oslShowSplashScreen1();
extern int oslShowSplashScreen2();

/** Shows one of the predefined splash screens in OldSchool Library.
	\param splashType
		- 1: Shows the OldSchool Library logo. This logo is not very long and can constitute a great introduction to a game. It can be skipped with cross, circle or start buttons.
		- 2: Shows the neoflash logo. Requires and uses the following file: "logo/neoflash.png".

	\return
		1 in case of success, 0 otherwise.

	Call this function just after having initialized OSLib.

	\code
int main(void)
{
	//Init oslib.
	oslInit(0);
	oslInitGfx(OSL_PF_8888, 1);

	//Show our splash screen
	oslShowSplashScreen(1);

	//Begin our game
	[...]
\endcode

	\b Warning: This function tries to load images files located on your memory stick. It will do nothing if they are not present. But if your application is running in kernel mode and you are
	creating an user thread, your thread will not have a default running directory, making this function to always fail! You should always set a working folder (e.g. with sceIoChdir) before calling this
	functions!

	These files can be found in the Resource folder of your OSLib distribution.
*/
static inline int oslShowSplashScreen(int splashType)		{
	if (splashType == 1)
		return oslShowSplashScreen1();
	else if (splashType == 2)
		return oslShowSplashScreen2();
	return 0;
}

static inline int oslShowNeoflashLogo()		{
	return oslShowSplashScreen(2);
}

#ifdef PSP
	#define OSL_UVRAM_BASE ((u8*)0x04000000)
#else
	extern u8 *OSL_UVRAM_BASE;
	#define alloca _alloca
//	void *_alloca(size_t size);
#endif
#define OSL_UVRAM_SIZE	(2 << 20)
#define OSL_UVRAM_END	((u8*)((u32)OSL_UVRAM_BASE + OSL_UVRAM_SIZE))

/** @} */ // end of main_misc


/** @} */ // end of main


// *** Console SONY ***
/*#define oslCls()								pspDebugScreenClear()
#define oslPrintf(str, format...)				pspDebugScreenPrintf(str,##format)
#define oslMoveTo(x,y)							pspDebugScreenSetXY(x,y)
#define oslPrintf_xy(x,y, str, format...)		({			oslMoveTo(x,y);		\
															oslPrintf(str,##format); })*/


//#include "vfpu.h"
#include "oslmath.h"

#include "vram_mgr.h"
#include "VirtualFile.h"
#include "drawing.h"
#include "map.h"
#include "text.h"
#include "messagebox.h"
#include "keys.h"
#include "audio.h"
#include "bgm.h"
#include "usb.h"
#include "dialog.h"
#include "osk.h"
#include "saveload.h"
#include "net.h"
#include "browser.h"
#include "adhoc/pspadhoc.h"
#include "ccc.h"
#include "sfont.h"

#ifdef __cplusplus
}
#endif

#endif
