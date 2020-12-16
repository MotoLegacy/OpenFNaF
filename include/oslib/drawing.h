#ifndef _OSL_DRAWING_H_
#define _OSL_DRAWING_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifndef PSP
	#undef RGB
#endif

/** Boolean type */
#ifndef __cplusplus
#define bool char
#endif







/** @defgroup drawing Drawing

    Drawing in OSLib.
	@{
*/

/** \ingroup drawing_color True color */
typedef unsigned long OSL_COLOR;


/** @defgroup drawing_main Main

	Main and configuration routines of the drawing part of OSLib.
	@{
*/

/** Initializes the graphical part of OSLib.
	\param pixelFormat
		Defines the screen resolution.
			- OSL_PF_8888: 32-bit render, very precise and nice, especially when it comes to gradients. However requires twice the memory required by the 16-bit mode (1088 kilobytes in double buffer
			mode, half in single buffer mode). Also uses more bandwidth and thus is slower.
			- OSL_PF_5650: 16-bit render, can only display 65 thousand colors instead of 16 millions. Requires 544 kB in double buffer mode, and half in single buffer mode. It's the recommended mode.
			Use oslSetDithering to simulate more colors with dithering.
	\param bDoubleBuffer
		A game will always use double buffering (else flickering will appear), let it to TRUE (1) except if you really know what you are doing. Look at oslSwapBuffers for more information.

	Note: you can call it as many times as you want. However, any images placed in video memory (OSL_IN_VRAM) will be invalidated after a call to oslInitGfx, so make sure to delete all of them before,
	else you'll encounter (very) weird and random problems!
*/
extern void oslInitGfx(int pixelFormat, int bDoubleBuffer);

/** Starts drawing with OSLib. Call this before trying to draw anything. */
extern void oslStartDrawing();
/** Ends drawing. Call this once you've finished your render. This command will wait for the GPU to finish its render, and thus can be slow. Though, OSLib can use this remaining time to render and
output sound. */
extern void oslEndDrawing();
/** oslSyncDrawing waits for the GPU (that's how I'm calling the graphic processor; some say GE, meaning Graphic Engine) rendering to finish.
As rendering is done in parallel with the CPU, you may be modifying an image which is currently used by the GPU, and thus render will be
incorrect. In this case you should first issue a oslSyncDrawing to be sure the GPU is not active anymore and modify what you want after that. Howerver, take note that synchronizing with the GPU waits
that it ends everythings it's doing, and thus can be VERY slow, so avoid doing it except if you really need it and if you know what you are doing.

Note: The amount of things you can draw between a call to oslStartDrawing and oslEndDrawing is limited because commands are sent to the GPU, and they are stored in the so called display list, which has
a limited size. If you overflow the display list size, your program will crash (or have very weird behaviour). oslSyncDrawing waits the GPU to finish to execute the display list and resets it to
zero (as oslStartDrawing / oslEndDrawing do: oslSyncDrawing is just a faster way to call both one after the other, it just skips end and beginning of frame processes).
So, if you are really drawing a lot of things at once and your program is crashing, try to put a oslSyncDrawing somewhere. */
extern void oslSyncDrawing();

/** Swaps buffers. In double buffering mode, call it once per frame.
The utility of double buffering is to avoid flickering, because if you only have one buffer, the buffer displayed on the screen is the same which you are drawing to.
And thus the screen will show everything you're making with the buffer! So let's say you first draw a sky and then a sprite over it, the user will first see the sky and the
sprite flickering.

With double buffering though you have one buffer which is displayed, and another (hidden) you're drawing to. Once rendering to the hidden buffer is terminated, you can show it to the user: this is
swapping buffers. The user will now see the hidden buffer (which is complete now) and the other buffer will now become hidden so that you can draw another frame to it, and so on.
*/
extern void oslSwapBuffers();

/** Ends the graphical part, releasing memory for the display list. All images placed in video memory (OSL_IN_VRAM) become invalid after calling this, so make sure to release ALL of them before! You will
need to reload them after having called oslInitGfx again. */
extern void oslEndGfx();

/** Very advanced command which sets the display list size. Don't use it unless you know what you are doing. Default is 1 MB (megabyte). Don't forget to call oslStartDrawing after a call to this
function before beginning to draw again. */
extern void oslSetSysDisplayListSize(int newSize);

/** @} */ // end of drawing_main


/** @defgroup drawing_general General

	General drawing tasks.
	@{
*/

/** Clears the entire screen with the specified color.
	\param backColor
		Color to fill the screen with.
*/
extern void oslClearScreen(int backColor);

/** Sets the clipping region. Anything outside of the clipping rectangle will not be drawn on the screen. Initially the clipping region is set to the entire screen.

Note: oslSetDrawBuffer automatically adjusts the clipping region to cover the full the drawbuffer image. */
extern void oslSetScreenClipping(int x0, int y0, int x1, int y1);

/** Sets the depth test. */
extern void oslSetDepthTest(int enabled);

/** Resets the screen clipping to the whole screen. */
#define oslResetScreenClipping()					oslSetScreenClipping(0, 0, osl_curBuf->sizeX, osl_curBuf->sizeY);


/** Sets current special alpha effect parameters. Images and shapes drawn next to this command will be using these parameters.
	\param effect
		Can be one of the following:
		- OSL_FX_NONE: Blending is purely disabled.
		- OSL_FX_DEFAULT: Reverts to default mode (RGBA).
		- OSL_FX_RGBA: The alpha value of images or colors is taken in account for blending. For example, drawing a rectangle with the following color: RGBA(0, 0, 0, 128) will draw it semi-transparent.
		- OSL_FX_ALPHA: Regular alpha blending: dstColor = dstColor = srcColor * coeff1 + dstColor * (1 - coeff1).
		- OSL_FX_ADD: Color addition: dstColor = dstColor * coeff2 + srcColor * coeff1. With oslSetAlpha, coeff2 is not supplied and set to full (255) by default.
		- OSL_FX_SUB: Color substraction: dstColor = dstColor * coeff2 - srcColor * coeff1. Please note that substracting colors produce the invert effect that you'd expect to see: substracting red
		(255, 0, 0) from a white image (255, 255, 255) for example gives cyan output (0, 255, 255).
		Furthermore, you can OR the value with the following:
		- OSL_FX_COLOR: Coeff1 and coeff2 are treated as 32-bit colors instead of 8-bit alpha values. Multiplication between a color and another gives a tinted color.
	\param coeff1
		Coefficient used in the operations described above. Either alpha coefficient (0 to 255) or color (0 to 0xffffffff)
	\param coeff2
		Extended coefficient used in the operations described above.

Here are some examples:
\code
//Default alpha
oslSetAlpha(OSL_FX_DEFAULT, 0);
//Tint an image with red
oslSetAlpha(OSL_FX_TINT, RGBA(255, 0, 0, 255));
//The image will be drawn semi-transparent.
oslSetAlpha(OSL_FX_ALPHA, 128);
//Tinted + semi-transparent image
oslSetAlpha(OSL_FX_ALPHA | OSL_FX_COLOR, RGBA(255, 0, 0, 128));
//Simple addition as on the Super Nintendo
oslSetAlpha(OSL_FX_ADD, 255);
//dst = 0.5 * src + 0.5 * dst
oslSetAlpha2(OSL_FX_ADD, 128, 128);
\endcode
That's not all, a lot of nice effects can be produced with this function.

About multiplication, note that black won't be tinted as it's a multiplication, zero * something always gives zero. In fact, a multiplication always gives a darker color.
Each separate color component is first divided by 255 and then multiplicated with the other one. For example, let's multiply gray RGB(128, 128, 128) by orange RGB(255, 128, 0):
- r1 is 128 (red for the first component)
- g1 is 128 (green for the first component)
- b1 is 128 (...)
- r2 is 255 (red for the second component)
- g2 is 128 (...)
- b2 is 0

By ranging them to 1.0, we get r1 =0.5, g1 = 0.5, b1 = 0.5, r2 = 1.0, g2 = 0.5, b2 = 0.0. We now multiply r1 by r2, g1 by g2 and b1 by b2. The final result is:
RGB(0.5, 0.25, 0.0), which is, when converted again RGB(128, 64, 0), brown! In general, multiplicating by gray always darkens the color, multiplying by white does nothing, and multiplying by black gives
black. You can get some nice effects once you've understood correctly what color operations do. */
extern void oslSetAlpha2(u32 effect, u32 coeff1, u32 coeff2);

/** See oslSetAlpha2. */
static inline void oslSetAlpha(u32 effect, u32 coeff1)			{
	oslSetAlpha2(effect, coeff1, 0xffffffff);
}


/** Structure containing a special alpha effect data. */
typedef struct		{
	int effect;								//!< Current alpha special effect
	int coeff1;								//!< First coefficient
	int coeff2;								//!< Second coefficient
} OSL_ALPHA_PARAMS;

extern int osl_currentAlphaEffect;
extern OSL_COLOR osl_currentAlphaCoeff, osl_currentAlphaCoeff2;

/** Stores the current alpha parameters to an OSL_ALPHA_PARAMS structure. */
static inline void oslGetAlphaEx(OSL_ALPHA_PARAMS *alpha)		{
	alpha->effect = osl_currentAlphaEffect;
	alpha->coeff1 = osl_currentAlphaCoeff;
	alpha->coeff2 = osl_currentAlphaCoeff2;
}

/** Sets the current alpha parameters using an OSL_ALPHA_PARAMS structure. */
static inline void oslSetAlphaEx(OSL_ALPHA_PARAMS *alpha)		{
	oslSetAlpha2(alpha->effect, alpha->coeff1, alpha->coeff2);
}


