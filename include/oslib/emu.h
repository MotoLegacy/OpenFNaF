#ifndef EMU_H
#define EMU_H

#define _CRT_SECURE_NO_DEPRECATE

#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "gl/glext.h"

#ifdef __cplusplus
	extern "C"		{
#endif
extern const char *emuWindowTitle;
extern const int emuDebugMode;

#ifdef _DEBUG
	#define PSP_MODULE_INFO(text, zero, one, one2)			const char *emuWindowTitle = text; const int emuDebugMode = 1;
#else
	#define PSP_MODULE_INFO(text, zero, one, one2)			const char *emuWindowTitle = text; const int emuDebugMode = 0;
#endif

extern void emuInit();
extern void emuStartDrawing();
extern void emuInitGfx();

//A checker
#define PSP_MAIN_THREAD_ATTR(value) int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		\
												{	emuInit();	}


#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
#define GL_RESCALE_NORMAL                 0x803A
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed __int64 s64;
typedef unsigned __int64 u64;

enum {GU_PSM_5650=0, GU_PSM_5551, GU_PSM_4444, GU_PSM_8888, GU_PSM_T4, GU_PSM_T8};
enum {GU_NEAREST=GL_NEAREST, GU_LINEAR=GL_LINEAR};
enum {GU_FLAT=GL_FLAT, GU_SMOOTH=GL_SMOOTH};

/* States */
#define GU_ALPHA_TEST		GL_ALPHA_TEST
#define GU_DEPTH_TEST		GL_DEPTH_TEST
#define GU_SCISSOR_TEST		GL_SCISSOR_TEST
#define GU_STENCIL_TEST		GL_STENCIL_TEST
#define GU_BLEND			GL_BLEND
#define GU_CULL_FACE		GL_CULL_FACE
#define GU_DITHER			GL_DITHER
#define GU_FOG				GL_FOG
#define GU_CLIP_PLANES		GL_CLIP_PLANES
#define GU_TEXTURE_2D		GL_TEXTURE_2D
#define GU_LIGHTING			GL_LIGHTING
#define GU_LIGHT0			GL_LIGHT0
#define GU_LIGHT1			GL_LIGHT1
#define GU_LIGHT2			GL_LIGHT2
#define GU_LIGHT3			GL_LIGHT3
#define GU_UNKNOWN_15		(-15)
#define GU_UNKNOWN_16		(-16)
#define GU_COLOR_TEST		(-17)
#define GU_COLOR_LOGIC_OP	(-18)
#define GU_FACE_NORMAL_REVERSE	(-19)
#define GU_PATCH_FACE		(-20)
#define GU_FRAGMENT_2X		(-21)

/* Primitive types */
#define GU_POINTS			(-1)
#define GU_LINES			GL_LINES
#define GU_LINE_STRIP		GL_LINE_STRIP
#define GU_TRIANGLES		GL_TRIANGLES
#define GU_TRIANGLE_STRIP	GL_TRIANGLE_STRIP
#define GU_TRIANGLE_FAN		GL_TRIANGLE_FAN
#define GU_SPRITES			(-6)

/* Test Function */
#define GU_NEVER		GL_NEVER
#define GU_ALWAYS		GL_ALWAYS
#define GU_EQUAL		GL_EQUAL
#define GU_NOTEQUAL		GL_NOTEQUAL
#define GU_LESS			GL_LESS
#define GU_LEQUAL		GL_LEQUAL
#define GU_GREATER		GL_GREATER
#define GU_GEQUAL		GL_GEQUAL

/* Clear Buffer Mask */
#define GU_COLOR_BUFFER_BIT		GL_COLOR_BUFFER_BIT
#define GU_STENCIL_BUFFER_BIT	GL_STENCIL_BUFFER_BIT
#define GU_DEPTH_BUFFER_BIT		GL_DEPTH_BUFFER_BIT

/* Texture Effect */
#define GU_TFX_MODULATE		(0)
#define GU_TFX_DECAL		(1)
#define GU_TFX_BLEND		(2)
#define GU_TFX_REPLACE		(3)
#define GU_TFX_ADD		(4)

/* Texture Color Component */
#define GU_TCC_RGB		(0)
#define GU_TCC_RGBA		(1)

/* Blending Op */
#define GU_ADD			GL_FUNC_ADD
#define GU_SUBTRACT		GL_FUNC_SUBTRACT
#define GU_REVERSE_SUBTRACT	GL_FUNC_REVERSE_SUBTRACT
#define GU_MIN			GL_MIN
#define GU_MAX			GL_MAX
#define GU_ABS			(-5)

/* Blending Factor */
#define GU_SRC_COLOR			GL_SRC_COLOR
#define GU_ONE_MINUS_SRC_COLOR	GL_ONE_MINUS_SRC_COLOR
#define GU_SRC_ALPHA			GL_SRC_ALPHA
#define GU_ONE_MINUS_SRC_ALPHA	GL_ONE_MINUS_SRC_ALPHA
#define GU_DST_COLOR			GL_DST_COLOR
#define GU_ONE_MINUS_DST_COLOR	GL_ONE_MINUS_DST_COLOR
#define GU_DST_ALPHA			GL_DST_ALPHA
#define GU_ONE_MINUS_DST_ALPHA	GL_ONE_MINUS_DST_ALPHA
#define GU_FIX					GL_CONSTANT_COLOR

/* Vertex Declarations Begin */
#define GU_TEXTURE_SHIFT(n)	((n)<<0)
#define GU_TEXTURE_8BIT		GU_TEXTURE_SHIFT(1)
#define GU_TEXTURE_16BIT	GU_TEXTURE_SHIFT(2)
#define GU_TEXTURE_32BITF	GU_TEXTURE_SHIFT(3)
#define GU_TEXTURE_BITS		GU_TEXTURE_SHIFT(3)

#define GU_COLOR_SHIFT(n)	((n)<<2)
#define GU_COLOR_RES1		GU_COLOR_SHIFT(1)
#define GU_COLOR_RES2		GU_COLOR_SHIFT(2)
#define GU_COLOR_RES3		GU_COLOR_SHIFT(3)
#define GU_COLOR_5650		GU_COLOR_SHIFT(4)
#define GU_COLOR_5551		GU_COLOR_SHIFT(5)
#define GU_COLOR_4444		GU_COLOR_SHIFT(6)
#define GU_COLOR_8888		GU_COLOR_SHIFT(7)
#define GU_COLOR_BITS		GU_COLOR_SHIFT(7)

#define GU_NORMAL_SHIFT(n)	((n)<<5)
#define GU_NORMAL_8BIT		GU_NORMAL_SHIFT(1)
#define GU_NORMAL_16BIT		GU_NORMAL_SHIFT(2)
#define GU_NORMAL_32BITF	GU_NORMAL_SHIFT(3)
#define GU_NORMAL_BITS		GU_NORMAL_SHIFT(3)

#define GU_VERTEX_SHIFT(n)	((n)<<7)
#define GU_VERTEX_8BIT		GU_VERTEX_SHIFT(1)
#define GU_VERTEX_16BIT		GU_VERTEX_SHIFT(2)
#define GU_VERTEX_32BITF	GU_VERTEX_SHIFT(3)
#define GU_VERTEX_BITS		GU_VERTEX_SHIFT(3)

#define GU_WEIGHT_SHIFT(n)	((n)<<9)
#define GU_WEIGHT_8BIT		GU_WEIGHT_SHIFT(1)
#define GU_WEIGHT_16BIT		GU_WEIGHT_SHIFT(2)
#define GU_WEIGHT_32BITF	GU_WEIGHT_SHIFT(3)
#define GU_WEIGHT_BITS		GU_WEIGHT_SHIFT(3)

#define GU_INDEX_SHIFT(n)	((n)<<11)
#define GU_INDEX_8BIT		GU_INDEX_SHIFT(1)
#define GU_INDEX_16BIT		GU_INDEX_SHIFT(2)
#define GU_INDEX_BITS		GU_INDEX_SHIFT(3)

#define GU_WEIGHTS(n)		((((n)-1)&7)<<14)
#define GU_WEIGHTS_BITS		GU_WEIGHTS(8)
#define GU_VERTICES(n)		((((n)-1)&7)<<18)
#define GU_VERTICES_BITS	GU_VERTICES(8)

#define GU_TRANSFORM_SHIFT(n)	((n)<<23)
#define GU_TRANSFORM_3D		GU_TRANSFORM_SHIFT(0)
#define GU_TRANSFORM_2D		GU_TRANSFORM_SHIFT(1)
#define GU_TRANSFORM_BITS	GU_TRANSFORM_SHIFT(1)
/* Vertex Declarations End */

/* Contexts */
#define GU_DIRECT		(0)
#define GU_CALL			(1)
#define GU_SEND			(2)

typedef int SceSize;
//typedef int size_t;
typedef long SceUID;
typedef unsigned long SceMode;

/** Permission value for the sceIoAssign function */
enum IoAssignPerms
{
	/** Assign the device read/write */
	IOASSIGN_RDWR = 0,
	/** Assign the device read only */
	IOASSIGN_RDONLY = 1
};

/* Note: Not all of these sceIoOpen() flags are not compatible with the
   open() flags found in sys/unistd.h. */
#define PSP_O_RDONLY	0x0001
#define PSP_O_WRONLY	0x0002
#define PSP_O_RDWR	(PSP_O_RDONLY | PSP_O_WRONLY)
#define PSP_O_NBLOCK	0x0004
#define PSP_O_DIROPEN	0x0008	// Internal use for dopen
#define PSP_O_APPEND	0x0100
#define PSP_O_CREAT	0x0200
#define PSP_O_TRUNC	0x0400
#define	PSP_O_EXCL	0x0800
#define PSP_O_NOWAIT	0x8000

#define PSP_SEEK_SET	0
#define PSP_SEEK_CUR	1
#define PSP_SEEK_END	2

/* Wrap Mode */
#define GU_REPEAT		(0)
#define GU_CLAMP		(1)

/** Returned controller data */
typedef struct SceCtrlData {
	/** The current read frame. */
	unsigned int 	TimeStamp;
	/** Bit mask containing zero or more of ::PspCtrlButtons. */
	unsigned int 	Buttons;
	/** Analogue stick, X axis. */
	unsigned char 	Lx;
	/** Analogue stick, Y axis. */
	unsigned char 	Ly;
	/** Reserved. */
	unsigned char 	Rsrv[6];
} SceCtrlData;

enum PspCtrlButtons
{
	/** Select button. */
	PSP_CTRL_SELECT     = 0x000001,
	/** Start button. */
	PSP_CTRL_START      = 0x000008,
	/** Up D-Pad button. */
	PSP_CTRL_UP         = 0x000010,
	/** Right D-Pad button. */
	PSP_CTRL_RIGHT      = 0x000020,
	/** Down D-Pad button. */
	PSP_CTRL_DOWN      	= 0x000040,
	/** Left D-Pad button. */
	PSP_CTRL_LEFT      	= 0x000080,
	/** Left trigger. */
	PSP_CTRL_LTRIGGER   = 0x000100,
	/** Right trigger. */
	PSP_CTRL_RTRIGGER   = 0x000200,
	/** Triangle button. */
	PSP_CTRL_TRIANGLE   = 0x001000,
	/** Circle button. */
	PSP_CTRL_CIRCLE     = 0x002000,
	/** Cross button. */
	PSP_CTRL_CROSS      = 0x004000,
	/** Square button. */
	PSP_CTRL_SQUARE     = 0x008000,
	/** Home button. */
	PSP_CTRL_HOME       = 0x010000,
	/** Hold button. */
	PSP_CTRL_HOLD       = 0x020000,
	/** Music Note button. */
	PSP_CTRL_NOTE       = 0x800000,
};

typedef int (*SceKernelThreadEntry )(SceSize args, void *argp);
typedef unsigned int SceUInt;
typedef struct SceKernelThreadOptParam		{
	SceSize size;
	SceUID stackMpid;
} SceKernelThreadOptParam;


void emuInitGL();
void emuInitEmulation();
void emuExit();

float vfpu_sini(int f1, int f2);
float vfpu_cosi(int f1, int f2);

void sceKernelDcacheWritebackInvalidateRange(void *p, int n);
void sceKernelDcacheWritebackRange(void *p, int n);
void sceKernelDcacheWritebackInvalidateAll();
void sceGuDrawArray(int prim, int vtype, int count, const void* indices, const void* vertices);
//void *memalign(size_t a, size_t b);
//int sceDmacMemcpy(void *dest, const void *source, unsigned int size);
void sceGuEnable(int state);
void sceGuDisable(int state);
void sceGuAlphaFunc(int func, int value, int mask);
void sceGuAmbientColor(unsigned int color);
void sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix);
void sceGuTexFunc(int tfx, int tcc);
void sceGuColorFunc(int func, unsigned int color, unsigned int mask);
void* sceGuGetMemory(int size);
void sceGuTexImage(int mipmap, int width, int height, int tbw, const void* tbp);
void sceGuTexMode(int tpsm, int maxmips, int a2, int swizzle);
void sceGuClutLoad(int num_blocks, const void* cbp);
void sceGuClutMode(unsigned int cpsm, unsigned int a1, unsigned int a2, unsigned int a3);
void sceGuStart(int cid, void* list);
int sceGuSync(int mode, int a1);
int sceGuFinish(void);
void sceGuDispBuffer(int width, int height, void* dispbp, int dispbw);
void sceDisplayWaitVblankStart();
int sceGuDisplay(int state);
void sceGuShadeModel(int mode);
void sceGuTexFilter(int min, int mag);
void sceGuScissor(int x, int y, int w, int h);
void sceGuDrawBuffer(int psm, void* fbp, int fbw);
void sceGuInit(void);
void pspDebugScreenInit();
void* sceGuSwapBuffers(void);
void sceGuClearColor(unsigned int color);
void sceGuClear(int flags);
void sceGuTerm();
void sceKernelExitGame();
SceUID sceIoOpen(const char *file, int flags, SceMode mode);
int sceIoClose(SceUID fd);
int sceIoRead(SceUID fd, void *data, SceSize size);
int sceIoWrite(SceUID fd, const void *data, SceSize size);
int sceCtrlSetSamplingCycle(int cycle);
int sceCtrlSetSamplingMode(int mode);
int sceCtrlPeekBufferPositive(SceCtrlData *pad_data, int count);
void emuGlReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void gettimeofday(struct timeval *tv, int unused);
void emuConfigure2DTransfer(int enable);
int sceIoLseek32(SceUID  fd, int  offset, int  whence);
int sceAudioChReserve (int channel, int samplecount, int format);
int sceAudioChRelease (int channel);
int sceAudioOutputPannedBlocking (int channel, int leftvol, int rightvol, void *buffer);
int sceAudioChangeChannelConfig (int channel, int format);
SceUID sceKernelCreateThread(const char *name, SceKernelThreadEntry entry, int initPriority, int stackSize, SceUInt attr, SceKernelThreadOptParam *option);
int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp);
int sceKernelWaitThreadEnd(SceUID thid, SceUInt *timeout);
int sceKernelDeleteThread(SceUID thid);
int sceKernelExitDeleteThread(int status);
void sceGuTexWrap(int u, int v);

#define EMU_NB_TEXTURES 1
extern GLuint emuGlTexture[EMU_NB_TEXTURES];
extern u32 emuCurrentTexSizeX, emuCurrentTexSizeY, emuCurrentTextureState, emuCurrentAmbientColor;
extern const int emu_pixelPhysFormats[];
extern int emuReuseTextures;
extern int emuUsePixelShaders;
extern int emuFrameSkipMode;
extern int emuPauseGame;

#ifdef __cplusplus
}
#endif

#endif
