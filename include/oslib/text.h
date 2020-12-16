#ifndef TEXT_H
#define TEXT_H

#include "intraFont/intraFont.h"
/** @defgroup text Text functions

	Text functions in OSLib. Don't forget to look at the 'Debug Console' section (subsection of the 'Main' section).
	@{
*/

/** Loaded font */
typedef struct			{
	OSL_IMAGE *img;							//!< Image containing character sprites
	unsigned char *charWidths;				//!< Table containing the width of each character (256 entries)
	unsigned short *charPositions;			//!< Position of characters in the image (16-bits: y:7, x:9)
	int isCharWidthConstant;				//!< Internal (pour savoir s'il faut libérer charWidth)
	int charWidth;
	int charHeight;							//!< Height of characters (constant)
	int recentrage;							//!< Add this to text positions when drawing it (à ajouter aux positions pour le dessin du texte)
	unsigned char addedSpace;				//!< Space added between characters on the texture (allows to make characters bigger than indicated by charWidths)
    int fontType;                           //!< Font type (OSL_FONT_OFT or OSL_FONT_INTRA)
    intraFont *intra;                       //!< IntraFont data
} OSL_FONT;

/** Font information type */
typedef struct			{
	void *fontdata;							//!< Character image data
	short pixelFormat;						//!< 1 = 1 bit (default)
	unsigned char *charWidths;				//!< Width of characters
	int charWidth;							//!< Default char width (if charWidths is NULL)
	int charHeight;							//!< Height of characters (constant)
	int lineWidth;							//!< Number of bytes of data per line
	int recentrage;							//!< Added to text positions for drawing text (recentering)
	unsigned char addedSpace;				//!< Space added between the characters on the texture (allows to make characters graphically bigger than what indicated by charWidths)
	unsigned short paletteCount;			//!< Palette count
	unsigned long *paletteData;				//!< Palette data
} OSL_FONTINFO;

/** Header of a .oft file (Oslib FonT) */
typedef struct		{
	char strVersion[12];						//!< "OSLFont v01"
	unsigned char pixelFormat;				//!< Number of bits per pixel (1 = 1 bit, default)
	unsigned char variableWidth;			//!< If true, the first 256 bytes of data specify the character widths
	int charWidth, charHeight;				//!< Mean character sizes (used for the console)
	int lineWidth;								//!< Number of bytes of data per line
	unsigned char addedSpace;				//!< Space added between the characters on the texture (allows to make characters graphically bigger than what indicated by charWidths)
	unsigned short paletteCount;			//!< Text palette data
	unsigned char reserved[29];				//!< Must be null (reserved)
} OSL_FONT_FORMAT_HEADER;

/** Current font. You can read from it but please do not write to it, use #oslSetFont instead. */
extern OSL_FONT *osl_curFont;

/** Sets the current font. For forward compatibilty, please use this function rather than osl_curFont = yourFont.

\code
//Save the current font
OSL_FONT *oldFont = osl_curFont;
//Temporarily set another font
oslSetFont(newFont);
oslPrintf("Using the new font.\n");
//Restore the old font
oslSetFont(oldFont);
oslPrintf("Using the normal font.\n");
\endcode */
#define oslSetFont(f)		(osl_curFont = f)
//extern void oslSetFont(OSL_FONT *f);

//System defines
#define OSL_TEXT_TEXWIDTH 512
#define OSL_TEXT_TEXDECAL 9

/**Font type OFT*/
#define OSL_FONT_OFT 1
/**Font type INTRA*/
#define OSL_FONT_INTRA 2

//Returns the position of a character 'i' in the font 'f' texture as a pair (x, y).
#define OSL_TEXT_CHARPOSXY(f,i)		(f)->charPositions[i]&(OSL_TEXT_TEXWIDTH-1), ((f)->charPositions[i]>>OSL_TEXT_TEXDECAL)*(f)->charHeight

/** Initializes the console. You do not need to call this anymore, as it's automatically done upon #oslInitGfx. */
extern void oslInitConsole();

