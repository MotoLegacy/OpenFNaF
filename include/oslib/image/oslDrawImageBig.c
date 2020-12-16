#include "oslib.h"
#include "vfpu.h"

#define TEXSIZEX_LIMIT 512
#define TEXSIZEX_LIMITF 512.0f
#define TEXSIZEY_LIMIT 512
#define TEXSIZEY_LIMITF 512.0f

void oslSetTexturePart(OSL_IMAGE *img, int x, int y)		{
	u8 *data = (u8*)img->data;

	//To adjust the texture offset when swizzling is enabled (with a normal factor, the image appears trashed)
#ifdef PSP
	int swizzleScaleFactor = oslImageIsSwizzled(img) ? 8 : 1;
#else
	int swizzleScaleFactor = 1;
#endif

	oslEnableTexturing();
	if (img->palette && osl_curPalette != img->palette)		{
		osl_curPalette = img->palette;
		//Change la palette
		sceGuClutMode(img->palette->pixelFormat,0,0xff,0);
		//Uploade la palette
		sceGuClutLoad((img->palette->nElements>>3), img->palette->data);
	}

	data = (u8*)oslGetImagePixelAdr(img, x * swizzleScaleFactor, y);
	if (osl_curTexture != data)		{
		osl_curTexture = data;
		//Change la texture
		sceGuTexMode(img->pixelFormat, 0, 0, oslImageIsSwizzled(img));
		sceGuTexImage(0, TEXSIZEX_LIMIT, TEXSIZEY_LIMIT, img->realSizeX, data);
/*		extern void sendCommandi(u32, u32);
        sendCommandi(0xa0, ((unsigned int)data) & 0xffffff);
        sendCommandi(0xa8, ((((unsigned int)data) >> 8) & 0x0f0000) | img->realSizeX);
        sendCommandi(0xb8, (9 << 8) | 9);
		sceGuTexFlush();*/
	}
}

