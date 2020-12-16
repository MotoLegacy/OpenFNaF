#include "oslib.h"

extern inline int oslFindColorInPalette(OSL_PALETTE *pal, int count, OSL_COLOR color)		{
   int i;

   for (i=0;i<count;i++)		{
	   //Get a 32 bit value for the current palette entry
	   u32 pixel = oslConvertColor(OSL_PF_8888, pal->pixelFormat, oslGetPaletteColor(pal, i));
	   if (pixel == color)
		   return i;
   }
   return -1;
}

OSL_IMAGE *oslConvertImageTo(OSL_IMAGE *imgOriginal, int newLocation, int newFormat)			{
	int palSize = 1 << osl_paletteSizes[newFormat];
	int width = imgOriginal->sizeX, height = imgOriginal->sizeY;
	OSL_IMAGE *img;
	int i, j;
	int palCount;

	img = oslCreateImage(width, height, newLocation, newFormat);
	if (osl_pixelWidth[newFormat] <= 8)
		img->palette = oslCreatePalette(palSize, OSL_PF_8888);

	if (img)			{
		//D'abord: tout à zéro
		memset(img->data, 0, img->totalSize);
		palCount = 0;

		//On parcourt toute l'image pour créer les index de la palette
		for (j=0;j<height;j++)		{
			for (i=0;i<width;i++)			{
				u32 pixel;
				//In paletted mode, we need a bit of work
				if (osl_pixelWidth[newFormat] <= 8)			{
					u32 *palette = (u32*)img->palette->data;

					//Get a 32 bit value for the pixel
					pixel = oslConvertColorEx(imgOriginal->palette, OSL_PF_8888, imgOriginal->pixelFormat, oslGetImagePixel(imgOriginal, i, j));

					//Does the color already exist?
					int colorNb = oslFindColorInPalette(img->palette, palCount, pixel);

					if (colorNb < 0)			{
						if (palCount < palSize)			{
							palette[palCount] = pixel;
							colorNb = palCount++;
						}
						//Pas assez de couleurs, on pourrait essayer d'approcher au max mais le résultat serait assez mauvais parce que seules les couleurs des premiers pixels seraient disponibles.
						else
							colorNb = 0;
					}
					//We can finally write the color
					oslSetImagePixel(img, i, j, colorNb);
				}
				else	{
					//True color mode
					pixel = oslConvertColorEx(imgOriginal->palette, newFormat, imgOriginal->pixelFormat, oslGetImagePixel(imgOriginal, i, j));
					oslSetImagePixel(img, i, j, pixel);
				}
			}
		}
		oslDeleteImage(imgOriginal);
		oslUncacheImage(img);
	}
	return img;
}

