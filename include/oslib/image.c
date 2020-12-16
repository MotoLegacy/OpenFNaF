#include "oslib.h"

int osl_autoSwizzleImages = 1;

/*
	Crée une image

	L'image fera en réalité sysSizeX*sizeY*pixelDepth octets (sizeY aligné à 8).
*/
OSL_IMAGE *oslCreateImage(int larg, int haut, short location, short pixelFormat)		{
	OSL_IMAGE *img;
	img = (OSL_IMAGE*)malloc(sizeof(OSL_IMAGE));
	if (!img)
		goto _error;
	memset(img, 0, sizeof(OSL_IMAGE));
	img->sizeX = larg;
	img->sizeY = haut;
	//Taille
	img->stretchX = larg;
	img->stretchY = haut;
	//Partie de texture
	img->offsetX0 = img->offsetY0 = 0;
	img->offsetX1 = larg, img->offsetY1 = haut;
/*	if (pixelFormat == OSL_PF_5650)
		img->alpha.fxType = OSL_FX_NONE;
	else
		img->alpha.fxType = OSL_FX_RGBA;
	img->alpha.fxCoeffSrc = 0xff;*/

	if (img->sizeX > 512)		{
		//Plus besoin d'aligner parce que la PSP ne supporte de toute façon pas les textures plus grandes
//		img->sysSizeX = (img->sizeX + 511) & ~511;
		img->sysSizeX = img->sizeX;
	}
	else
		img->sysSizeX = oslGetNextPower2(img->sizeX);

	if (img->sizeY > 512)
		//Plus besoin d'aligner parce que la PSP ne supporte de toute façon pas les textures plus grandes
//		img->sysSizeY = (img->sizeY + 511) & ~511;
		img->sysSizeY = img->sizeY;
	else
		img->sysSizeY = oslGetNextPower2(img->sizeY);

/*	img->sysSizeX = oslGetNextPower2(img->sizeX);
	img->sysSizeY = oslGetNextPower2(img->sizeY);*/

	img->location = location & OSL_LOCATION_MASK;
	img->pixelFormat = pixelFormat;
	oslImageSetAutoStrip(img, 1);
	oslImageIsSwizzledSet(img, 0);
	oslImageIsCopySet(img, 0);
	img->realSizeX = (osl_alignBuffer & 1) ? img->sysSizeX : img->sizeX;
	//[OPTIMISER] Apparemment, la largeur doit être alignée au quad-word
	if (img->realSizeX % (128/osl_pixelWidth[pixelFormat]))			{
		img->realSizeX /= (128/osl_pixelWidth[pixelFormat]);
		img->realSizeX ++;
		img->realSizeX *= (128/osl_pixelWidth[pixelFormat]);
	}
#ifdef PSP
	img->realSizeY = img->sizeY;
	//Aligner aux 8 pixels
	if (osl_alignBuffer & 2)		{
		if (img->realSizeY & 7)
			img->realSizeY = ((img->realSizeY) & ~7) + 8;
	}
#else
	img->realSizeY = img->sysSizeY;
#endif

	//Bidouille pour aligner quand même sysSizeX et sysSizeY
	if (img->sizeX > 512)
		img->sysSizeX = img->realSizeX;
	if (img->sizeY > 512)
		img->sysSizeY = img->realSizeY;

	//Taille (en octets) -> pixelWidth est exprimé en bits (multiplié par 8)
	img->totalSize = (img->realSizeX*img->realSizeY*osl_pixelWidth[img->pixelFormat])>>3;
	img->palette = NULL;

	if (!oslAllocImageData(img, location))
		goto _error;

	return img;

_error:
	if (img)
		free(img);
	return NULL;
}

/*
	Efface une image
*/
void oslDeleteImage(OSL_IMAGE *img)
{
	if (!oslImageIsCopy(img))			{
		oslFreeImageData(img);
		if (img->palette)
			oslDeletePalette(img->palette);
	}
	free(img);
	img = NULL;
}






/*
	Frees data associated to the raw image / texture.
*/
void oslFreeImageData(OSL_IMAGE *img)		{
	if (img->data)		{
		//Free memory depending on the current location
		if (img->location == OSL_IN_RAM)
        {
			free(img->data);
            img->data = NULL;
        }
		else if (img->location == OSL_IN_VRAM)
			oslVramMgrFreeBlock(img->data, img->totalSize);
	}
}

