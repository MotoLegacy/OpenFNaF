#include "oslib.h"

void oslSwizzleTexture(u8* out, const u8* in, unsigned int width, unsigned int height)
{
   unsigned int blockx, blocky;
   unsigned int j;
 
   unsigned int width_blocks = (width / 16);
   unsigned int height_blocks = (height / 8);
 
   unsigned int src_pitch = (width-16)/4;
   unsigned int src_row = width * 8;
 
   const u8* ysrc = in;
   u32* dst = (u32*)out;
 
#ifndef PSP
   memcpy(out, in, width * height);
   return;
#endif
   for (blocky = 0; blocky < height_blocks; ++blocky)
   {
      const u8* xsrc = ysrc;
      for (blockx = 0; blockx < width_blocks; ++blockx)
      {
         const u32* src = (u32*)xsrc;
         for (j = 0; j < 8; ++j)
         {
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            src += src_pitch;
         }
         xsrc += 16;
     }
     ysrc += src_row;
   }
}

void *oslGetSwizzledPixelAddr(OSL_IMAGE *img, unsigned int x, unsigned int y)
{
	if (!oslImageIsSwizzled(img))
		return oslGetImagePixelAdr(img, x, y);

	//Multiply the x value by the pixelformat
	x = (x * osl_pixelWidth[img->pixelFormat]) >> 3;

	unsigned int width = (img->realSizeX * osl_pixelWidth[img->pixelFormat]) >> 3;
	unsigned int rowblocks = (width / 16);
 
	unsigned int blockx = x / 16;
	unsigned int blocky = y / 8;

	unsigned int block_index = blockx + ((blocky) * rowblocks);
	unsigned int block_address = block_index * 16 * 8;
 
	x = (x - blockx * 16);
	y = (y - blocky * 8);

	return (void*)((u8*)img->data + block_address + x + y * 16);
}

void oslSwizzleImage(OSL_IMAGE *img)		{
	void *block;

	//Already swizzled?
	if (oslImageIsSwizzled(img))
		return;

	block = (void*)malloc(img->totalSize);
	if (block)		{
		memcpy(block, img->data, img->totalSize);
		oslSwizzleTexture((u8*)img->data, (u8*)block, (img->realSizeX * osl_pixelWidth[img->pixelFormat]) >> 3, img->realSizeY);
		free(block);

		oslUncacheImageData(img);
		oslImageIsSwizzledSet(img, 1);
	}
}

void oslSwizzleImageTo(OSL_IMAGE *imgDst, OSL_IMAGE *imgSrc)
{
	//Même image?
	if (imgDst == imgSrc)
		oslSwizzleImage(imgDst);

	if (imgSrc->pixelFormat != imgDst->pixelFormat || imgSrc->totalSize != imgDst->totalSize)			{
		oslFatalError("oslSwizzleImageTo: Both images must have the same size and pixel format!");
		return;
	}
	else		{
		oslSwizzleTexture((u8*)imgDst->data, (u8*)imgSrc->data, (imgSrc->realSizeX*osl_pixelWidth[imgSrc->pixelFormat])>>3, imgSrc->realSizeY);
		oslUncacheImageData(imgDst);
		oslImageIsSwizzledSet(imgDst, 1);
	}
}