/** Enables (1) or disables (0) bilinear filtering.

Bilinear filtering "smoothes" images, reducing the edge aliasing when you stretch or rotate them. However for text and unstretched images, smoothing creates a blurry effect, so avoid enabling it
permanently. */
extern void oslSetBilinearFilter(int enabled);
/** Enables (1) or disables (0) dithering. If you are using the 16-bit mode (as specified with oslInitGfx) or you are drawing to a 16-bit image, you can enable dithering. If a color can't be rendered
exactly, it will use the nearest available colors and blend them together (with small dots) to give the illusion of an intermediate color. For example, dithering red and yellow gives the illusion of
an orange surface. */
extern void oslSetDithering(int enabled);

/** Enables color keying. Any pixel of the specified color will not be drawn to the screen.

It has not much interest in itself, but the fact is that it takes effect as well when loading images. Any color equal to this one is set as transparent. As transparency is defined by the alpha channel,
you can disable color keying then and the image will still contain transparency.

Example 1: loading an image with a color key. You can often see that the background of some sprite images are of a bright and unconventional color, like pink or bright red. This color is not used
usually, but is only there to define the transparent parts of the image. Our example uses bright pink as a transparent color:
\code
//Bright pink pixels will be transparent
oslSetTransparentColor(RGB(255, 0, 255));
//Load the image. Take care to select a pixel format which contains alpha. Every pixel format does, except 5650.
OSL_IMAGE *img = oslLoadImageFilePNG("test.png", OSL_IN_RAM, OSL_PF_5551);
//Load some other images, with pink as transparent...
//After loading images, disable color keying else we won't be able to display any pink pixels, and the render will appear messed up...
oslDisableTransparentColor();
\endcode

Example 2: Enable this when drawing (as said, it has nearly no interest in itself):
\code
oslSetTransparentColor(RGB(255, 0, 255));
//You'd expect this command to do something? In fact it won't as the rectangle is pink and pink is currently masked out...
oslDrawFillRect(0, 0, 100, 100, RGB(255, 0, 255));
\endcode
*/
extern void oslSetTransparentColor(OSL_COLOR color);
extern void oslDisableTransparentColor();


/** Holds whether bilinear filetering is currently turned on. */
extern int osl_bilinearFilterEnabled;
/** Holds whether dithering is currently turned on. */
extern int osl_ditheringEnabled;
/** Holds whether color keying is enabled. */
extern int osl_colorKeyEnabled;

//Undocumented (see oslSetAlpha for more information).
enum {OSL_FX_NONE=0, OSL_FX_FLAT, OSL_FX_ALPHA, OSL_FX_ADD, OSL_FX_SUB};
#define OSL_FX_DEFAULT OSL_FX_RGBA
#define OSL_FX_OPAQUE OSL_FX_NONE
#define OSL_FX_TINT				(OSL_FX_ALPHA | OSL_FX_COLOR)
//Cumulable avec les autres pour définir l'utilisation du canal alpha.
#define OSL_FX_RGBA 0x100
//Coefficients alpha à trois canaux
#define OSL_FX_COLOR 0x1000


//Don't access them!
//extern int osl_currentResolutionBPP;
//Couleur transparente utilisée au chargement des images.
extern int osl_colorKeyValue;

/** @} */ // end of drawing_general



/** @defgroup drawing_color Colors

	Color manipulation.
	@{
*/

/** Creates a 32-bit opaque color. */
#define RGB(r,v,b)		((r) | ((v)<<8) | ((b)<<16) | (0xff<<24))
/** Creates a 32-bit color. 32-bit colors is what all OSLib calls asking for a color want. Use RGB if you don't want to bother about alpha (semi transparency). */
#define RGBA(r,v,b,a)	((r) | ((v)<<8) | ((b)<<16) | ((a)<<24))
/** Creates a 12-bit color with 3 coefficients (red, green, blue). Alpha is set to the maximum (opaque). The r, v, b values can be from 0 to 255, they're divided to match the 12-bit pixel format. */
#define RGB12(r,v,b)	((((b)>>4)<<8) | (((v)>>4)<<4) | ((r)>>4) | (0xf<<12))
/** Creates a 12-bit color with 4 coefficients (red, green, blue, alpha). */
#define RGBA12(r,v,b,a)	((((a)>>4)<<12) | (((b)>>4)<<8) | (((v)>>4)<<4) | ((r)>>4))
/** Creates a 15-bit opaque color. */
#define RGB15(r,v,b)	((((b)>>3)<<10) | (((v)>>3)<<5) | ((r)>>3) | (1<<15))
/** Creates a 15-bit color with alpha. */
#define RGBA15(r,v,b,a)	((((a)>>7)<<15) | (((b)>>3)<<10) | (((v)>>3)<<5) | ((r)>>3))
/** Creates a 16-bit color. */
#define RGB16(r,v,b)	((((b)>>3)<<11) | (((v)>>2)<<5) | ((r)>>3))

/** Gets R, G, B, A (Red, Green, Blue, Alpha) component values from a 32-bit color. Stores the 8-bit values to the variables indicated by the 2nd to 5th argument.
	Example:
	\code
u32 color = RGBA(1, 2, 3, 4);
u8 red, green, blue, alpha;

oslRgbaGet8888(color, red, green, blue, alpha);

oslPrintf("%i %i %i %i", red, green, blue, alpha);
	\endcode

	This will print 1 2 3 4.
*/
#define oslRgbaGet8888(data, r, g, b, a)		((r)=((data)&0xff), (g)=(((data)>>8)&0xff), (b)=(((data)>>16)&0xff), (a)=(((data)>>24)&0xff))
/** Gets R, G, B, A values from a 4444 color. The values are always 8 bit, so they are expanded with a multiplication!
	Example:
	\code
u32 color = RGBA12(255, 128, 0, 1);
u8 red, green, blue, alpha;

oslRgbGet4444(color, red, green, blue, alpha);

oslPrintf("%i %i %i %i", red, green, blue, alpha);
	\endcode

	This will print 240 128 0 0. The alpha was 1 but was lost because of precision in 12-bit mode (it can only store coefficients from 0 to 15 instead of 0 to 255). The red color was 255 but it has lost
	precision due to 12-bit conversion. It has passed from 255 to 15 (integer division by 16), and then was multiplicated by 16, making it 240 instead of 255. This can be problematic because you will no
	more get bright and vibrant color after conversion. However you can use the 'f' alternate routines which give more precise results (but they are slightly slower), in this case the result of
	oslRgbaGet4444f would be 255 136 0 0, which is brighter and nicer.
*/
#define oslRgbaGet4444(data, r, g, b, a)		((r)=((data)&0xf)<<4, (g)=(((data)>>4)&0xf)<<4, (b)=(((data)>>8)&0xf)<<4, (a)=(((data)>>12)&0xf)<<4)
/** Get components from a 5551 (15-bit) color. Same remarks as oslRgbGet4444 apply. */
#define oslRgbaGet5551(data, r, g, b, a)		((r)=((data)&0x1f)<<3, (g)=(((data)>>5)&0x1f)<<3, (b)=(((data)>>10)&0x1f)<<3, (a)=(((data)>>15)&0x1)<<7)
/** Get components from a 5650 (16-bit) color. Same remarks as oslRgbGet4444 apply. */
#define oslRgbGet5650(data, r, g, b)			((r)=((data)&0x1f)<<3, (g)=(((data)>>5)&0x3f)<<2, (b)=(((data)>>11)&0x1f)<<3)

//Précises - vieilles et lentes
/*#define oslRgbGet5650f(data, r, g, b)			((r)=(((data)&0x1f)*255)/31, (g)=((((data)>>5)&0x3f)*255)/63, (b)=((((data)>>11)&0x1f)*255)/31)
#define oslRgbaGet5551f(data, r, g, b, a)		((r)=(((data)&0x1f)*255)/31, (g)=((((data)>>5)&0x1f)*255)/31, (b)=((((data)>>10)&0x1f)*255)/31, (a)=(((data)>>15)&0x1)*255)
#define oslRgbaGet4444f(data, r, g, b, a)		((r)=(((data)&0xf)*255)/15, (g)=((((data)>>4)&0xf)*255)/15, (b)=((((data)>>8)&0xf)*255)/15, (a)=((((data)>>12)&0xf)*255)/15)*/

//Encore autre chose...
/*#define oslRgbGet5650f(data, r, g, b)			((r)=(((data)&0x1f)*264)>>5, (g)=((((data)>>5)&0x3f)*260)>>6, (b)=((((data)>>11)&0x1f)*264)>>5)
#define oslRgbaGet5551f(data, r, g, b, a)		((r)=(((data)&0x1f)*264)>>5, (g)=((((data)>>5)&0x1f)*264)>>5, (b)=((((data)>>10)&0x1f)*264)>>5, (a)=(((data)>>15)&0x1)*255)
#define oslRgbaGet4444f(data, r, g, b, a)		((r)=(((data)&0xf)*272)>>4, (g)=((((data)>>4)&0xf)*272)>>4, (b)=((((data)>>8)&0xf)*272)>>4, (a)=((((data)>>12)&0xf)*272)>>4)*/