/*
	Returns NULL in case of error.

	Be very careful when modifying this function, as oslMoveImageTo relies does some tricky things with this function! Especially, don't free automatically if an image currently exists!
*/
void *oslAllocImageData(OSL_IMAGE *img, int location)		{
	location &= OSL_LOCATION_MASK;

#ifndef PSP
	location = OSL_IN_RAM;
#endif

	//If data already exists, first free it
	if (img->data)
		oslFreeImageData(img);

	//Allocate some memory for the new image
	switch (location)			{
		case OSL_IN_VRAM:
			img->data = oslVramMgrAllocBlock(img->totalSize);
			break;

		case OSL_IN_RAM:
			img->data = memalign(16, img->totalSize);
			break;

		default:
			img->data = NULL;
			break;
	}

	//Don't forget to set its location
	img->location = location;

	//Return a pointer to the new data
	return img->data;
}


void oslUncacheImage(OSL_IMAGE *img)
{
    if (img != NULL)
    {
        oslUncacheImageData(img);
        if (img->palette)
            oslUncachePalette(img->palette);
    }
}

//Copie les données d'une image vers l'autre pour autant que les deux ont des propriétés identiques
void oslCopyImageTo(OSL_IMAGE *imgDst, OSL_IMAGE *imgSrc)
{
	if (imgSrc->pixelFormat != imgDst->pixelFormat || imgSrc->totalSize > imgDst->totalSize || imgSrc->realSizeX != imgDst->realSizeX)			{
		oslFatalError("oslCopyImageTo: Both images must have the same size and pixel format!");
		return;
	}
	else		{
		//Vide les caches -> pas nécessaire si on indique à l'utilisateur de le faire avant cette copie!
		oslUncacheImageData(imgDst);
		oslUncacheImageData(imgSrc);
		sceDmacMemcpy(imgDst->data, imgSrc->data, imgSrc->totalSize);
		//Set the same swizzled flag for the destination image
		oslImageIsSwizzledSet(imgDst, oslImageIsSwizzled(imgSrc));
		oslUncacheImageData(imgDst);
//		sceGuCopyImage(imgSrc->pixelFormat,0,0,imgSrc->sizeX,imgSrc->sizeY,imgSrc->realSizeX,imgSrc->data,0,0,imgDst->realSizeX,imgDst->data);
	}
}

void oslClearImage(OSL_IMAGE *img, int color)
{
/*	if (img->location == OSL_IN_VRAM)		{
		OSL_IMAGE *db = oslGetDrawBuffer();
		int curAl = osl_currentAlphaEffect, curCo = osl_currentAlphaCoeff;
		oslSetAlpha(OSL_FX_NONE, 0);
		oslSetDrawBuffer(img);
		sceGuClearColor(color);
		sceGuClear(GU_COLOR_BUFFER_BIT);
		oslSetAlpha(curAl, curCo);
		oslSetDrawBuffer(db);
	}
	else		{*/
		u32 c64[2];
		//A TESTER!!!!
		if (img->pixelFormat == OSL_PF_5650)		{
			color = (((color&0xff0000)>>19)<<11) | (((color&0xff00)>>10)<<5) | ((color&0xff)>>3);
			color = color | (color<<16);
		}
		else if (img->pixelFormat == OSL_PF_5551)		{
			color = (((color&0xff000000)>>31)<<15) | (((color&0xff0000)>>19)<<10) | (((color&0xff00)>>11)<<5) | ((color&0xff)>>3);
			color = color | (color<<16);
		}
		else if (img->pixelFormat == OSL_PF_4444)			{
			color = (((color&0xff000000)>>28)<<12) | (((color&0xff0000)>>20)<<8) | (((color&0xff00)>>12)<<4) | ((color&0xff)>>4);
			color = color | (color<<16);
		}
		else if (img->pixelFormat == OSL_PF_8BIT)
			color = color | (color<<8) | (color<<16) | (color<<24);
		else if (img->pixelFormat == OSL_PF_4BIT)
			color = color | (color<<4) | (color<<8) | (color<<12) | (color<<16) | (color<<20) | (color<<24) | (color<<28);
		c64[0] = color;
		c64[1] = color;
		oslFasterMemset((u64*)img->data, (u64*)c64, img->totalSize);
		oslUncacheImageData(img);
//	}
}

OSL_IMAGE *oslCreateImageCopy(OSL_IMAGE *src, int newLocation)
{
	OSL_IMAGE *img;
	if (!src)
		return NULL;
	img = oslCreateImage(src->sizeX, src->sizeY, newLocation, src->pixelFormat);
	if (img)
		oslCopyImageTo(img, src);
	return img;
}

OSL_IMAGE *oslCreateSwizzledImage(OSL_IMAGE *src, int newLocation)
{
	OSL_IMAGE *img;
	if (!src)
		return NULL;
	if (oslImageIsSwizzled(src))
		return NULL;
	img = oslCreateImage(src->sizeX, src->sizeY, newLocation, src->pixelFormat);
	if (img)
		oslSwizzleImageTo(img, src);
	return img;
}