void oslDrawImageBig(OSL_IMAGE *img)		{
	OSL_PRECISE_VERTEX *vertices = NULL;
//		int rsX, rsY;
	float cX, cY, tmpY;
	float uVal, xVal;
	const float uCoeff = 64.0f, vCoeff = TEXSIZEY_LIMITF;
	float xCoeff, yCoeff;
	float angleRadians;
	float offsetX0 = img->offsetX0, offsetX1 = img->offsetX1, offsetY0 = img->offsetY0, offsetY1 = img->offsetY1;
	float uLimit, vLimit, vVal;
	int uStart, vStart;
	bool flipH = 0, flipV = 0;
	bool lastTexWrapU = osl_currentTexWrapU, lastTexWrapV = osl_currentTexWrapV;
	int nDiff;

	//No need to use this huge routine?
	if (img->sizeX <= 512 && img->sizeY <= 512)		{
		oslDrawImage(img);
		return;
	}

	//Handle mirroring
	if (offsetX0 > offsetX1)		{
		float tmp = offsetX0;
		offsetX0 = offsetX1;
		offsetX1 = tmp;
		flipH = 1;
	}
	if (offsetY0 > offsetY1)		{
		float tmp = offsetY0;
		offsetY0 = offsetY1;
		offsetY1 = tmp;
		flipV = 1;
	}

	xVal = 0.f;
	angleRadians = (img->angle * 3.141592653f) / 180.f;

	//X scaling coefficient (pixels per stripe)
	xCoeff = uCoeff / ( (float)/*oslAbs*/(offsetX1 - offsetX0) / (float)img->stretchX );
	//Y scaling coefficient (pixels per block)
	yCoeff = vCoeff / ( (float)/*oslAbs*/(offsetY1 - offsetY0) / (float)img->stretchY );

	cX = (-img->centerX * img->stretchX)/(int)(offsetX1 - offsetX0);
	cY = (-img->centerY * img->stretchY)/(int)(offsetY1 - offsetY0);

	if ((int)offsetY0 >= 0)
		nDiff = (int)offsetY0 % TEXSIZEY_LIMIT;
	else
		nDiff = TEXSIZEY_LIMIT + (int)offsetY0 % TEXSIZEY_LIMIT;
	vStart = offsetY0 - nDiff;
	vVal = offsetY0;

	if (offsetY1 - vStart  >= TEXSIZEY_LIMITF)
		vLimit = vStart + TEXSIZEY_LIMITF;
	else
		vLimit = offsetY1;

	//Ici yCoeff est toujours positif!
	if (flipV)		{
//		cY += (yCoeff * img->stretchY) / vCoeff;
		cY += img->stretchY;
		tmpY = cY - ((vLimit - vVal) * img->stretchY) / (int)(offsetY1 - offsetY0);
	}
	else
		tmpY = cY + ((vLimit - vVal) * img->stretchY) / (int)(offsetY1 - offsetY0);

	if (flipH)
		xCoeff = -xCoeff;
	if (flipV)
		yCoeff = -yCoeff;

	//We need to clamp the texture to its end.
	oslSetTextureWrap(OSL_TW_CLAMP, OSL_TW_CLAMP);

	do		{
		
		//For each line...
		uVal = offsetX0;

		xVal = cX;

		//Ici xCoeff est toujours négatif
		if (flipH)
			xVal += img->stretchX;
//			xVal -= (xCoeff * img->stretchX) / uCoeff;

		//We should not get further than the texture width limit
		if ((int)offsetX0 >= 0)
			nDiff = (int)offsetX0 % TEXSIZEX_LIMIT;
		else
			nDiff = TEXSIZEX_LIMIT + (int)offsetX0 % TEXSIZEX_LIMIT;
		uStart = offsetX0 - nDiff;
		//Mince! Une division par un nombre négatif inférieur à 512 donne un résultat de 0...


		if (offsetX1 - uStart >= TEXSIZEX_LIMITF)
			uLimit = uStart + TEXSIZEX_LIMITF;
		else
			uLimit = offsetX1;

		do		{
			int bDrawn = 1;

			//Out of bounds => do not draw!
			if (uStart >= 0 && uStart < img->sizeX && vStart >= 0 && vStart < img->sizeY)
				oslSetTexturePart(img, uStart, vStart);
			else
				bDrawn = 0;

			while (uVal < uLimit)		{

				if (bDrawn)		{
					vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_PRECISE_VERTEX));

					vertices[0].u = uVal - uStart;
					vertices[0].v = vVal - vStart;
					vertices[0].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, cY) + img->x;
					vertices[0].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, cY) + img->y;
					vertices[0].z = 0;

					vertices[2].u = uVal - uStart;
					vertices[2].v = vLimit - vStart;
					vertices[2].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, tmpY) + img->x;
					vertices[2].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, tmpY) + img->y;
					vertices[2].z = 0;
				}

				//Plus loin que la limite?
				if (uVal + uCoeff >= uLimit)		{
					xVal += ((uLimit - uVal) * xCoeff) / uCoeff;
					uVal += uLimit - uVal;
				}
				else	{
					uVal += uCoeff;
					xVal += xCoeff;
				}
				//Plus loin que l'offset max?
/*				if (uVal >= offsetX1)		 {
					if (flipH)
						xVal = 0;
					else
						xVal = cX + img->stretchX;
					uVal = offsetX1;
				}*/

				if (bDrawn)		{
					vertices[1].u = uVal - uStart;
					vertices[1].v = vVal - vStart;
					vertices[1].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, cY) + img->x;
					vertices[1].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, cY) + img->y;
					vertices[1].z = 0;

					vertices[3].u = uVal - uStart;
					vertices[3].v = vLimit - vStart;
					vertices[3].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, tmpY) + img->x;
					vertices[3].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, tmpY) + img->y;
					vertices[3].z = 0;

					sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D,4,0,vertices);
				}
			}

			//Is there any pixel left to draw?
			if (uLimit < offsetX1)		{
				uLimit += TEXSIZEX_LIMITF;
				uStart += TEXSIZEX_LIMIT;
			}
			else
				break;

			if (uLimit >= offsetX1)
				uLimit = offsetX1;
		
		} while (1);

		float oldVl = vLimit;

		//Is there any lines left?
		if (vLimit < offsetY1)		{
			vLimit += TEXSIZEY_LIMITF;
			vStart += TEXSIZEY_LIMIT;
		}
		else
			break;
		
		//We are too far?
		if (vLimit >= offsetY1)
			vLimit = offsetY1;

		//Advance our coefficients
		vVal += (tmpY - cY) * TEXSIZEY_LIMITF / yCoeff;
		cY += (tmpY - cY);
		tmpY += (vLimit - oldVl) * yCoeff / TEXSIZEY_LIMITF;

	} while (1);

	//Restore the texture wrapping
	oslSetTextureWrap(lastTexWrapU, lastTexWrapV);
}

