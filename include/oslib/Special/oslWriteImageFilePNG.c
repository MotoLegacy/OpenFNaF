#include "../oslib.h"
#include "../png.h"
#include "../pngconf.h"
#include "../zlibInterface.h"
#include "../zlib.h"
#include "../zconf.h"

void oslPngFlushFn(png_structp png_ptr);

void oslPngWriteFn(png_structp png_ptr, png_bytep data, png_size_t length)			{
	VIRTUAL_FILE *f = (VIRTUAL_FILE*)png_get_io_ptr(png_ptr);
	VirtualFileWrite(data, length, 1, f);
}

//Prendre un screenshot
int oslWriteImageFilePNG(OSL_IMAGE *img, const char* filename, int flags)
{
	png_structp png_ptr;
	png_infop info_ptr;
	VIRTUAL_FILE *f;
	int i, x, y;
	u8* line;
	int width = img->offsetX1-img->offsetX0, height = img->offsetY1-img->offsetY0;
	int r, g, b, a;
	void *ptr;
	int color;
	const int saveAlpha = flags & OSL_WRI_ALPHA;
	int lbSuccess = 0;

	f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_WRITE);
	if (!f)
		return 0;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)		{
		goto error;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)		{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		goto error;
	}
//	png_init_io(png_ptr, fp);
	png_set_write_fn(png_ptr, f, oslPngWriteFn, oslPngFlushFn);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8,
		saveAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	line = (u8*) malloc(width * (saveAlpha ? 4 : 3));
	if (!line)
		goto error;
	for (y = 0; y < height; y++) {
		for (i = 0, x = 0; x < width; x++)		{
			ptr = oslGetImagePixelAdr(img, (int)(x + img->offsetX0), (int)(y + img->offsetY0));
			//N'avait pas d'effet jusque là et ça marchait bien donc bon...
//			if (img->location == OSL_IN_VRAM)
//				ptr = oslGetUncachedPtr(ptr);
			if (osl_pixelWidth[img->pixelFormat] == 32)
				color = *(unsigned long*)ptr;
			else if (osl_pixelWidth[img->pixelFormat] == 16)
				color = *(unsigned short*)ptr;
			else if (osl_pixelWidth[img->pixelFormat] == 8)
				color = *(unsigned char*)ptr;
			else				//4 bits pas supporté pour le moment
				color = 0;
			color = oslConvertColor(OSL_PF_8888, img->pixelFormat, color);
			r = color & 0xff;
			g = (color >> 8) & 0xff;
			b = (color >> 16) & 0xff;
			a = (color >> 24) & 0xff;
			if (img->pixelFormat == OSL_PF_5650)
				a = 0xff;
			line[i++] = r;
			line[i++] = g;
			line[i++] = b;
			if (saveAlpha)
				line[i++] = a;
		}
		png_write_row(png_ptr, line);
	}
	free(line);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	lbSuccess = 1;
error:
	VirtualFileClose(f);
	return lbSuccess;
}
