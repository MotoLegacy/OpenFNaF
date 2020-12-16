#include "../oslib.h"
#include "../png.h"
#include "../pngconf.h"
#include "../zlibInterface.h"
#include "../zlib.h"
#include "../zconf.h"

// Read / Write PNG
void oslPngReadFn(png_structp png_ptr, png_bytep data, png_size_t length)			{
	VIRTUAL_FILE *f = (VIRTUAL_FILE *)png_get_io_ptr(png_ptr);
	VirtualFileRead(data, length, 1, f);
}

void oslPngFlushFn(png_structp png_ptr)			{
}

OSL_IMAGE *oslLoadImageFilePNG(char *filename, int location, int pixelFormat)
{
	const size_t	nSigSize=8;
	u8				signature[ nSigSize ];
	OSL_IMAGE		*img = NULL;
	VIRTUAL_FILE 	*f;
	//We only keep the location bits
	int imgLocation = location & OSL_LOCATION_MASK;
	int i;

	f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_READ);
	if (!f)
		goto error;

	if (VirtualFileRead(signature, sizeof(u8), nSigSize, f) == 0)
	{
		goto error;
	}

	if (png_check_sig(signature, nSigSize) == 0) // For libPNG < 1.4.4
	{
		goto error;
	}

	png_struct *	pPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

	if ( pPngStruct == NULL)
	{
		goto error;
	}

	png_info *	pPngInfo = png_create_info_struct( pPngStruct );

	if ( pPngInfo == NULL )
	{
		png_destroy_read_struct( &pPngStruct, NULL, NULL );
		goto error;
	}

	if ( setjmp( png_jmpbuf(pPngStruct) ) != 0 )
	{
		png_destroy_read_struct( &pPngStruct, NULL, NULL );
		goto error;
	}
	
	png_set_read_fn(pPngStruct, f, (png_rw_ptr)oslPngReadFn);
	//png_init_io( pPngStruct, &f );
	png_set_sig_bytes( pPngStruct, nSigSize );

	if (osl_pixelWidth[pixelFormat] <= 8)
		png_read_png( pPngStruct, pPngInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | /*PNG_TRANSFORM_EXPAND |*/ PNG_TRANSFORM_BGR, NULL );
	else
		png_read_png( pPngStruct, pPngInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_BGR, NULL );

	png_uint_32 width = png_get_image_width(pPngStruct, pPngInfo);
	png_uint_32 height = png_get_image_height(pPngStruct, pPngInfo);
	png_uint_32 depth = png_get_bit_depth(pPngStruct, pPngInfo);
	int color_type = png_get_color_type(pPngStruct, pPngInfo);

	png_byte **pRowTable = png_get_rows(pPngStruct, pPngInfo);
	unsigned char r=0, g=0, b=0, a=0;
	
	png_colorp palette;
	int num_palette;
	png_get_PLTE(pPngStruct, pPngInfo, &palette, &num_palette);

	int wantedPixelFormat = pixelFormat;
	
	//If we don't have a palette in the PNG but the pixel format requires one, we load the image in 32-bit mode and convert it to paletted later with oslConvertImageTo.
	if (!num_palette && osl_pixelWidth[pixelFormat] <= 8)			{
		pixelFormat = OSL_PF_8888;
		img = oslCreateImage(width, height, OSL_IN_RAM, pixelFormat);
	}
	else
		// Otherwise, we create our image normally directly
		img = oslCreateImage(width, height, imgLocation, pixelFormat);

	if (img)
	{
   		// If there is need for a palette ...
		if (osl_pixelWidth[pixelFormat] <= 8)				{

			img->palette = oslCreatePalette(oslMin(num_palette, 1 << osl_paletteSizes[pixelFormat]), OSL_PF_8888);
			if (img->palette)			{
				//Make sure to not use too much colors!
				png_set_PLTE(pPngStruct, pPngInfo, palette, num_palette);

			   //Suggestion: consider num_trans?
			   for (i = 0; i < num_palette; i++)		{
				   r = palette[i].red;
				   g = palette[i].green;
				   b = palette[i].blue;
				   a = 0xff;
				   //Color key?
				   if (osl_colorKeyEnabled && RGBA(palette[i].red, palette[i].green, palette[i].blue, 0) == (osl_colorKeyValue & 0x00ffffff))
					   a = 0;
				   ((u32*)img->palette->data)[i] = RGBA(r, g, b, a);
			   }
			   oslUncachePalette(img->palette);
			}
		}

		u32 *p_dest4 = (u32*)img->data;
		u16 *p_dest2 = (u16*)img->data;
		u8 *p_dest1 = (u8*)img->data;
		u32 x, y;
		int color_per_entry = 8 / depth;
		int color_offset, pixel_value = 0;
		int mask = (1 << depth) - 1;

		for ( y = 0; y < height; ++y )
		{
			const png_byte *	pRow = pRowTable[ y ];

			for ( x = 0; x < width; ++x )
			{
				switch ( color_type )
				{
				case PNG_COLOR_TYPE_GRAY:
					r = g = b = *pRow++;
					if ( r == 0 && g == 0 && b == 0 )	a = 0x00;
					else								a = 0xff;
					break;
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					r = g = b = *pRow++;
					if ( r == 0 && g == 0 && b == 0 )	a = 0x00;
					else								a = 0xff;
					pRow++;
					break;
				case PNG_COLOR_TYPE_RGB:
					b = *pRow++;
					g = *pRow++;
					r = *pRow++;
					if (osl_colorKeyEnabled && 
						r == (osl_colorKeyValue&0xff) &&
						g == ((osl_colorKeyValue>>8)&0xff) &&
						b == ((osl_colorKeyValue>>16)&0xff))
							a = 0, r = 0, g = 0, b = 0;
					else
							a = 0xff;
					break;
				case PNG_COLOR_TYPE_RGB_ALPHA:
					b = *pRow++;
					g = *pRow++;
					r = *pRow++;
					a = *pRow++;
					break;
				
				case PNG_COLOR_TYPE_PALETTE:
					color_offset = x % color_per_entry;
					pixel_value = (*pRow >> (8 - depth * (color_offset + 1))) & mask;
					// Last pixel for this byte
					if (x % color_per_entry == color_per_entry - 1)
						pRow++;
					if (img->palette)		{
						r = (((u32*)img->palette->data)[pixel_value]) & 0xff;
						g = ((((u32*)img->palette->data)[pixel_value]) >> 8) & 0xff;
						b = ((((u32*)img->palette->data)[pixel_value]) >> 16) & 0xff;
						a = ((((u32*)img->palette->data)[pixel_value]) >> 24) & 0xff;
					}
					else	{
						b = palette[pixel_value].blue;
						g = palette[pixel_value].green;
						r = palette[pixel_value].red;
						a = 0xff;
					}
					break;
				}

				if (pixelFormat == OSL_PF_8888)
					p_dest4[x] = RGBA(r,g,b,a);
				else if (pixelFormat == OSL_PF_5650)
					p_dest2[x] = RGB16(r,g,b);
				else if (pixelFormat == OSL_PF_5551)
					p_dest2[x] = RGBA15(r,g,b,a);
				else if (pixelFormat == OSL_PF_4444)
					p_dest2[x] = RGBA12(r,g,b,a);
				else if (pixelFormat == OSL_PF_8BIT)
					p_dest1[x] = pixel_value;
				else if (pixelFormat == OSL_PF_4BIT)				{
					p_dest1[x >> 1] &= ~(15 << ((x & 1) << 2));
					p_dest1[x >> 1] |= (pixel_value & 15) << ((x & 1) << 2);
				}
			}

			p_dest1 += ( img->realSizeX * osl_pixelWidth[pixelFormat]) >> 3;
			p_dest2 += ( img->realSizeX );
			p_dest4 += ( img->realSizeX );
		}

		//Finally convert to the paletted format
		if (wantedPixelFormat != pixelFormat)
		   img = oslConvertImageTo(img, imgLocation, wantedPixelFormat);

		png_destroy_read_struct( &pPngStruct, &pPngInfo, NULL );

		if (oslImageLocationIsSwizzled(location))
			oslSwizzleImage(img);

		oslUncacheImage(img);
	}

error:
	if (f)
		VirtualFileClose(f);
	if (!img)
		oslHandleLoadNoFailError(filename);
	return img;
}
