#include "oslib.h"
#include "sfont.h"
#include <png.h>

///////////////////////////////////////////////////////////////////////////////
//Private functions
///////////////////////////////////////////////////////////////////////////////
//Get offset for a char
static int _getOffset(OSL_SFONT *sfont, unsigned char ch)
{
    int offset;

    if (((ch >= 127) && (ch <= 159)) || (ch <= 32))
       offset = -1;
    else if (ch >= 160)
       offset = (int)ch - 160 + 126 - 33;
    else
       offset = (int)ch - 33;

    if (offset >= sfont->lettersCount)
       offset = -1;

    return offset;
}

//Get next power 2 of a number
static u16 _getNextPower2(u16 width)
{
    int k;
    u16 r = 1;

    for (k = 0; k < 16; ++k)
    {
       if (r >= width)
          return (r < 16) ? 16 : r;

       r *= 2;
    }

    return 0;
}

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
}

//Struct to load a png
typedef struct
{
	u32 *rawdata;
	int sizeX;
	int sizeY;
	int textureSizeX;
	int textureSizeY;
} PNG_DATA;

//Load a png image (this is necessary because OSLib cannot handle images bigger than 512x512)
static PNG_DATA* _loadFromPNG(char *filename)
{
	PNG_DATA *pngData = (PNG_DATA*)malloc(sizeof(PNG_DATA));
	if (!pngData)
		return NULL;

    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int bit_depth, color_type, interlace_type, y;
    png_uint_32 pw, ph;
    //u32* line;
    FILE *fp;
    int passes;
    u8** rows;

    VIRTUAL_FILE *f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_READ);
    if (!f)    return NULL;

    const unsigned char *input, *input_free;
    int input_size = 0;
    input = (const unsigned char*)oslReadEntireFileToMemory(f, &input_size);
    input_free = input;
    VirtualFileClose(f);

    fp = fmemopen((void *)input,input_size,"rb");
    if ( fp == NULL)
	{
	   free(pngData);
       return NULL;
	}

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
       fclose(fp);
       free((void*)input_free);
	   free(pngData);
	   return NULL;
    }

    png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
       fclose(fp);
       free((void*)input_free);
       png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
	   free(pngData);
	   return NULL;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &pw, &ph, &bit_depth, &color_type, &interlace_type, (int *) NULL, (int *) NULL);

    pngData->sizeX = (u16)pw;
    pngData->sizeY = (u16)ph;

    pngData->textureSizeX = _getNextPower2(pngData->sizeX);
    pngData->textureSizeY = _getNextPower2(pngData->sizeY);

    png_set_strip_16(png_ptr);
    png_set_packing(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
       png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
       png_set_expand_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
       png_set_tRNS_to_alpha(png_ptr);

    if (interlace_type == PNG_INTERLACE_ADAM7)
    {
       passes = png_set_interlace_handling(png_ptr);
       png_read_update_info(png_ptr, info_ptr);
    }
    else
    {
       passes = 1;
    }

    png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    pngData->rawdata = (u32*) memalign(16, pngData->textureSizeX * pngData->textureSizeY * sizeof(u32));
    if (!pngData->rawdata)
    {
       fclose(fp);
       free((void*)input_free);
       png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
	   free(pngData);
	   return NULL;
    }

    rows = (u8**)malloc(sizeof(u8*) * pngData->sizeY);
    for (y = 0;  y < pngData->sizeY; ++y)
    {
       rows[y] = (u8*)(pngData->rawdata + y * pngData->textureSizeX);
    }

    for (y = 0; y < passes; ++y)
    {
       png_read_rows(png_ptr, rows, NULL, pngData->sizeY);
    }

    free(rows);

    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    fclose(fp);
    free((void*)input_free);

	return pngData;
}

//Get a pixel from a png image
static u32 _getPixel(PNG_DATA *pngData, u16 x, u16 y)
{
    return *(pngData->rawdata + y * pngData->textureSizeX + x);
}

//Delete (free memory) of a png image
static void _deletePngImage(PNG_DATA *pngData)
{
    free(pngData->rawdata);
}

///////////////////////////////////////////////////////////////////////////////
//Public functions
///////////////////////////////////////////////////////////////////////////////
OSL_SFONT *oslLoadSFontFile(char *filename, int pixelFormat)
{
	OSL_SFONT *sfont;
	sfont = (OSL_SFONT*)malloc(sizeof(OSL_SFONT));
	if (!sfont){
		oslHandleLoadNoFailError(filename);
		return NULL;
	}

	PNG_DATA *img = _loadFromPNG(filename);
	if (!img){
		free(sfont);
		oslHandleLoadNoFailError(filename);
		return NULL;
	}

	unsigned int refcolor = _getPixel(img, 0, 0) & 0x00FFFFFFU;

    int x = 0;
	int letterCount = 0;
    while (x < img->sizeX)
    {
	   int color = _getPixel(img, x, 0);
       if ((color & 0x00FFFFFFU) != refcolor)
       {
          int pos = x;
          while (x < img->sizeX && ((_getPixel(img, x, 0) & 0x00FFFFFFU) != refcolor))
          {
             ++x;
          }

          OSL_SFLETTER *lt;
          lt = (OSL_SFLETTER*)malloc(sizeof(OSL_SFLETTER));
          if (!lt){
            _deletePngImage(img);
            free(sfont);
            oslHandleLoadNoFailError(filename);
            return NULL;
          }

          lt->width = x - pos;
		  lt->letter = oslCreateImage(lt->width, img->sizeY - 1, OSL_IN_RAM, pixelFormat);
          oslClearImage(lt->letter, RGBA(0, 0, 0, 0));
		  oslLockImage(lt->letter);

		  int dx = 0;
		  int dy = 0;
		  for (dx=pos; dx<pos+lt->width; dx++)
		  {
			  for (dy=1; dy<img->sizeY; dy++)
			  {
				  int pixel = _getPixel(img, dx, dy);
                  pixel = oslConvertColorEx(lt->letter->palette, lt->letter->pixelFormat, OSL_PF_8888, pixel);
				  oslSetImagePixel(lt->letter, dx-pos, dy-1, pixel);
			  }
		  }

		  oslUnlockImage(lt->letter);
		  oslSwizzleImage(lt->letter);
		  sfont->letters[letterCount++] = lt;
       }
       ++x;
    }

    sfont->height = img->sizeY - 1;
	sfont->lettersCount = letterCount;

    _deletePngImage(img);
	return sfont;
}

void oslDeleteSFont(OSL_SFONT *sfont)
{
	if (!sfont)
		return;

	int i = 0;
	for (i=0; i<sfont->lettersCount ;i++){
		oslDeleteImage(sfont->letters[i]->letter);
	}
	free(sfont);
	sfont = NULL;
}

int oslGetSFontTextWidth(OSL_SFONT *sfont, char *text)
{
    int w = 0;
	int k = 0;

    for (k=0; k < strlen(text); k++)
    {
       int offset = _getOffset(sfont, (unsigned char)text[k]);
       if (offset < 0)
          w += sfont->letters[0]->width;
       else
          w += sfont->letters[offset]->width;
    }
    return w;
}

int oslSFontDrawText(OSL_SFONT *sfont, int x, int y, char *text)
{
    int cx = x;
	int k = 0;
    for (k=0; k<strlen(text); k++)
    {
       int offset = _getOffset(sfont, (unsigned char)(text[k]));
       if (offset < 0)
          cx += sfont->letters[0]->width;
       else
       {
		  oslDrawImageXY(sfont->letters[offset]->letter, cx, y);
          cx += sfont->letters[offset]->width;
       }
    }
	return cx;
}
