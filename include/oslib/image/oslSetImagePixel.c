#include "oslib.h"

//Dessine un pixel sur une image - lent
void oslSetImagePixel(OSL_IMAGE *img, unsigned int x, unsigned int y, int pixelValue)			{
	void *pPixel = oslGetUncachedPtr(oslGetSwizzledPixelAddr(img, x, y));

	//Clipping - use of unsigned avoids checking if it's greater than 0.
	if (/*x >= 0 &&*/ x < img->sizeX && /*y >= 0 &&*/ y < img->sizeY)			{
		switch (img->pixelFormat)			{
			case OSL_PF_8888:
				*(u32*)pPixel = pixelValue;
				break;

			case OSL_PF_5650:
			case OSL_PF_5551:
			case OSL_PF_4444:
				*(u16*)pPixel = pixelValue;
				break;

			case OSL_PF_8BIT:
				*(u8*)pPixel = pixelValue;
				break;

			case OSL_PF_4BIT:
				*(u8*)pPixel &= ~(15 << ((x & 1) << 2));
				*(u8*)pPixel |= (pixelValue & 15) << ((x & 1) << 2);
				break;
		}
	}
}
