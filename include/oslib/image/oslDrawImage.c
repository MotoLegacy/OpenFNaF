#include "oslib.h"
#include "vfpu.h"

/*float calc_x(int angle, int cX, int cY, int x)
{
	//cos(angle, cX) - sin(angle, cY) + x
	float vfpu_vars[8];
	register void *ptr __asm ("a0") = vfpu_vars; 
	*(int*)ptr = angle;
	*((int*)ptr+1) = cX;
	*((int*)ptr+2) = cY;
	*((int*)ptr+3) = x;
	vfpu_vars[4] = 90;
	__asm__ volatile ( 
		//Chargement des registres
		cgen_asm(lv_s(0, 0, R_a0, 0))			//r0 = angle
		cgen_asm(lv_s(1, 1, R_a0, 0))			//r1 = cX
		cgen_asm(lv_s(2, 2, R_a0, 0))			//r2 = cY
		cgen_asm(lv_s(3, 3, R_a0, 0))			//r3 = x
		cgen_asm(lv_s(4, 4, R_a0, 0))			//r4 = 90
		//Convertit les entiers (r0 à r3) en float
		cgen_asm(vi2f_s(0, 0, 0))
		cgen_asm(vi2f_s(1, 1, 0))
		cgen_asm(vi2f_s(2, 2, 0))
		cgen_asm(vi2f_s(3, 3, 0))
		//Divise l'angle par 90
		cgen_asm(vdiv_s(0, 0, 4))				//r0 = r0 / 90
		//r5 = cos(angle) * cX
		cgen_asm(vcos_s(5, 0))					//r5 = cos(r0)
		cgen_asm(vmul_s(5, 5, 1))				//r5 = r5 * r1
		//r6 = sin(angle)* cY
		cgen_asm(vsin_s(6, 0))					//r6 = sin(r0)
		cgen_asm(vmul_s(6, 6, 1))				//r6 = r6 * r1
		//Additionne le tout dans r124
		cgen_asm(vadd_s(124, 5, 6))				//r124 = r5 + r6
		cgen_asm(vadd_s(124, 124, 3))			//r124 = r124 + r3
		cgen_asm(sv_q(31, 0 * 4, R_a0, 0)) 
	: "=r"(ptr) : "r"(ptr) : "memory"); 
	return vfpu_vars[0];
}*/

void oslDrawImage(OSL_IMAGE *img)				{
		//Use the simple routine if no rotation is needed
		if (img->angle == 0 && img->centerX == 0 && img->centerY == 0)		{
			oslDrawImageSimple(img);
			return;
		}

		OSL_PRECISE_VERTEX *vertices;
//		int rsX, rsY;
		float cX, cY, tmpY;
		float uVal, xVal;
		float uCoeff;
		float xCoeff;
		float angleRadians;

		//Avoid divisions by zero
		if (img->stretchX == 0 || img->stretchY == 0)
			return;

		oslSetTexture(img);

		xVal = 0.f;
		uVal = img->offsetX0;
		angleRadians = (img->angle * 3.141592653f) / 180.f;

		//Mirrored?
		if (img->offsetX1 >= img->offsetX0)
			uCoeff = 64;
		else
			uCoeff = -64;

		//X scaling coefficient (pixels per stripe)
		xCoeff = uCoeff / ( (float)oslAbs(img->offsetX1 - img->offsetX0) / (float)img->stretchX );

		cX = (-img->centerX*img->stretchX)/(int)(img->offsetX1 - img->offsetX0);
		cY = (-img->centerY*img->stretchY)/(int)(img->offsetY1 - img->offsetY0);

		tmpY = cY + img->stretchY;
		xVal = cX;

		while (uVal != img->offsetX1)		{
			vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_PRECISE_VERTEX));

			vertices[0].u = uVal;
			vertices[0].v = img->offsetY0;
			vertices[0].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, cY) + img->x;
			vertices[0].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, cY) + img->y;
			vertices[0].z = 0;

			vertices[2].u = uVal;
			vertices[2].v = img->offsetY1;
			vertices[2].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, tmpY) + img->x;
			vertices[2].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, tmpY) + img->y;
			vertices[2].z = 0;

			uVal += uCoeff;
			xVal += xCoeff;
			if (uVal > img->offsetX1)		 {
				xVal = cX + img->stretchX;
				uVal = img->offsetX1;
			}

			vertices[1].u = uVal;
			vertices[1].v = img->offsetY0;
			vertices[1].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, cY) + img->x;
			vertices[1].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, cY) + img->y;
			vertices[1].z = 0;

			vertices[3].u = uVal;
			vertices[3].v = img->offsetY1;
			vertices[3].x = oslVfpu_cosf(angleRadians, xVal) - oslVfpu_sinf(angleRadians, tmpY) + img->x;
			vertices[3].y = oslVfpu_sinf(angleRadians, xVal) + oslVfpu_cosf(angleRadians, tmpY) + img->y;
			vertices[3].z = 0;

			sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D,4,0,vertices);
		}
}