//Meilleur compromis taille - vitesse
/** Look at oslRgbaGet4444 to find out why these are useful. */
#define oslRgbaGet4444f(data, r, g, b, a)		((r)=((data)&0xf)<<4 | ((data)&0xf), (g)=(((data)>>4)&0xf)<<4 | (((data)>>4)&0xf), (b)=(((data)>>8)&0xf)<<4 | (((data)>>8)&0xf), (a)=(((data)>>12)&0xf)<<4 | (((data)>>12)&0xf))
/** Same remark as oslRgbaGet4444f. */
#define oslRgbaGet5551f(data, r, g, b, a)		((r)=((data)&0x1f)<<3 | ((data)&0x1f)>>2, (g)=(((data)>>5)&0x1f)<<3 | (((data)>>5)&0x1f)>>2, (b)=(((data)>>10)&0x1f)<<3 | (((data)>>10)&0x1f)>>2, (a)=(((data)>>15)&0x1)*255)
/** Same remark as oslRgbaGet4444f. */
#define oslRgbGet5650f(data, r, g, b)			((r)=((data)&0x1f)<<3 | ((data)&0x1f)>>2, (g)=(((data)>>5)&0x3f)<<2 | (((data)>>5)&0x3f)>>4, (b)=(((data)>>11)&0x1f)<<3 | (((data)>>10)&0x1f)>>2)

/** @} */ // end of drawing_color


/** @defgroup drawing_shapes Shapes

	Formerly untextured geometry.
	@{
*/

/** Draws a line from (x0, y0) to (x1, y1). */
extern void oslDrawLine(int x0, int y0, int x1, int y1, OSL_COLOR color);
/** Draws an empty rectangle from (x0, y0) to (x1, y1). */
extern void oslDrawRect(int x0, int y0, int x1, int y1, OSL_COLOR color);
/** Draws a filled rectangle from (x0, y0) to (x1, y1). */
extern void oslDrawFillRect(int x0, int y0, int x1, int y1, OSL_COLOR color);
/** Draws a gradient rectangle from (x0, y0) to (x1, y1).
	\param x0, y0
		Top-left position of the rectangle.
	\param x1, y1
		Bottom-right position of the rectangle.
	\param c1
		Color of the top-left corner.
	\param c2
		Color of the top-right corner.
	\param c3
		Color of the bottom-left corner.
	\param c4
		Color of the bottom-right corner.
*/
extern void oslDrawGradientRect(int x0, int y0, int x1, int y1, OSL_COLOR c1, OSL_COLOR c2, OSL_COLOR c3, OSL_COLOR c4);

/** @} */ // end of drawing_shapes


/** @defgroup drawing_adv Advanced

	Advanced drawing tasks.
	@{
*/

//Undocumented
extern void oslSwizzleTexture(u8* out, const u8* in, unsigned int width, unsigned int height);
extern void oslUnswizzleTexture(u8* out, const u8* in, unsigned int width, unsigned int height);
extern void oslDrawTile(int u, int v, int x, int y, int tX, int tY);

/** @} */ // end of drawing_adv


/** @} */ // end of drawing




/** @defgroup image Images

	Image support in OSLib.
	@{
*/

#define OSL_PALETTEDATA16 unsigned short __attribute__((aligned(16)))
#define OSL_PALETTEDATA32 unsigned long __attribute__((aligned(16)))

/** Palette type. */
typedef struct			{
	short pixelFormat;						//!< Pixel format of palette entries
	short nElements;						//!< Number of entries
	short location;							//!< Location of the palette (always use OSL_IN_RAM)
	void *data;								//!< Actual (raw) data, representing color entries in their pixel format
} OSL_PALETTE;

/** Constants for the flags member of OSL_IMAGE. */
enum OSL_IMAGE_FLAGS {
	OSL_IMAGE_SWIZZLED = 1,					//!< Image is swizzled
	OSL_IMAGE_COPY = 2,						//!< Image is a copy
	OSL_IMAGE_AUTOSTRIP = 4					//!< Image can be automatically stripped (let it one)
};

/** Structure of an image loaded in memory. */
typedef struct		{
	//protected
	u16 sizeX, sizeY;						//!< Displayable size
	u16 sysSizeX, sysSizeY;					//!< Size aligned to the next power of two
	u16 realSizeX, realSizeY;				//!< Actual buffer size (never write outside of these dimensions!)
	void *data;								//!< Raw image data in memory
	u8 flags;								//!< Special image flags
	int totalSize;							//!< Total image size in bytes
	short location;							//!< Image location (OSL_IN_RAM or OSL_IN_VRAM)
	short pixelFormat;						//!< Image pixel format

	//public
	OSL_PALETTE *palette;					//!< Palette for 4 and 8 bit modes
	u16 frameSizeX, frameSizeY;				//!< Size of a frame in the image

	//Special
	int x, y;								//!< Image positions
	int stretchX, stretchY;					//!< Final image size when drawn (stretched)
	//Morceau
	float offsetX0, offsetY0, offsetX1, offsetY1;		//!< Offset in the texture
	//Rotation
	int centerX, centerY;					//!< Rotation center
	int angle;								//!< Angle (rotation) in degrees
	//Paramètres
//	bool autoStrip;							//!< Automatic stripping (let it one)
} OSL_IMAGE;

/** Places where to place an image. Values for the argument "location" of some loading routines. */
typedef enum OSL_LOCATION_FLAGS {
	OSL_IN_NONE=0,								//!< Doesn't exist
	OSL_IN_VRAM=1,								//!< In VRAM
	OSL_IN_RAM=2,								//!< In RAM
	OSL_LOCATION_MASK=7,						//!< There will probably never be more than 8 locations...
	OSL_SWIZZLED=8,								//!< Directly swizzle image (only works for oslLoadImage[...] functions!)
	OSL_UNSWIZZLED=16							//!< Force no swizzling (oslLoadImage[...])
} OSL_LOCATION_FLAGS;

/** Pixelformats (color types / modes). */
enum OSL_PIXELFORMATS		{
	OSL_PF_5650=GU_PSM_5650,					//!< 16 bits, 5 bits per component, except green which has 6, no alpha
	OSL_PF_5551=GU_PSM_5551,					//!< 15 bits, 5 bits per component, 1 alpha bit
	OSL_PF_4444=GU_PSM_4444,					//!< 12 bits, 4 bits per component, 4 alpha bits
	OSL_PF_8888=GU_PSM_8888,					//!< 32 bits, 8 bits per component, 8 alpha bits
	OSL_PF_4BIT=GU_PSM_T4,						//!< Paletted format, 4 bits (16 colors), cannot be set as drawbuffer
	OSL_PF_8BIT=GU_PSM_T8						//!< Paletted format, 8 bits (256 colors), cannot be set as drawbuffer
};

/** @defgroup image_creation Creating or loading images

	Creating or loading images in OSLib.
	@{
*/

/** Creates an empty image. Please note that the image is initially filled with random values, you should take care to clear it by yourself with oslClearImage!
	\param larg
		Width of the image. The real image size will be aligned to the next power of two and so that an image line contains at least 16 bytes.
	\param haut
		Height of the image. The real image height will be aligned to the next multiple of 8 to make swizzling possible.
	\param location
		Location of the image (either OSL_IN_VRAM or OSL_IN_RAM)
	\param pixelFormat
		Format of pixels on the image. The pixel format directly defines the color type, precision and the room taken by the image. Also, biggest images are drawn slower (same applies to writting when
		they're set as drawbuffer). So, avoid using OSL_PF_8888 because it takes a lot of room and is slower. But the render is nicer of course ;)
	\return
		The return is NULL if the image could not be created! Always check for a NULL pointer else your program will CRASH when you'll try to use that image! (the PSP will turn off by itself after ten
		seconds).

\b Important: The maximum size of an image is 512x512! None of the height or width can exceed 512 (for example 10x600 is invalid, 10x500 is okay).
\b Note: Empty created images are never swizzled automatically, even though the oslSetImageAutoSwizzle parameter is true.
Do NOT pass OSL_SWIZZLED or OSL_UNSWIZZLED to oslCreateImage! */
extern OSL_IMAGE *oslCreateImage(int larg, int haut, short location, short pixelFormat);
/** Deletes an image previously created or loaded. */
extern void oslDeleteImage(OSL_IMAGE *img);

/** Loads an image from a file. You can select its location and pixel format, the rest is done automatically.

	Don't forget to check if the returned image is NULL before using it! In this case, display a gentleful message to the user telling him he has not read the readme and put the wrong files :p

	\b Important: Avoid using this function, as it will automatically detect the format depending on the file extension. This means support for ALL formats will be added to your code, weightening your
	application a lot! Use specific routines instead like oslLoadImageFilePNG, oslLoadImageFileJPG, etc. to spare some precious RAM space.

	\b Important (bis): When loading any image, see considerations with #oslCreateImage, they apply to loaded images as well (namely the 512x512 limit).
*/
extern OSL_IMAGE *oslLoadImageFile(char *filename, int location, int pixelFormat);

/** Loads a PNG file. Same remark as for oslLoadImageFile apply. */
extern OSL_IMAGE *oslLoadImageFilePNG(char *filename, int location, int pixelFormat);
/** Loads a JPG file. Same remark as for oslLoadImageFile apply.

Note: The actual JPG decompressor is very simple, and doesn't support JPG files of a too high quality (it will crash in this case). Avoid using JPG files with a quality factor of
more than 90%. Also, if you let users the choice of using JPG files, you should specify this limitation in your instructions. */
extern OSL_IMAGE *oslLoadImageFileJPG(char *filename, int location, int pixelFormat);
/** Loads a GIF file. Same remark as for oslLoadImageFile apply. */
extern OSL_IMAGE *oslLoadImageFileGIF(char *filename, int location, int pixelFormat);

//Wrapped to by oslSetImageAutoSwizzle.
extern int osl_autoSwizzleImages;

