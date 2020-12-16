#ifndef _SFONT_H
#define _SFONT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup SFont SFont

	Functions to manage SFonts
	@{
*/

/**Struct describing a signle SFont letter
	\param letter
        The image representing the letter
	\param width
        The letter's width
*/
typedef struct
{
      OSL_IMAGE *letter;
      unsigned int width;
} OSL_SFLETTER;

/**Struct describing a SFont
	\param letters
        Single letters
	\param lettersCount
        Total number of letters
*/
typedef struct
{
	OSL_SFLETTER *letters[256];
	int height;
	int lettersCount;
} OSL_SFONT;

/** Load an SFont
	Returns a pointer to the loaded OSL_SFONT or NULL
	\param filename
        file name
	\param pixelFormat
		pixel format
*/
OSL_SFONT *oslLoadSFontFile(char *filename, int pixelFormat);

/** Delete a loaded SFont
	\param sfont
        SFont to delete
*/
void oslDeleteSFont(OSL_SFONT *sfont);

/** Get the text width using a SFont
	Returns width in pixel of the string rendered with the given SFont
	\param sfont
        SFont
	\param text
        Text to measure
*/
int oslGetSFontTextWidth(OSL_SFONT *sfont, char *text);

/** Print a string using SFont
	Returns the string end position
    \param sfont
        SFont to use
	\param x
        X position
	\param y
        Y position
	\param text
        Text to print
*/
int oslSFontDrawText(OSL_SFONT *sfont, int x, int y, char *text);

/** @} */ // end of SFont

#ifdef __cplusplus
}
#endif

#endif