#if 0
void oslDrawImage_new(OSL_IMAGE *img)				{
	OSL_PRECISE_VERTEX *vertices;
//		int rsX, rsY;
	int cX, cY, tmpX, tmpY;
	float uVal = img->offsetX0, xVal = 0;
	float uCoeff;
	float xCoeff = 64.0f / ( (float)oslAbs(img->offsetX1 - img->offsetX0) / (float)img->stretchX );

	//Mirrored?
	if (img->offsetX1 >= img->offsetX0)
		uCoeff = 64;
	else
		uCoeff = -64;
	xCoeff = uCoeff / ( (float)oslAbs(img->offsetX1 - img->offsetX0) / (float)img->stretchX );

	oslSetTexture(img);

	// Some matrix stuff to position the matrix and rotate it. Extremely fast though, since it uses vfpu to get the job done
	vfpu_identity_m ( &osl_matOrthoView );
	vfpu_translate_m ( &osl_matOrthoView, img->x, img->y, 0.0f );
	
	vfpu_identity_m ( &osl_matOrthoModel );
	vfpu_rotateZ_m ( &osl_matOrthoModel, img->angle );
	vfpu_translate_m ( &osl_matOrthoModel, img->centerX, img->centerY, 0.0f );

	sceGuSetMatrix ( GU_PROJECTION, &osl_matOrthoProjection );
	sceGuSetMatrix ( GU_VIEW, &osl_matOrthoView );
	sceGuSetMatrix ( GU_MODEL, &osl_matOrthoModel );

	sceGuTexScale(1.0f / ((float)img->sizeX), 1.0f / ((float)img->sizeY));

	while (uVal != img->offsetX1)		{
		vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_PRECISE_VERTEX));

		vertices[0].u = uVal;
		vertices[0].v = img->offsetY0;
		vertices[0].x = xVal;
		vertices[0].y = 0.0f;
		vertices[0].z = 0.0f;

		vertices[2].u = uVal;
		vertices[2].v = img->offsetY1;
		vertices[2].x = xVal;
		vertices[2].y = img->stretchY;
		vertices[2].z = 0.0f;

		uVal += uCoeff;
		xVal += xCoeff;

		if (uVal > img->offsetX1)		 {
			xVal = img->stretchX;
			uVal = img->offsetX1;
		}

		vertices[1].u = uVal;
		vertices[1].v = img->offsetY0;
		vertices[1].x = xVal;
		vertices[1].y = 0.0f;
		vertices[1].z = 0.0f;

		vertices[3].u = uVal;
		vertices[3].v = img->offsetY1;
		vertices[3].x = xVal;
		vertices[3].y = img->stretchY;
		vertices[3].z = 0.0f;

		sceGuDrawArray (GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);
	}
}
#endif