/** Controls whether the images should automatically be swizzled upon loading. You can specify OSL_UNSWIZZLED to get a raw image, which you can then access using the data member of the image.
Enabled by default, but wasn't in the first versions of OSLib.

\code
//Force loading image as unswizzled
oslLoadImageFilePNG("test.png", OSL_IN_RAM | OSL_UNSWIZZLED, OSL_PF_5551);
//Force swizzled
oslLoadImageFilePNG("test.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_5551);
//Will swizzle if osl_autoSwizzleImages is set to true.
oslLoadImageFilePNG("test.png", OSL_IN_RAM, OSL_PF_5551);
\endcode */
static inline void oslSetImageAutoSwizzle(int enabled)		{
	osl_autoSwizzleImages = enabled;
}

/** Controls advanced parameters when creating an image. Don't use it or be prepared to an eventual breaking change in the future.

	Add the following values:
		- 1: Align the image sizes horizontally to a power of two
		- 2: Align the vertical size to a multiple of 8 (mandatory for swizzling)

	Default value: 3.
*/
extern int osl_alignBuffer;

//Undocumented
extern void *oslAllocImageData(OSL_IMAGE *img, int location);
extern void oslFreeImageData(OSL_IMAGE *img);

#define oslAddVramPrefixPtr(adr)			((void*)((int)(adr)|0x04000000))
#define oslRemoveVramPrefixPtr(adr)			((void*)((int)(adr)&(~0x04000000)))

//Don't use
extern int oslImageLocationIsSwizzled(int location);

/** @} */ // end of image_creation


/** @defgroup image_drawing Drawing images

	Image support in OSLib.
	@{
*/

#ifdef PSP
	/** Draws an image at positions px, py. */
	#define oslDrawImageXY(img,px,py)				({ (img)->x=(px), (img)->y=(py); oslDrawImage(img); })
	/** Here for backward compatibility, don't use it. */
	#define oslDrawImageSimpleXY(img,px,py)	({ (img)->x=(px), (img)->y=(py); oslDrawImageSimple(img); })
#else
	#define oslDrawImageXY(img,px,py)				{ (img)->x=(px), (img)->y=(py); oslDrawImage(img); }
	#define oslDrawImageSimpleXY(img,px,py)			{ (img)->x=(px), (img)->y=(py); oslDrawImageSimple(img); }
#endif

/** Draws an image. */
extern void oslDrawImage(OSL_IMAGE *img);
/** Here for backward compatibility, don't use it. */
extern void oslDrawImageSimple(OSL_IMAGE *img);
/* - Original documentation for this routine has been removed because it's useless (limited to 1024 pixels in width) -
Draws a very big image. By default, OSLib can't handle images bigger than 512x512, but it can still load and manipulate them, however the GPU will fail to draw them. This routine will allow you to
draw bigger images, at cost of extra complexity and CPU time. Unfortunately, for now it's limited to a width of 1024 pixels, making it not very useful...

\b Note: Pretty everything is supported (rotation, stretching, image tile, stripping, etc.) but the texture wrapping (#oslSetTextureWrap) is not supported. You should not give offsetXX coordinates outside
of the image rectangle, as results are unpredictable (and may change in the future).

\b Warning: This routine can be EXTREMELY slow, and drawing images bigger than what originally meant by the PSP hardware is NOT recommended. The more pixels the PSP has to draw, the more time the PSP
will need (for example, scaling down a 960x544 image down to 480x272 will still need the PSP GPU to resample ALL 960x544 pixels, and thus being way slower than drawing a regular 480x272 image!!!).
You should always rescale your images to an acceptable size (that is, less than 512x512), at loading for example.
However, if you REALLY know what you are doing, this routine may be useful, so it's there if you need it. Remember to swizzle your images, this greatly improves speed (especially if put in RAM)!

Here is a sample which loads an image and scales it down to 480x272 (the PSP screen size). It's excessively complex because I made it as clean as possible: restoring everything I change, supporting the
special location bits as usual, etc. but if you are writing yours you'll probably not need to do all of this, it's just an example for people who want to know how to write a clean OSLib utility routine.

\code
OSL_IMAGE *loadScaledImage(char *filename, int location, int pixelFormat)		{
	OSL_IMAGE *img = NULL, *temp, *oldDrawBuf = oslGetDrawBuffer();
	int oldBilin = osl_bilinearFilterEnabled, oldDither = osl_ditheringEnabled;

	//Load our image with the maximum precision possible; it's just temporary
	temp = oslLoadImageFile(filename, OSL_IN_RAM, OSL_PF_8888);
	if (temp)			{
		//Create an image in the right pixel format. Location HAS to be VRAM because we'll draw on it.
		img = oslCreateImage(480, 272, OSL_IN_VRAM, pixelFormat);
		if (img)		{
			oslStartDrawing();
			//First clear the image to opaque black
			oslClearImage(img, RGBA(0, 0, 0, 255));
			//We'll draw on that image.
			oslSetDrawBuffer(img);
			//Enable dithering, useful if the pixelFormat is not 8888, because dithering will
			//remove the blocky effect of gradients for example.
			oslSetDithering(1);
			//Use bilinear filter for rescaling
			oslSetBilinearFilter(1);
			//Here, you could add some computation to keep the aspect ratio
			temp->stretchX = 480;
			temp->stretchY = 272;
			//Draw the image, scaled down to the PSP screen size
			oslDrawImageBig(temp);
			//Restore everything back to original values
			oslSetDrawBuffer(oldDrawBuf);
			oslSetBilinearFilter(oldBilin);
			oslSetDithering(oldDither);
			oslEndDrawing();
			//Copy the image to the good place
			//We only keep the location bits (to support something like OSL_IN_RAM | OSL_SWIZZLED)
			oslMoveImageTo(img, (location & OSL_LOCATION_MASK));
			//Finally, swizzle the image for more speed!
			if (location & OSL_SWIZZLED)
				oslSwizzleImage(img);
		}
		oslDeleteImage(temp);
	}
	return img;
} \endcode */
/** Don't use this routine, it will probably do something else in the next OSLib release. */
extern void oslDrawImageBig(OSL_IMAGE *img);

/** @} */ // end of image_drawing


/** @defgroup image_placement Image placement

	Image placement in OSLib.
	@{
*/

#ifdef PSP
	/** Sets the source rectangle of the image to be displayed. Let's imagine you've got a big image (e.g. 128x128). This image contains in fact 16 "sprites" of 32x32 each. You can set it to display
	one of these with:
\code
oslSetImageTile(image, 0, 0, 32, 32);
oslDrawImage(image);
\endcode
	This means "display the part of the image beginning at (0,0) (top-left) and finishing at (32,32) (bottom-right) excluded, rendering a 32x32 sprite. For the next one to the right you'd call
	oslSetImageTile(image, 32, 0, 64, 32); */
	#define oslSetImageTile(img,x0,y0,x1,y1)		({ (img)->offsetX0=(float)x0, (img)->offsetY0=(float)y0, (img)->offsetX1=(float)x1, (img)->offsetY1=(float)y1; (img)->stretchX = oslAbs((int)((float)((img)->offsetX1) - (float)((img)->offsetX0))), (img)->stretchY = oslAbs((int)((float)((img)->offsetY1) - (float)((img)->offsetY0))); })
	/** Same as oslSetImageTile except that x1 and y1 parameters are the size of the image rectangle. In case of 32x32 sprites it would always be 32, 32, and you would just set the x0, y0 coordinates to
	the beginning (top-left corner) of your sprite in the image. */
	#define oslSetImageTileSize(img,x0,y0,x1,y1)		({ (img)->offsetX0=(float)(x0), (img)->offsetY0=(float)(y0), (img)->offsetX1=(float)(x0)+(float)(x1), (img)->offsetY1=(float)(y0)+(float)(y1), (img)->stretchX = (int)(x1), (img)->stretchY = (int)(y1); })
	/** Resets the image rectangle to its full (original) size. */
	#define oslResetImageTile(img)					({ (img)->offsetX0=0, (img)->offsetY0=0, (img)->offsetX1=(img)->sizeX, (img)->offsetY1=(img)->sizeY; (img)->stretchX = (img)->sizeX, (img)->stretchY = (img)->sizeY; })
	/** Mirrors an image (or an image tile) horizontally. Note that mirroring twice reverts the image to its normal orientation. If you are calling this in a loop, don't forget to reset it after the
	image is drawn, else your image will be mirrored once per loop iteration and will appear ugly. */
	#define oslMirrorImageH(img)					({ float ___tmp;  ___tmp = (img)->offsetX0; (img)->offsetX0 = (img)->offsetX1; (img)->offsetX1 = ___tmp; })
	/** Mirrors an image vertically. Same remarks as oslMirrorImageH apply. */
	#define oslMirrorImageV(img)					({ float ___tmp;  ___tmp = (img)->offsetY0; (img)->offsetY0 = (img)->offsetY1; (img)->offsetY1 = ___tmp; })
#else
	#define oslSetImageTile(img,x0,y0,x1,y1)		{ (img)->offsetX0=(float)x0, (img)->offsetY0=(float)y0, (img)->offsetX1=(float)x1, (img)->offsetY1=(float)y1; (img)->stretchX = oslAbs((int)((float)((img)->offsetX1) - (float)((img)->offsetX0))), (img)->stretchY = oslAbs((int)((float)((img)->offsetY1) - (float)((img)->offsetY0))); }
	#define oslSetImageTileSize(img,x0,y0,x1,y1)	{ (img)->offsetX0=(float)(x0), (img)->offsetY0=(float)(y0), (img)->offsetX1=(float)(x0)+(float)(x1), (img)->offsetY1=(float)(y0)+(float)(y1), (img)->stretchX = (int)(x1), (img)->stretchY = (int)(y1); }
	#define oslResetImageTile(img)					{ (img)->offsetX0=0, (img)->offsetY0=0, (img)->offsetX1=(img)->sizeX, (img)->offsetY1=(img)->sizeY; (img)->stretchX = (img)->sizeX, (img)->stretchY = (img)->sizeY; }
	#define oslMirrorImageH(img)					{ float ___tmp;  ___tmp = (img)->offsetX0; (img)->offsetX0 = (img)->offsetX1; (img)->offsetX1 = ___tmp; }
	#define oslMirrorImageV(img)					{ float ___tmp;  ___tmp = (img)->offsetY0; (img)->offsetY0 = (img)->offsetY1; (img)->offsetY1 = ___tmp; }