//Vérifie s'il y a besoin de faire un strip blit et le fait. Retourne 1 si le blit a été fait, 0 s'il faut blitter normalement.
//Attention: ne supporte pas la rotation ou les déformations avancées!
//ATTENTION: UTILISER LA VFPU!
int oslVerifyStripBlit(OSL_IMAGE *img)		{
	int i, ud, uf, size;
	float xd, xf, zoomX;
	//Ne tient pas dans une page
	size = (int)img->offsetX1 - (int)img->offsetX0;
	//This routine doesn't support rotation.
	if (size > OSL_SLICE_SIZE /*&& img->angle==0 && img->centerX==0 && img->centerY==0*/)			{
		OSL_PRECISE_VERTEX *vertices;

		zoomX = (float)img->stretchX/size;
		for (i=0;i<size;i+=OSL_SLICE_SIZE)			{
			vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_PRECISE_VERTEX));
			ud = i;
			if ((i + OSL_SLICE_SIZE) > size)
				uf = size;
			else
				uf = i + OSL_SLICE_SIZE;
			xd = img->x + i * zoomX;
			xf = img->x + uf * zoomX;

			vertices[0].u = img->offsetX0 + ud;
			vertices[0].v = img->offsetY0;
			vertices[0].x = xd;
			vertices[0].y = img->y;
			vertices[0].z = 0;

			vertices[1].u = img->offsetX0 + uf;
			vertices[1].v = img->offsetY0;
			vertices[1].x = xf;
			vertices[1].y = img->y;
			vertices[1].z = 0;

			vertices[2].u = img->offsetX0 + ud;
			vertices[2].v = img->offsetY1;
			vertices[2].x = xd;
			vertices[2].y = img->y + img->stretchY;
			vertices[2].z = 0;

			vertices[3].u = img->offsetX0 + uf;
			vertices[3].v = img->offsetY1;
			vertices[3].x = xf;
			vertices[3].y = img->y + img->stretchY;
			vertices[3].z = 0;

			sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D,4,0,vertices);
		}
		return 1;
	}
	else
		return 0;
}


void oslSetTexture(OSL_IMAGE *img)		{
    //int wasEnable = osl_textureEnabled;
	oslEnableTexturing();
	if (img->palette && osl_curPalette != img->palette)		{
		osl_curPalette = img->palette;
		//Change la palette
		sceGuClutMode(img->palette->pixelFormat,0,0xff,0);
		//Uploade la palette
		sceGuClutLoad((img->palette->nElements>>3), img->palette->data);
	}
	if (osl_curTexture != img->data)		{
		osl_curTexture = img->data;
		//Change la texture
		sceGuTexMode(img->pixelFormat, 0, 0, oslImageIsSwizzled(img));
//		sceGuTexFunc(GU_TFX_REPLACE, img->pixelFormat==OSL_PF_5650?GU_TCC_RGB:GU_TCC_RGBA);
		sceGuTexImage(0, img->sysSizeX, img->sysSizeY, img->realSizeX, img->data);
	}
    /*if (!wasEnable)
        oslDisableTexturing();*/
}


OSL_IMAGE *oslCreateImageTile(OSL_IMAGE *img, int offsetX0, int offsetY0, int offsetX1, int offsetY1)
{
	OSL_IMAGE *newImg;
	newImg = (OSL_IMAGE*)malloc(sizeof(OSL_IMAGE));
	if (!newImg)
		return NULL;
	memcpy(newImg, img, sizeof(OSL_IMAGE));
	oslImageIsCopySet(newImg, 1);
	oslSetImageTile(newImg, offsetX0, offsetY0, offsetX1, offsetY1);
	return newImg;
}

OSL_IMAGE *oslCreateImageTileSize(OSL_IMAGE *img, int offsetX0, int offsetY0, int width, int height)
{
	OSL_IMAGE *newImg;
	newImg = (OSL_IMAGE*)malloc(sizeof(OSL_IMAGE));
	if (!newImg)
		return NULL;
	memcpy(newImg, img, sizeof(OSL_IMAGE));
	oslImageIsCopySet(newImg, 1);
	oslSetImageTileSize(newImg, offsetX0, offsetY0, width, height);
	return newImg;
}


void oslSetImageFrame(OSL_IMAGE *img, int frame)		{
	u32 framesPerLine = img->sizeX / img->frameSizeX;
	u32 line = frame / framesPerLine;

	img->offsetX0 = (frame - line * framesPerLine) * img->frameSizeX;
	img->offsetY0 = line * img->frameSizeY;
	img->offsetX1 = img->offsetX0 + img->frameSizeX;
	img->offsetY1 = img->offsetY0 + img->frameSizeY;
	img->stretchX = img->frameSizeX;
	img->stretchY = img->frameSizeY;
}

int oslImageLocationIsSwizzled(int location)		{
	return ((osl_autoSwizzleImages && !(location & OSL_UNSWIZZLED)) || (location & OSL_SWIZZLED));
}

