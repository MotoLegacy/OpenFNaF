#include "oslib.h"

//Retourne la valeur d'un pixel sur une image - lent
int oslGetImagePixel(OSL_IMAGE *img, unsigned int x, unsigned int y)			{
	void *pPixel = oslGetUncachedPtr(oslGetSwizzledPixelAddr(img, x, y));

	//Because of unsigned, we don't have to check if they're greater than 0!
	if (/*x >= 0 &&*/ x < img->sizeX /*&& y >= 0*/ && y < img->sizeY)			{
	   switch (img->pixelFormat)			{
			case OSL_PF_8888:
				return *(u32*)pPixel;

			case OSL_PF_5650:
			case OSL_PF_5551:
			case OSL_PF_4444:
				return *(u16*)pPixel;

			case OSL_PF_8BIT:
				return *(u8*)pPixel;

			case OSL_PF_4BIT:
				return *(u8*)pPixel & (15 << ((x & 1) << 2));
		}
	}
	return -1;
}