#endif

/** Return true if the image was mirrored horizontally. */
#define oslImageIsMirroredH(img)				((img)->offsetX1 > (img)->offsetX0)
/** Return true if the image was mirrored vertically. */
#define oslImageIsMirroredV(img)				((img)->offsetY1 > (img)->offsetY0)


//Donne les coordonnées d'une image de manière à ce qu'avec un angle de 0 le haut-gauche de l'image se trouve à la la position x,y indiquée
/** Moves the image so that its top-left corner appears at the specified x and y coordinates when angle is 0, taking in account rotation parameters. */
#define oslImageRotMoveX(img,x)			((((x)+(img)->centerX*(img)->stretchX)/(img)->sizeX))
/** Same as oslImageRotMoveX but for the y coordinate. */
#define oslImageRotMoveY(img,y)			((((y)+(img)->centerY*(img)->stretchY)/(img)->sizeY))
/** Sets the rotation center (centerX, centerY properties) to the center of the image. Useful for rotation as it seems usually normal that the rotation center is the middle of the image in question. */
#define oslSetImageRotCenter(img)		((img)->centerX=(int)oslAbs((img)->offsetX1-(img)->offsetX0)>>1, (img)->centerY=(int)oslAbs((img)->offsetY1-(img)->offsetY0)>>1)
/** Alias for oslSetImageRotCenter. */
#define oslImageSetRotCenter			oslSetImageRotCenter


/** Awful hacky routine, which crops 0.5 pixel to each image corner.
This is useful if bilinear filtering is turned on and the image is stretched. In this case, by interpolation, the GPU may add a border to the image, which looks bad.
This routine will make sure that this border won't be displayed. */
#define oslCorrectImageHalfBorder(img)			((img)->offsetX0 = (img)->offsetX0 + 0.5f, (img)->offsetY0 = (img)->offsetY0 + 0.5f, (img)->offsetX1 = (img)->offsetX1 - 0.5f, (img)->offsetY1 = (img)->offsetY1 - 0.5f, img->stretchX--, img->stretchY--)
/** Undoes the effect of oslCorrectImageHalfBorder. Rather use oslResetImageTile, this one will mess your image up if you're calling it twice or if you call without having called oslCorrectImageHalfBorder before. */
#define oslResetImageHalfBorder(img)			((img)->offsetX0 = (img)->offsetX0 - 0.5f, (img)->offsetY0 = (img)->offsetY0 - 0.5f, (img)->offsetX1 = (img)->offsetX1 + 0.5f, (img)->offsetY1 = (img)->offsetY1 + 0.5f, img->stretchX++, img->stretchY++)

/** Returns the image width, taking in account the frame (tile). You can retrieve the full image width with image->sizeX. */
#define oslGetImageWidth(img)			((int)oslAbs((img)->offsetX1-(img)->offsetX0))
/** Returns the image height, taking in account the frame (tile). You can get the full image height with image->sizeY. */
#define oslGetImageHeight(img)			((int)oslAbs((img)->offsetY1-(img)->offsetY0))

#define oslGetImageSizeX				oslGetImageWidth
#define oslGetImageSizeY				oslGetImageHeight

/** @} */ // end of image_placement


/** @defgroup image_manip Image manipulation

	Image manipulation in OSLib.
	@{
*/

/** Creates an alternate image referencing a part (tile) of another one.

	\param offsetX0
		The starting x position of the image part to reference
	\param offsetX1
		The ending x position of the image part to reference
	\param offsetY0
		The starting y position of the image part to reference
	\param offsetY1
		The ending y position of the image part to reference

This will return a pointer to a new image, but the data will not be copied to the new image, instead it will reference the original
image data. When you will delete the new image, the original image will be left unchanged, only the size of an OSL_IMAGE will be freed from memory (about 80 bytes? I don't remember). However if you delete
the original image, the new one will be unusable! If you try to draw it, corrupt data may appear, and if you try to write to it, your program may crash.

\code
OSL_IMAGE *imageTile = oslCreateImageTile(originalImage, 0, 0, 32, 32);
oslDrawImage(imageTile);									//Will draw a 32x32 image inside of originalImage
oslDeleteImage(imageTile);									//Only frees the copy, the original remains untouched!
\endcode

This routine was meant to create individual "sprite" images from a big one and manipulate them easily. However as it's not very clear, avoid the use of it.
Another way to do it would be:
\code
oslSetImageTile(originalImage, 0, 0, 32, 32);
oslDrawImage(originalImage);								//Will draw a 32x32 image inside of originalImage
\endcode */
extern OSL_IMAGE *oslCreateImageTile(OSL_IMAGE *img, int offsetX0, int offsetY0, int offsetX1, int offsetY1);
/** Same but you specify the width and height of the image part to be displayed instead of the box coordinates. */
extern OSL_IMAGE *oslCreateImageTileSize(OSL_IMAGE *img, int offsetX0, int offsetY0, int width, int height);

/** Converts an image to another pixel format (and places it to another location). The image may have moved, so you must not try to use the original image anymore but the returned image instead!
\code
image = oslConvertImageTo(image, OSL_IN_RAM, OSL_PF_5551);
\endcode
*/
extern OSL_IMAGE *oslConvertImageTo(OSL_IMAGE *imgOriginal, int newLocation, int newFormat);

/** Creates a copy of an image. The image data is copied as well, so the returned image is a completely new instance, and both are not linked together. */
extern OSL_IMAGE *oslCreateImageCopy(OSL_IMAGE *src, int newLocation);

/** Creates a copy of an image, just like oslCreateImageCopy, but new one will be swizzled. */
extern OSL_IMAGE *oslCreateSwizzledImage(OSL_IMAGE *src, int newLocation);

/** Copies an image to another (copies the image data). Both images must have the same format (width, height, pixelformat) else the copy will fail! */
extern void oslCopyImageTo(OSL_IMAGE *imgDst, OSL_IMAGE *imgSrc);

/** Swizzles an image. A swizzled image is then drawn a lot faster, but the problem is that you cannot modify it then because it gets a special format. However you can still unswizzle it, but it's slow if
you do it often.

Hint: You should always swizzle images stored in RAM. Use the OSL_SWIZZLED bit when loading them for more facility. */
extern void oslSwizzleImage(OSL_IMAGE *img);

/** Does the exact opposite of oslSwizzleImage. Call this after your image has been swizzled to restore its original state and make it accessible for raw reading and writing. */
extern void oslUnswizzleImage(OSL_IMAGE *img);


/** Swizzles imgSrc and writes the result to imgDst. Old and bad routine. Use oslSwizzleImage instead (swizzles a single image). */
extern void oslSwizzleImageTo(OSL_IMAGE *imgDst, OSL_IMAGE *imgSrc);

/** Moves an image to a new location, which can be either OSL_IN_RAM or OSL_IN_VRAM. */
extern bool oslMoveImageTo(OSL_IMAGE *img, int newLocation);

/** Clears an image with a specific value. Color is the raw pixel value, and depends from the pixelformat. For example if the image is OSL_PF_4BIT, the color is a palette entry number. Or if it's
OSL_PF_5551 it's a 15-bit color with alpha. Example:
\code
OSL_IMAGE *img = oslCreateImage(32, 32, OSL_IN_RAM, OSL_PF_4444);
//Clear image to black, opaque (alpha=255)
oslClearImage(img, RGBA12(0, 0, 0, 255));
\endcode
*/
extern void oslClearImage(OSL_IMAGE *img, int color);				//A tester!!!

/** Writes an image to a file. Same remark as oslLoadImageFile, avoid this because it will autodetect the file type depending on its extension, and thus include EVERY possible format. This adds unnecessary
code to your project, rendering it very fat and wasting RAM.

\b Note: It is impossible to write a swizzled image. OSLib internally checks for that, and calling oslWriteImageFile with a swizzled image will do nothing. If your image is swizzled, you should first
unswizzle it by calling #oslUnswizzleImage. */
extern int oslWriteImageFile(OSL_IMAGE *img, const char* filename, int flags);
/** Writes an image to a PNG file. Same remarks as oslWriteImageFile apply.
	\param img
		The image you want to write.
	\param filename
		The name of the file that you want to write to.
	\param flags
		Either 0 or OSL_WRI_ALPHA. If OSL_WRI_ALPHA is specified, the alpha will be written to the PNG image file, making it semi-transparent. Else, alpha is ignored and always set to opaque.

	You can pass for example OSL_SECONDARY_BUFFER (the buffer which is currently displayed on the screen) to take a screenshot.
\code
oslWriteImageFilePNG(OSL_SECONDARY_BUFFER, "screenshot.png", 0);
\endcode

\b Note: The same considerations as #oslWriteImageFile apply. */
extern int oslWriteImageFilePNG(OSL_IMAGE *img, const char* filename, int flags);

/** Resets the properties of an image (position, image tile, angle, rotation center, stretching). */
extern void oslResetImageProperties(OSL_IMAGE *img);

