#include "oslib.h"

void oslUnswizzleTexture(u8* out, const u8* in, unsigned int width, unsigned int height)
{
   unsigned int blockx, blocky;
   unsigned int j;

   unsigned int width_blocks = (width / 16);
   unsigned int height_blocks = (height / 8);

   unsigned int dst_pitch = (width-16)/4;
   unsigned int dst_row = width * 8;

   u32* src = (u32*)in;
   u8* ydst = out;

#ifndef PSP
   memcpy(out, in, width * height);
   return;
#endif

   for (blocky = 0; blocky < height_blocks; ++blocky)
   {
      const u8* xdst = ydst;
      for (blockx = 0; blockx < width_blocks; ++blockx)
      {
         u32* dst= (u32*)xdst;
         for (j = 0; j < 8; ++j)
         {
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            *(dst++) = *(src++);
            dst += dst_pitch;
         }
         xdst += 16;
     }
     ydst += dst_row;
   }
}


void oslUnswizzleImage(OSL_IMAGE *img)		{
	void *block;

	//Not swizzled yet?
	if (!oslImageIsSwizzled(img))
		return;

	block = (void*)malloc(img->totalSize);
	if (!block)
		return;
	memcpy(block, img->data, img->totalSize);
	oslUnswizzleTexture((u8*)img->data, (u8*)block, (img->realSizeX * osl_pixelWidth[img->pixelFormat]) >> 3, img->realSizeY);
	free(block);

	oslUncacheImageData(img);
	oslImageIsSwizzledSet(img, 0);
}

