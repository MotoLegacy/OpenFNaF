#include "oslib.h"

/*
	Marche pas, le pixelformat doit être le même que le buffer
inline void DrawImageFast(IMAGE *img)
{
	sceGuCopyImage(GU_PSM_4444,0,0,img->sizeX,img->sizeY,img->sizeX,img->data,img->x,img->y,512,(void*)(0x04000000+(u32)g_curDrawBuf));
}*/


void oslDrawImageSimple(OSL_IMAGE *img)				{
		OSL_UVFLOAT_VERTEX *vertices;
		int x = img->x, y = img->y;
		//Very little overhead to support the rotation center but anyway... this routine was meant to be very simple :p
//		int x = img->x - img->centerX, y = img->y - img->centerY;

		// do a striped blit (takes the page-cache into account)
		oslSetTexture(img);
		if (oslImageGetAutoStrip(img))			{
			if (oslVerifyStripBlit(img))
				return;
		}

		vertices = (OSL_UVFLOAT_VERTEX*)sceGuGetMemory(2 * sizeof(OSL_UVFLOAT_VERTEX));

		vertices[0].u = img->offsetX0;
		vertices[0].v = img->offsetY0;
		vertices[0].x = x;
		vertices[0].y = y;
		vertices[0].z = 0;

		vertices[1].u = img->offsetX1;
		vertices[1].v = img->offsetY1;
		vertices[1].x = x + img->stretchX;
		vertices[1].y = y + img->stretchY;
		vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
}