/** Draws srcImg to dstImg with being scaled. */
extern void oslScaleImage(OSL_IMAGE *dstImg, OSL_IMAGE *srcImg, int newX, int newY, int newWidth, int newHeight);

/** Creates a scaled copy of an image. */
extern OSL_IMAGE *oslScaleImageCreate(OSL_IMAGE *img, short newLocation, int newWidth, int newHeight, short newPixelFormat);

/** Look at oslWriteImageFilePNG for more information. */
enum OSL_WRITE_FLAGS	{
	OSL_WRI_ALPHA = 1							//!< Writes the alpha channel of the image.
};

/** @} */ // end of image_manip



/** @defgroup image_sprites Simili sprite system

	A basic system allowing you to cut your images just as if they were sprites. Remember that the maximum size of an image is 512x512!
	@{
*/

/** Defines the size of a frame in an image. This prepares the image for an use as a sprite sheet. In a sprite sheet, you have to arrange your sprites (small images) in a grid, whose dimensions are fixed.
That is, if you decide for example that the sprite size is 29x37, EVERY sprite must have the same size, and be padded if it doesn't. Sprites are placed from left to right, and then on the next line from
top to bottom (just like the text you are reading). For example, a 64x64 image containing 32x32 sprites (total: 4 sprites) would have the following disposition:

\code
sprite 1 | sprite 2
---------|---------
sprite 3 | sprite 4

That is, the following coordinates:

0, 0     | 32, 0
---------|---------
0, 32    | 32, 32
\endcode

\b Important: The maximum size of an image is 512x512! See considerations with #oslCreateImage. */
static inline void oslSetImageFrameSize(OSL_IMAGE *img, u16 width, u16 height)			{
	img->frameSizeX = width, img->frameSizeY = height;
}

/**

To make a smart use of this feature, remember that you can always code macros to simplify and wrap your code.
\code
//Remember to never use a name beginning by oslSomething for your functions
//because it may exist some day in OSLib, and it will break your code!

//Draws a specific frame of an image
void DrawImageFrame(OSL_IMAGE *img, int frame)		{
	oslSetImageFrame(img, frame);
	oslDrawImage(img);
}

//Alternative method using a macro (won't work with Visual Studio, except if you remove the parenthesis,
//but they are needed because of a bug with GCC)
#define DrawImageFrame(img, frame)			({ oslSetImageFrame(img, frame); oslDrawImage(img); })

//If you really want to do an inline thing (the same way as a macro), use this instead:
extern inline void DrawImageFrame(OSL_IMAGE *img, int frame)		{
	oslSetImageFrame(img, frame);
	oslDrawImage(img);
}

//Draws a specific frame of an image at a specified location (like a real sprite)
void DrawImageFrameXY(OSL_IMAGE *img, int x, int y, int frame)		{
	oslSetImageFrame(img, frame);
	oslDrawImageXY(img, x, y);
}

//Load an image specifying the frame size directly
void LoadSpriteFilePNG(char *filename, int location, int pixelFormat, int frameWidth, int frameHeight)		{
	OSL_IMAGE *img = oslLoadImageFile(filename, location, pixelFormat);
	if (img)
		oslSetImageFrameSize(frameWidth, frameHeight);
	return img;
}

void main()		{
	//Initialization
	[...]

	//30x40 pixels per sprite
	OSL_IMAGE *myImage;
	myImage = LoadSpriteFilePNG("test.png", OSL_IN_RAM, OSL_PF_5551, 30, 40);
	[...]

	//Draw the frame n°2 (that is, the third sprite) at location (0, 0) (top-left corner of the screen).
	DrawImageFrameXY(myImage, 0, 0, 2);
	[...]
} \endcode */
extern void oslSetImageFrame(OSL_IMAGE *img, int frame);

/** @} */ // end of image_sprites



/** @defgroup image_palette Image palettes

	Palette for 4 and 8-bit images.
	@{
*/

/** Creates a new (empty) palette with the specified pixelFormat.
	\param size
		Number of colors of the palette.
	\param location
		Location where to put the palette. Always put it in RAM for now (OSL_IN_RAM).
	\param pixelFormat
		Pixel format of each palette entry. It should always be OSL_PF_8888 as palettes are rather small in size.

	Remember that raw access to palette is cached, so you must uncache it after having filled it. Look at oslGetImageLine and oslGetImagePixelAdr for more information.
\code
OSL_IMAGE *img = oslCreateImage(32, 32, OSL_IN_RAM, OSL_PF_4BIT);
//We can get the palette size with this (in our case it's 16, as 4 bits means 16 colors).
int palSize = 1 << osl_paletteSizes[img->pixelFormat];
//Loop index variable.
int i;
//Palettes are not automatically created with images, we need to do it by yourself
img->palette = oslCreatePaletteEx(palSize, OSL_IN_RAM, OSL_PF_8888);
//Get a pointer to palette data
u32 *paletteData = (u32*)img->palette->data;
//Set all entries to bright opaque red.
for (i=0;i<img->palette->nElements;i++)
	paletteData[i] = RGBA(255, 0, 0, 255);
//Don't forget to uncache the palette after finished with it
oslUncachePalette(img->palette);
//An alternate possibility is: oslUncacheImage(img); (uncaches the palette as well)
\endcode
*/
extern OSL_PALETTE *oslCreatePaletteEx(int size, int location, short pixelFormat);

/** Creates a palette. Simpler function without the \e location argument. */
static inline OSL_PALETTE *oslCreatePalette(int size, short pixelFormat)		{
	return oslCreatePaletteEx(size, OSL_IN_RAM, pixelFormat);
}

/** Creates a palette from existing data. Please note that data is not copied to a new location but used as is! You just have to specify the palette size and pixelFormat. */
extern OSL_PALETTE *oslCreatePaletteFrom(void *data, int size, short pixelFormat);

/** Deletes an existing palette. If the palette was created with oslCreatePaletteFrom, the data is not freed or anything, it's left as it was before, only the OSL_PALETTE structure is freed. */
extern void oslDeletePalette(OSL_PALETTE *p);

/** Returns a color entry from a palette. The color will be in the same pixelformat as the palette (16 or 32 bits). */
extern int oslGetPaletteColor(OSL_PALETTE *p, int index);

/** Uncaches a palette. As with oslUncacheData, always uncache a palette after you've accessed it in a cached way (pal->data). Look at oslCreatePalette for more info. */
extern void oslUncachePalette(OSL_PALETTE *pal);

/** @} */ // end of image_palette


/** @defgroup image_pixel Pixel access

	Low level access to images.
	@{
*/

//Nécessaire avant de pouvoir dessiner ou manier l'image (à appeler après une modification du contenu de  img->data à la main)
/** Flushes the image data from the cache.
Never forget to call this after you've modified an image in a cached way (by default all the following routines do). See oslUncacheData for more information.

Note: this routine does not flush the associated image palette data! Call oslUncacheImage instead if you need it! */
static inline void oslUncacheImageData(OSL_IMAGE *img)		{
    if (img != NULL)
        sceKernelDcacheWritebackInvalidateRange(img->data, img->totalSize);
}
/** Uncache a whole image, including its associated palette (image->palette). */
extern void oslUncacheImage(OSL_IMAGE *img);

/** High-level routine which reads a single pixel on an image. Beware, caching is enabled, so after finished reading and/or writing pixels to an image, please take care to uncache your image! The best
and cleaner way to do it is to use #oslLockImage and #oslUnlockImage. See #oslGetImagePixel for an example.

	\return
		The value of the pixel at that position. The value depends on the pixelformat, it can be a palette entry number if the image is paletted, or a color in the format of the image (so you may have
		to convert it depending on what you'd like).

\code
//Get a 32 bit (OSL_PF_8888) color for the pixel at x, y, whatever its pixelformat is.
pixel = oslConvertColorEx(yourImage->palette, OSL_PF_8888, yourImage->pixelFormat, oslGetImagePixel(yourImage, x, y));
\endcode

Note: This routine also works for swizzled images!

\b Note: this routine is rather slow, you should consider raw access if you need good performance. */
extern int oslGetImagePixel(OSL_IMAGE *img, unsigned int x, unsigned int y);

/** Sets a pixel on the image. Same remarks as for oslGetImagePixel apply. The value is in the destination format of the image (so, either a palette entry or a color in the good format). Use conversions
if needed, as shown in the example above. Here is another example that does calculation on every pixel of an image.

\code
//Before doing low level operations on an image, we first need to lock it, it takes care of low level cache operations.
oslLockImage(image);
{
	//The {...} block is just for estethic, not necessary
	for (j=0;j<image->sizeY;j++)		{
		for (i=0;i<image->sizeX;i++)		{
			//Get a pixel of the image (depends on its pixelformat, here we assume it's 8888)
			u32 pixel = oslGetImagePixel(image, i, j);
			//Mask out the green and blue components
			oslSetImagePixel(image, i, j, pixel & 0xff0000ff);
		}
	}
}
oslUnlockImage(image);
\endcode */
extern void oslSetImagePixel(OSL_IMAGE *img, unsigned int x, unsigned int y, int pixelValue);