/** Loads a font from a file. Remember that you can load virtual files located in RAM, see the VirtualFile section for more information. Also, there is an application (font2osl) which converts fonts to
the OldSchool Library Font format (.oft).

\code
OSL_FONT *f = oslLoadFontFile("verdana.oft");
oslSetFont(f);
oslDrawString(0, 0, "Hello world using verdana!");
\endcode */
extern OSL_FONT *oslLoadFontFile(const char *filename);

/** Loads a font from a #OSL_FONTINFO file (located in RAM). Rather use oslLoadFontFile, which is more friendly.
Use this ONLY with OFT fonts (doesen't work with intraFont).
*/
extern OSL_FONT *oslLoadFont(OSL_FONTINFO *fi);

/** Draws a single character at the specified x and y positions.
If you must draw several characters placed one after the other, use oslDrawString, as indiviudal oslDrawChar calls will be slightly slower. */
extern void oslDrawChar(int x, int y, unsigned char c);

/** Draws a string litteral at the specified x and y positions.

\code
oslDrawString(0, 0, "Test string");
\endcode */
extern void oslDrawString(int x, int y, const char *str);

/** Draws a string litteral at the specified x and y positions limiting it to a given width.

\code
oslDrawStringLimited(0, 0, 200, "Test string");
\endcode */
extern void oslDrawStringLimited(int x, int y, int width, const char *str);

/** Draws a formatted string litteral at the specified x and y positions.

\code
oslDrawStringf(0, 0, "Test string %i", 1);
\endcode */
#define oslDrawStringf(x, y, ...)		{ char __str[1000]; sprintf(__str , __VA_ARGS__); oslDrawString(x, y, __str); }

/** Outputs a text to the console at the current cursor position, and moves it. Here for debugging purposes, not very useful in a game. */
extern void oslConsolePrint(const char *str);

/** Sets the current text color.
This doesen't work with intraFont (use oslIntraFontSetStyle)
*/
extern void oslSetTextColor(OSL_COLOR color);

/** Sets the text background color. Setting a transparent color (e.g. RGBA(0, 0, 0, 0)) will disable drawing a background. In this case, the text rendering will be faster.
This doesen't work with intraFont (use oslIntraFontSetStyle)
*/
extern void oslSetBkColor(OSL_COLOR color);

/** Draws a text box, that is, text contained in a rectangle. The text will automatically be wrapped at the end of a line and be placed below.
	\param x0, y0
		Top-left corner of the text box.
	\param x1, y1
		Bottom-right corner of the text box.
	\param text
		Text to be drawn. Can contain \n characters to make a carriage return.
	\param format
		Let it 0.
*/
extern void oslDrawTextBox(int x0, int y0, int x1, int y1, const char *text, int format);

/** Draws a text box, that is, text contained in a rectangle. The text will automatically be wrapped at the end of a line and be placed below.
	Draw text by word's and not by char's
	\param x0, y0
		Top-left corner of the text box.
	\param x1, y1
		Bottom-right corner of the text box.
	\param text
		Text to be drawn. Can contain \n characters to make a carriage return.
	\param format
		Let it 0.
*/
extern void oslDrawTextBoxByWords(int x0, int y0, int x1, int y1, const char *text, int format);

/** Deletes a font.

\b Warning: the font must NOT be currently selected (that is f != #osl_curFont) else your program will crash the next time you'll try to draw a character (or later). */
extern void oslDeleteFont(OSL_FONT *f);

/** Returns the size (in pixels) of a text, using the currently selected font. Note that you can get the height of the text and some other parameters by exploring the #OSL_FONT structure. The following
sample shows you how to center a text horizontally and align it to the bottom of the screen:

\code
const char *text = "© 2007 Brunni";
int width = oslGetStringWidth(text);
oslDrawString((SCREEN_WIDTH - width) / 2, SCREEN_HEIGHT - osl_curFont->charHeight, text);
\endcode */
extern int oslGetStringWidth(const char *str);

/** Returns the height (in pixels) which would take a text box drawn with #oslDrawTextBox. */
extern int oslGetTextBoxHeight(int width, int maxHeight, const char *text, int format);

/** Console horizontal position (in pixels). Use #oslMoveTo to move the cursor. */
extern int osl_consolePosX;
/** Console vertical position (in pixels). */
extern int osl_consolePosY;