/** Returns the address of the beginning (left border) of an image line. This address can be used to access image raw data. Please note that when doing this, you'll have to respect the pixelformat of the
image! If the image is 16 bits (4444, 5551 or 5650) then use a u16 pointer and appropriate RGBxx macros.

Here is an example which fills one line:
\code
//Create a 15-bit image (16-bit pixel width as 1 alpha bit is added).
OSL_IMAGE *img = oslCreateImage(32, 32, OSL_IN_RAM, OSL_PF_5551);
//Get a pointer to the 4th line (0 is the first, 1 the second, ...)
u16 *data = (u16*)oslGetImageLine(img, 3);
//Loop index variable
int i;
//From the beginning to the end of the line...
for (i=0;i<img->sizeX;i++)		{
	//Set this pixel to bright red
	*data = RGBA15(255, 0, 0, 255);
	//Point to the next pixel
	data++;
}
\endcode
*/
#define oslGetImageLine(img,y)			((char*)((img)->data) + (y)*(((img)->realSizeX * osl_pixelWidth[(img)->pixelFormat])>>3))
/** Gets the address of a single pixel of an image. Please note that this works only for 8 to 32 bits pixel types, but not for 4 bits, as the byte is the smallest unit you can work with, and in this case
a byte contains 2 pixels. In case of 4 bits, a pointer to the corresponding pixel pair (byte) will be returned, and you'll have to use masks to handle this correctly.

Here is an (untested) example:
\code
void setPixel(OSL_IMAGE *img, int x, int y, int value)		{
	u8 *data = oslGetImagePixelAdr(x, y);

	//The pixel value can't be larger than 4 bits, else we would overflow on the next pixel, messing up the display.
	value &= 0xf;

	//First method: is x even?
	if (x & 1)		{
		//Mask out the last 4 bits
		data &= 0x0f;
		//Write to the last 4 bits
		data |= value << 4;
	}
	else	{
		//Mask out the first 4 bits
		data &= 0xf0;
		//Write to the first 4 bits
		data |= value;
	}

	//Second method: directly integrate the parity checking
	data &= ~(15 << ((x & 1) << 2));
	data |= value << ((x & 1) << 2);
}
\endcode
*/
#define oslGetImagePixelAddr(img,x,y)	((char*)((img)->data) + ((((y)*(img)->realSizeX + (x)) * osl_pixelWidth[(img)->pixelFormat])>>3))

//For backward compatibility
#define oslGetImagePixelAdr				oslGetImagePixelAddr

/** Returns the address of a pixel, working even if the image is swizzled. This implementation is slower but works in every case. */
void *oslGetSwizzledPixelAddr(OSL_IMAGE *img, unsigned int x, unsigned int y);

/** Converts a color.
	\param pfDst
		Destination pixel format.
	\param pfSrc
		Source pixel format.
	\param color
		Color (in the source pixel format).
*/
extern int oslConvertColor(int pfDst, int pfSrc, int color);
/** Converts a color but using a palette if either the source or destination pixel format are paletted. See oslGetImagePixel for an example of use. */
extern int oslConvertColorEx(OSL_PALETTE *p, int pfDst, int pfSrc, int color);


/** Locks an image, so that you can access it. Althrough not necessary, this is a good thing to do because it will automatically handle drawing to the drawbuffer, uncaching the image when finished and
flushing the texture cache to avoid render errors when an image is modified while it's being drawn.
\code
//Lock the image
oslLockImage(myImage);
{
	//Do some software access to the image
	u16 *ptr = (u16*)myImage->data;
	*ptr = 0;
}
//Unlock it after having finished
oslUnlockImage(myImage);

//We can now draw it!
oslDrawImage(myImage);
\endcode
*/
extern void oslLockImage(OSL_IMAGE *img);
/** Unlock the image after you've finished with it. */
extern void oslUnlockImage(OSL_IMAGE *img);


/** List of pixel widths (in bits) for each pixel format. */
extern const int osl_pixelWidth[];
/** List of palette sizes (in powers of two). For non-paletted formats, it is zero. Else you can get the palette size by doing 1 << osl_paletteSizes[pixelFormat]. */
extern const u8 osl_paletteSizes[];

/** @} */ // end of image_pixel


/** @defgroup image_drawbuf Drawbuffers

	Drawbuffers is an advanced and powerful capability of OSLib images, it allows you to draw directly on an image exactly as if you were drawing on the screen!
	See #oslSetDrawBuffer for basic a code sample.
	@{
*/

/** Returns the current drawbuffer as an OSL_IMAGE. You can save it to restore it later. */
#define oslGetDrawBuffer()			osl_curBuf

/** An image representing the primary buffer image, which is the buffer to which you are currently writing (see oslSwapBuffers for more information). */
#define OSL_DEFAULT_BUFFER			(&osl_defaultBufferImage)
/** An image representing the secondary buffer image, which is the buffer currently displayed to the user (see oslSwapBuffers for more information). In single buffering mode, OSL_DEFAULT_BUFFER is
identical to OSL_SECONDARY_BUFFER. */
#define OSL_SECONDARY_BUFFER		(&osl_secondaryBufferImage)

/** Returns the width of the current drawbuffer. On the default drawbuffer (i.e. the screen) it will be the resolution of the PSP LCD, that is 480 pixels. By taking these values in account you can make
game that automatically adapt to another resolution. */
#define OSL_SCREEN_WIDTH		(osl_curBuf->sizeX)
/** Returns the height of the current drawbuffer. On the default drawbuffer (i.e. the screen) it will be the resolution of the PSP LCD, that is 272 pixels. */
#define OSL_SCREEN_HEIGHT		(osl_curBuf->sizeY)

/** Define an image as the current drawbuffer. Don't forget to restore the original drawbuffer once you've finished drawing to the image. Here is an example that draws something on an image, and then
draws that image on the real screen.
\code
OSL_IMAGE *drawBuf = oslCreateImage(320, 182, OSL_IN_VRAM, OSL_PF_5650);
//Clear the image to black
oslClearImage(drawBuf, RGB16(0, 0, 0));
//We set that image as the drawbuffer
oslSetDrawBuffer(drawBuf);
//Draw a red filled rectangle on the image
oslDrawFillRect(0, 0, 100, 100, RGB(255, 0, 0));
//Restore the default drawbuffer
oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
//Draw that image somewhere on the screen
oslDrawImageXY(drawBuf, 80, 45);
\endcode

\b Important: The image pixel format must be non-paletted! (supported formats are 4444, 5551, 5650 and 8888). 8888 mode (32-bit) is slower than others and the image is bigger. Also, the image MUST be in
video memory (OSL_IN_VRAM). The GPU can't write to regular RAM (it can only read from it).

A common problem is the image not being displayed because its alpha is null. You can temporarily disable alpha blending by calling oslSetAlpha(OSL_FX_NONE, 0) to prevent this problem, but the best solution
is to not forget to always clear the image after you've created it!
*/
extern void oslSetDrawBuffer(OSL_IMAGE *img);

/** Enables writing to the alpha channel of the drawbuffer. You won't care about it when using the standard drawbuffer, but when it comes to draw on an image, it may become important. By default, alpha
values are never set on the drawbuffer (let as they are).
	\param action
		- OSL_FXAW_SET: Set the alpha to value1. Every pixel written to the drawbuffer will set the alpha value to value1. All pixels (for example from a square or rectangle image) will set the alpha
		value, even if they are transparent. In order to avoid this, you can set-up an alpha test which masks out transparents pixels. They will not be drawn and the alpha value will not be written
		in the drawbuffer.
	\param value1
		If action is OSL_FXAW_SET, this parameter holds the alpha opacity to be written, from 0 to 255 (0 = transparent, 255 = opaque).
	\param value2
		Not used, let it 0.

I've not got pure copy of the alpha values to work. If someone has already used it and wants to share his experience, it would be very nice.

\code
//Clear image to transparent (alpha=0)
oslClearImage(buffer, RGBA(0, 0, 0, 0));
//Set it as drawbuffer
oslSetDrawBuffer(buffer);
//Every pixel written will take the value 255 (opaque)
oslSetAlphaWrite(OSL_FXAW_SET, 255, 0);
//But do not draw transparent pixels (only those greater than 0).
oslSetAlphaTest(OSL_FXAT_GREATER, 0);
[...]
\endcode
*/
extern void oslSetAlphaWrite(int action, int value1, int value2);

/** Set alpha testing parameters. The test will be made against the alpha value of the pixel to be drawn. If the test passes, the pixel is written to the screen, else it is ignored. Note that pixels from
a OSL_PF_5650 image do not initially contain alpha, but obviously it is automatically set to opaque (alpha = 255).
	\param condition
		Condition for the test to pass. Can be one of the following:
		- OSL_FXAT_NEVER: Never passes.
		- OSL_FXAT_ALWAYS: Always passes.
		- OSL_FXAT_EQUAL: Alpha is equal to value.
		- OSL_FXAT_NOTEQUAL: Alpha is not equal to value.
		- OSL_FXAT_LESS: Alpha is less than value.
		- OSL_FXAT_LEQUAL: Alpha is less or equal to value.
		- OSL_FXAT_GREATER: Alpha is greater than value.
		- OSL_FXAT_GEQUAL: Alpha is greater or equal to value.
	\param value
		Reference value for comparision.
*/
extern void oslSetAlphaTest(int condition, int value);

/** Disables alpha testing. */
extern void oslDisableAlphaTest();

/** Holds whether alpha testing is currently enabled. */
extern int osl_alphaTestEnabled;

/** Available effects for #oslSetAlphaWrite. */
enum OSL_FX_ALPHAWRITE		{
	OSL_FXAW_NONE,								//!< Does not write alpha
	OSL_FXAW_SET,								//!< Sets alpha to value1 if the alpha channel of the pixel is not equal to value2.
};