/** System OSLib font. */
extern OSL_FONT *osl_sceFont;

//Font to be loaded
extern OSL_FONTINFO osl_sceFontInfo;

//IntraFont functions:
/**Inits intraFont (MUST be called before loading any pgf font).
The same options will be applied to all intraFonts
	\param options
		INTRAFONT_XXX flags as defined above including flags related to CACHE (ored together)

\code
#define INTRAFONT_ADVANCE_H     0x00000000 //default: advance horizontaly from one char to the next
#define INTRAFONT_ADVANCE_V     0x00000100
#define INTRAFONT_ALIGN_LEFT    0x00000000 //default: left-align the text
#define INTRAFONT_ALIGN_CENTER  0x00000200
#define INTRAFONT_ALIGN_RIGHT   0x00000400
#define INTRAFONT_WIDTH_VAR     0x00000000 //default: variable-width
#define INTRAFONT_WIDTH_FIX     0x00000800 //set your custom fixed witdh to 24 pixels: INTRAFONT_WIDTH_FIX | 24
                                           //(max is 255, set to 0 to use default fixed width, this width will be scaled by size)
#define INTRAFONT_ACTIVE        0x00001000 //assumes the font-texture resides inside sceGuTex already, prevents unecessary reloading -> very small speed-gain
#define INTRAFONT_STRING_ASCII  0x00000000 //default: interpret strings as ascii text
#define INTRAFONT_STRING_SJIS   0x00002000 //interpret strings as shifted-jis (japanese)
#define INTRAFONT_STRING_UTF8   0x00010000 //interpret strings as UTF-8
#define INTRAFONT_CACHE_MED     0x00000000 //default: 256x256 texture (enough to cache about 100 chars)
#define INTRAFONT_CACHE_LARGE   0x00004000 //512x512 texture(enough to cache all chars of ltn0.pgf or ... or ltn15.pgf or kr0.pgf)
#define INTRAFONT_CACHE_ASCII   0x00008000 //try to cache all ASCII chars during fontload (uses less memory and is faster to draw text, but slower to load font)

                                           //if it fails: (because the cache is too small) it will automatically switch to chache on-the-fly with a medium texture
									       //if it succeeds: (all chars and shadows fit into chache) it will free some now unneeded memory
#define INTRAFONT_CACHE_ALL     0x0000C000 //try to cache all chars during fontload (uses less memory and is faster to draw text, but slower to load font)
                                       //if it fails: (because the cache is too small) it will automatically switch to chache on-the-fly with a large texture
									   //if it succeeds: (all chars and shadows fit into chache) it will free some now unneeded memory
\endcode
*/
extern int oslIntraFontInit(unsigned int options);

/** Loads a font from a pgf file (intraFont). Use this if you want to load a pgf font with options different from the one passed to oslIntraFontInit.

\code
OSL_FONT *f = oslLoadIntraFontFile("verdana.pgf", INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8);
oslSetFont(f);
oslDrawString(0, 0, "Hello world using verdana!");
\endcode */
extern OSL_FONT *oslLoadIntraFontFile(const char *filename, unsigned int options);
extern void oslLoadAltIntraFontFile(OSL_FONT *font, const char *filename);

/**Sets style for a pgf font (works ONLY with pgf font) */
extern void oslIntraFontSetStyle(OSL_FONT *f, float size, unsigned int color, unsigned int shadowColor, unsigned int options);

/**
 * Draw text along the baseline starting at x, y.
 *
 * @param f - A valid ::OSL_FONT with type ::OSL_FONT_INTRA
 *
 * @param x - X position on screen
 *
 * @param y - Y position on screen
 *
 * @param width - column width for automatic line breaking (intraFontPrintColumn... versions only)
 *
 * @param autoBreakLine - char length of text to draw (...Ex versions only)
 *
 * @param text - Text to draw (ASCII & extended ASCII, S-JIS or UTF-8 encoded)
 *
 * @returns The x position after the last char
 */
extern float oslIntraFontPrintColumn(OSL_FONT *f, float x, float y, float width, int autoBreakLine, const char *text);

/**Shuts down intraFont */
extern void oslIntraFontShutdown();

/** @} */ // end of text

#endif