/** Alpha test comparision operators. See #oslSetAlphaTest. */
enum OSL_FX_ALPHATEST		{
	OSL_FXAT_NEVER = GU_NEVER,
	OSL_FXAT_ALWAYS = GU_ALWAYS,
	OSL_FXAT_EQUAL = GU_EQUAL,
	OSL_FXAT_NOTEQUAL = GU_NOTEQUAL,
	OSL_FXAT_LESS = GU_LESS,
	OSL_FXAT_LEQUAL = GU_LEQUAL,
	OSL_FXAT_GREATER = GU_GREATER,
	OSL_FXAT_GEQUAL = GU_GEQUAL
};


/** @} */ // end of image_drawbuf



/** @defgroup image_adv Advanced

	Advanced routines for working with images.
	@{
*/



/** Returns a nonzero value if the image is a copy of another, 0 if it's an original. */
#define oslImageIsCopy(img)						(img->flags & OSL_IMAGE_COPY)
/** Returns a nonzero value if the image is swizzled, 0 else. */
#define oslImageIsSwizzled(img)					(img->flags & OSL_IMAGE_SWIZZLED)
/** Returns a nonzero value if the image is swizzled, 0 else. */
#define oslImageGetAutoStrip(img)				(img->flags & OSL_IMAGE_AUTOSTRIP)

/** Defines if an image is a copy. For internal use only. */
static inline void oslImageIsCopySet(OSL_IMAGE *img, bool enabled)			{
	if (enabled)
		img->flags |= OSL_IMAGE_COPY;
	else
		img->flags &= ~OSL_IMAGE_COPY;
}

/** Defines if an image is swizzled. For internal use only */
static inline void oslImageIsSwizzledSet(OSL_IMAGE *img, bool enabled)			{
	if (enabled)
		img->flags |= OSL_IMAGE_SWIZZLED;
	else
		img->flags &= ~OSL_IMAGE_SWIZZLED;
}


/** Defines if an image should be automatically stripped (divided in stripes to be blitted faster if it's very big). You shouldn't care about this. */
static inline void oslImageSetAutoStrip(OSL_IMAGE *img, bool enabled)			{
	if (enabled)
		img->flags |= OSL_IMAGE_AUTOSTRIP;
	else
		img->flags &= ~OSL_IMAGE_AUTOSTRIP;
}

/** @} */ // end of image_adv

/** @} */ // end of image


/** \ingroup drawing
	@defgroup drawing_lowlev Low level drawing

	Low level drawing routines, nearer to the hardware and thus faster.
	@{
*/

/** @defgroup drawing_lowlev_general Basic documentation

	Here is a very little explanation on how to draw a textured or untextured object, assuming you already know about GU.
	Here is the skeleton of a very simple textured object and one untextured object.

\code
void drawSomethingTextured(OSL_IMAGE *img)				{
	//We'll work with vertices. Vertices represents a coordinate. Sprites have two coordinates, triangles have 3, quadliterals have 4.
	VERTEX_TYPE *vertices;

	//Set the image as the current texture
	oslSetTexture(img);

	vertices = (VERTEX_TYPE*)sceGuGetMemory(NUMBER_OF_VERTICES * sizeof(vertices[0]));

	vertices[0].something = ...;
	[...]

	sceGuDrawArray(PRIMITIVETYPE, VERTEX_DESCRIPTION | GU_TRANSFORM_2D, NUMBER_OF_VERTICES, 0, vertices);
}

void drawSomethingUntextured(OSL_IMAGE *img)				{
	//We'll work with vertices. Vertices represents a coordinate. Sprites have two coordinates, triangles have 3, quadliterals have 4.
	VERTEX_TYPE *vertices;

	//No texturing
	oslDisableTexturing();

	vertices = (VERTEX_TYPE*)sceGuGetMemory(NUMBER_OF_VERTICES * sizeof(vertices[0]));

	vertices[0].something = ...;
	[...]

	sceGuDrawArray(PRIMITIVETYPE, VERTEX_DESCRIPTION | GU_TRANSFORM_2D, NUMBER_OF_VERTICES, 0, vertices);
}
\endcode
As you see, it's very simple, if you're used to GU. If not, please read tutorials and examples from ps2dev.org. */

#ifdef PSP
	/** Disables texturing (the image will be drawn opaque using vertex colors). */
	#define oslEnableTexturing()	({ if (!osl_textureEnabled)  sceGuEnable(GU_TEXTURE_2D), osl_textureEnabled = 1; })
	/** Enables texturing. You should not call this function by yourself, as oslSetTexture does it by itself. */
	#define oslDisableTexturing()	({ if (osl_textureEnabled)  sceGuDisable(GU_TEXTURE_2D), osl_textureEnabled = 0; })
#else
	/** Disables texturing (the image will be drawn opaque using vertex colors). */
	#define oslEnableTexturing()	{ if (!osl_textureEnabled)  sceGuEnable(GU_TEXTURE_2D), osl_textureEnabled = 1; }
	/** Enables texturing. You should not call this function by yourself, as oslSetTexture does it by itself. */
	#define oslDisableTexturing()	{ if (osl_textureEnabled)  sceGuDisable(GU_TEXTURE_2D), osl_textureEnabled = 0; }
#endif

/** Don't forget about stripping when you're drawing images by yourself, else it will be slower! The thing to know is: never draw an image wider than 64 pixels. If the image is, say, 256 pixels wide, you
can draw it in 4 stripes of 64 each (setting u, v coordinates to 0, 64, 128, 192, etc.). */
#define OSL_SLICE_SIZE 64

/** Don't use it! It's internally used by oslDrawImageSimple. Look at the source code for more information. */
extern int oslVerifyStripBlit(OSL_IMAGE *img);


/** Holds the current texture wrap state (see #oslSetTextureWrap). */
extern int osl_currentTexWrapU;
/** Same for the v state. */
extern int osl_currentTexWrapV;

/** Sets the behaviour of images when their bounds are reached. For example let's say you have a 128x128 image and you draw a 200x200 region. This is larger than the image itself and thus the part to the
left or the right will be either repeated or clamped depending on the parameter.
	\param u, v
		OSL_TW_CLAMP: Clamp (the same pixel is repeated indefinitely)
		OSL_TW_REPEAT: The image texture is tiled.
*/
static inline void oslSetTextureWrap(int u, int v)		{
	sceGuTexWrap(u, v), osl_currentTexWrapU = u, osl_currentTexWrapV = v;
}

/** Available values for #oslSetTextureWrap. */
enum OSL_TEXWRAP_MODES		{
	OSL_TW_REPEAT = GU_REPEAT,					//!< Repeats the texture.
	OSL_TW_CLAMP = GU_CLAMP,					//!< Clamps the texture (the last image pixel is repeated indefinitely).
};



/** Defines the current texture, useful for low level image drawing. */
extern void oslSetTexture(OSL_IMAGE *img);
/** Same but also defines the beginning coordinate in the texture. Useful for tricks meant to display an image bigger than 512x512.
	\param img
		Texture to set
	\param x, y
		Beginning (top-left corner) of the image. Should be a multiple of 512. The (x, y) corner will become the (u, v) = (0, 0) coordinate when drawing a texture, and a 512x512 range (to the right and down)
		will be accessible.
*/
extern void oslSetTexturePart(OSL_IMAGE *img, int x, int y);

//Do NOT use this one
OSL_COLOR oslBlendColors(OSL_COLOR c1, OSL_COLOR c2);

/** Applies the alpha parameters to a color, tinting it. This is needed as alpha is not applied to vertex color but only to textures. */
static inline OSL_COLOR oslBlendColor(OSL_COLOR c)		{
	return oslBlendColors(c, osl_currentAlphaCoeff);
}

/** System display list. */
extern unsigned int *osl_list;
/** Holds whether texturing is currently enabled. */
extern int osl_textureEnabled;

//Undocumented - avoid use of them
//Current texture, current palette. They are used to avoid setting texture and uploading a palette too much times (which is slow) if it's the same than before.
extern void *osl_curTexture, *osl_curPalette;
extern void *osl_curDrawBuf, *osl_curDispBuf;
extern OSL_IMAGE osl_defaultBufferImage, osl_secondaryBufferImage, *osl_curBuf;
extern int osl_isDrawingStarted;

//Don't access this



/** @defgroup drawing_lowlev_vertex Vertex types

	Low level drawing routines, nearer to the hardware and thus faster.
	@{
*/

/** Precise vertex with texture. Uses float coordinates. */
typedef struct
{
	float u, v;
	float x, y, z;
} OSL_PRECISE_VERTEX;

/** Fast vertex with texture. Uses 16-bit coordinates. */
typedef struct
{
	unsigned short u, v;
	short x, y, z;
} OSL_FAST_VERTEX;

/** Fast vertex with texture and 16-bit color. Uses 16-bit coordinates. */
typedef struct
{
	unsigned short u, v;
	unsigned long color;
	short x, y, z;
} OSL_FAST_VERTEX_COLOR32;

/** Untextured vertex. Uses 16-bit coordinates with 32-bit color. */
typedef struct
{
	unsigned long color;
	short x, y, z;
} OSL_LINE_VERTEX;

/** Untextured vertex. Uses 16-bit coordinates and 16-bit color. */
typedef struct
{
	unsigned long color;
	short x, y, z;
} OSL_LINE_VERTEX_COLOR32;


/** Precise texture vertex. Uses 16-bit coordinates XYZ and float UV. */
typedef struct
{
	float u, v;
	short x, y, z;
} OSL_UVFLOAT_VERTEX;

/** @} */ // end of drawing_lowlev_vertex

/** @} */ // end of drawing_lowlev































// *** Image ***







// *** Général ***



// *** Interne ***








//Graphique::image

//Graphique::palette

//Graphique::dessin

//Graphique::écran

//Graphique::alpha


#ifdef __cplusplus
}
#endif

#endif

