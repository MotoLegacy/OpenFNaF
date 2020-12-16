#include "oslib.h"

#ifdef PSP
//	unsigned int __attribute__((aligned(64))) osl_list[262144];
#else
//	unsigned int osl_list[262144];
#endif
unsigned int *osl_list = NULL;
//unsigned char *osl_vMemPtr = (u8*)(0x04088000);
//unsigned short *g_vMemPtr = (void*)(0x00100000);
//unsigned long osl_temp_texture[8*8];
//int osl_gradientTextureSampling=16;
void *osl_curTexture=NULL, *osl_curPalette=NULL;
//Remplacer par des defines?
//int osl_currentResolutionBPP=32, osl_currentPixelFormat=GU_PSM_8888;
void *osl_curDrawBuf = (void*)0, *osl_curDispBuf = (void*)0;
int osl_bilinearFilterEnabled, osl_ditheringEnabled, osl_isDrawingStarted=0;
int osl_currentAlphaEffect;
OSL_COLOR osl_currentAlphaCoeff, osl_currentAlphaCoeff2;
int osl_colorKeyEnabled, osl_colorKeyValue;
OSL_IMAGE osl_defaultBufferImage, osl_secondaryBufferImage, *osl_curBuf;
//Peut être mis à 0 pour gagner de la place, mais ce n'est pas garanti que tout fonctionne (particulièrement si le mode de texture n'est pas repeat).
int osl_alignBuffer = 3;
int osl_doubleBuffer=1;
int osl_textureEnabled = 1;
int osl_alphaTestEnabled = 0;
//ScePspFMatrix4 osl_matOrthoProjection, osl_matOrthoView, osl_matOrthoModel;
int osl_currentTexWrapU, osl_currentTexWrapV;

const int osl_pixelWidth[] = {16, 16, 16, 32, 4, 8};
//Taille des palettes (2^n)
const u8 osl_paletteSizes[]=		{
	0,									//16 bits
	0, 									//16 bits
	0, 									//16 bits
	0, 									//32 bits
	4, 									//4 bits
	8, 									//8 bits
};


void oslSetAlpha2(u32 effect, u32 coeff1, u32 coeff2)		{
	int effet;
	osl_currentAlphaEffect = effect | OSL_FX_COLOR;
	if (effect > OSL_FX_NONE)		{
		effet = effect & ~OSL_FX_COLOR;

		if (effet == OSL_FX_RGBA)		{
			sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);
			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuAmbientColor(0xffffffff);
			osl_currentAlphaCoeff = 0xffffffff;
			sceGuEnable(GU_BLEND);
			return;
		}

		if (!(effect & OSL_FX_COLOR))
			coeff1 = (coeff1 << 24) | 0xffffff;

		if (effet == OSL_FX_ALPHA)		{		//fxCoeff=0 -> transparent, fxCoeff=255 -> opaque
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuAmbientColor(coeff1);
			osl_currentAlphaCoeff = coeff1;
		}
		else if (effet == OSL_FX_ADD)	{				//fxCoeff: opacité de l'objet, fxCoeffSrc: modifie la source
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA,GU_FIX, 0, coeff2);
			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuAmbientColor(coeff1);
			osl_currentAlphaCoeff = coeff1;
			osl_currentAlphaCoeff2 = coeff2;
		}
		else if (effet == OSL_FX_SUB)		{			//fxCoeff: opacité de l'objet, fxCoeffSrc: modifie la source
			sceGuBlendFunc(GU_REVERSE_SUBTRACT, GU_SRC_ALPHA,GU_FIX, 0, coeff2);
			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuAmbientColor(coeff1);
			osl_currentAlphaCoeff = coeff1;
			osl_currentAlphaCoeff2 = coeff2;
		}

/*		if (effet == OSL_FX_FLAT)		{
			sceGuAlphaFunc(GU_GREATER, 0, 0xff);
			sceGuEnable(GU_ALPHA_TEST);
			sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
			sceGuAmbientColor(0xffffffff);
			sceGuDisable(GU_BLEND);
		}
		else		{
			sceGuDisable(GU_ALPHA_TEST);
			sceGuEnable(GU_BLEND);
		}*/
		sceGuEnable(GU_BLEND);
	}
	else		{
		osl_currentAlphaCoeff = 0xffffffff;
		sceGuDisable(GU_BLEND);
	}
}

void oslSetAlphaWrite(int action, int value1, int value2)		{
#ifdef PSP
	if (action == OSL_FXAW_SET)		{
		sceGuStencilFunc(GU_ALWAYS, value1, 0xff);
		sceGuStencilOp(GU_KEEP, GU_REPLACE, GU_REPLACE);
		sceGuEnable(GU_STENCIL_TEST);
//		sceGuAlphaFunc(GU_NOTEQUAL, value2, 0xff);
//		sceGuEnable(GU_ALPHA_TEST);
	}
	else if (action == OSL_FXAW_NONE)		{
		sceGuDisable(GU_STENCIL_TEST);
//		sceGuDisable(GU_ALPHA_TEST);
	}
#endif
}


void oslDisableTransparentColor()		{
	osl_colorKeyEnabled = 0;
	sceGuDisable(GU_COLOR_TEST);
}

void oslSetTransparentColor(OSL_COLOR color)			{
	osl_colorKeyEnabled = 1;
	osl_colorKeyValue = color;
//	sceGuColorFunc(GU_NOTEQUAL,0,0xffffffff);
	sceGuColorFunc(GU_NOTEQUAL, color, 0xffffffff);
	sceGuEnable(GU_COLOR_TEST);
}


int oslConvertColor(int pfDst, int pfSrc, int color)		{
	int r=0, g=0, b=0, a=0;
	if (pfSrc == pfDst)
		return color;
	if (pfSrc == OSL_PF_8888)
		oslRgbaGet8888(color, r, g, b, a);
	else if (pfSrc == OSL_PF_5650)
		oslRgbGet5650f(color, r, g, b), a = 0xff;
	else if (pfSrc == OSL_PF_5551)
		oslRgbaGet5551f(color, r, g, b, a);
	else if (pfSrc == OSL_PF_4444)
		oslRgbaGet4444f(color, r, g, b, a);
	if (pfDst == OSL_PF_8888)
		color = RGBA(r, g, b, a);
	else if (pfDst == OSL_PF_5650)
		color = RGB16(r, g, b);
	else if (pfDst == OSL_PF_5551)
		color = RGBA15(r, g, b, a);
	else if (pfDst == OSL_PF_4444)
		color = RGBA12(r, g, b, a);
	return color;
}

int oslConvertColorEx(OSL_PALETTE *p, int pfDst, int pfSrc, int color)		{
	int r=0, g=0, b=0, a=0;

	if (pfSrc == pfDst)
		return color;
	if (!p)
		return oslConvertColor(pfDst, pfSrc, color);

	if (pfSrc == OSL_PF_8888)
		oslRgbaGet8888(color, r, g, b, a);
	else if (pfSrc == OSL_PF_5650)
		oslRgbGet5650f(color, r, g, b), a = 0xff;
	else if (pfSrc == OSL_PF_5551)
		oslRgbaGet5551f(color, r, g, b, a);
	else if (pfSrc == OSL_PF_4444)
		oslRgbaGet4444f(color, r, g, b, a);
	else if (pfSrc == OSL_PF_8BIT || pfSrc == OSL_PF_4BIT)
		//Convert to a true color
		color = oslConvertColor(OSL_PF_8888, p->pixelFormat, oslGetPaletteColor(p, color));

	if (pfDst == OSL_PF_8888)
		color = RGBA(r, g, b, a);
	else if (pfDst == OSL_PF_5650)
		color = RGB16(r, g, b);
	else if (pfDst == OSL_PF_5551)
		color = RGBA15(r, g, b, a);
	else if (pfDst == OSL_PF_4444)
		color = RGBA12(r, g, b, a);
	else if (pfDst == OSL_PF_8BIT || pfDst == OSL_PF_4BIT)		{
		u32 i;
		for (i=0;i<p->nElements;i++)		{
			//Is it the real palette color?
			if (oslGetPaletteColor(p, i) == color)
				return i;
		}
	}

	return color;
}


//Dessine une tile de la texture sélectionnée. Eviter d'utiliser à l'extérieur.   - NOT USED ANYMORE
void oslDrawTile(int u, int v, int x, int y, int tX, int tY)
{
	OSL_FAST_VERTEX *vertices;
	vertices = (OSL_FAST_VERTEX*)sceGuGetMemory(2 * sizeof(OSL_FAST_VERTEX));

	vertices[0].u = u;
	vertices[0].v = v;
	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = 0;

	vertices[1].u = u+tX;
	vertices[1].v = v+tY;
	vertices[1].x = x+tX;
	vertices[1].y = y+tY;
	vertices[1].z = 0;

	sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
}



void oslStartDrawing()		{
	//Dessin déjà commencé?
	if (osl_isDrawingStarted)
		return;
	osl_isDrawingStarted = 1;
	osl_curTexture = NULL;
	osl_curPalette=NULL;
	sceGuStart(GU_DIRECT, osl_list);
#if 0	//<-- STAS: In fact this is not needed for intraFont to work (see graphics.c in intrafont_0.31/samples/graphics).
		//          Furthermore, all this stuff is not good here because it could interfere with the user application !
    /*SAKYA */
    if (osl_intraInit){
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective( 75.0f, 16.0f/9.0f, 0.5f, 1000.0f);

        sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();

        sceGuClearColor(0xFF000000);
        sceGuClearDepth(0);
        sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
    }
    /*END SAKYA */
#endif	//<-- STAS END -->
    oslSetAlpha(OSL_FX_RGBA, 0xff);
}

void oslEndDrawing()		{
	if (!osl_isDrawingStarted)
		return;
	sceGuFinish();
	sceGuSync(0,0);
	osl_isDrawingStarted = 0;
}

void oslSyncDrawing()			{
	if (osl_isDrawingStarted)		{
		sceGuFinish();
		sceGuSync(0,0);
		sceGuStart(GU_DIRECT,osl_list);
	}
}

void oslSetSysDisplayListSize(int newSize)		{
	//Ensure we're not dealing with the GPU!
	oslEndDrawing();
	if (osl_list)
		free(osl_list);
	//This function can cause random crashes.
	//osl_list = (unsigned int*)memalign(64, newSize);
	osl_list = (unsigned int*)memalign(16, newSize);
}

void oslInitGfx(int pixelFormat, int bDoubleBuffer) {
	u8 *baseAdr;
	bool alreadyInited = (osl_list != NULL);

	osl_doubleBuffer = bDoubleBuffer;
//	osl_currentPixelFormat = pixelFormat;
//	osl_currentResolutionBPP = osl_pixelWidth[pixelFormat];

	if (!alreadyInited) {
		oslSetupFTrigo();

		//Allocate 1 Megabyte for the display list by default. Should be enough.
		oslSetSysDisplayListSize(1 << 20);
		//oslSetSysDisplayListSize((int)1024*1024*1.5);

		sceGuInit();
	}

	// setup
	sceGuStart(GU_DIRECT, osl_list);
	sceGuDisplay(0);
	sceGuDrawBuffer(pixelFormat,(void*)0,512);
	if (bDoubleBuffer)			{
		sceGuDispBuffer(480,272,(void*)((0x22000 * osl_pixelWidth[pixelFormat])>>3),512);
		baseAdr = (u8*)(OSL_UVRAM_BASE + ((0x22000 * 2 * osl_pixelWidth[pixelFormat])>>3));

		//Displaybuffer et drawbuffer initiaux
		osl_curDrawBuf = (void*)OSL_UVRAM_BASE;
		osl_curDispBuf = (void*)(OSL_UVRAM_BASE + ((0x22000 * osl_pixelWidth[pixelFormat])>>3));
	}
	else			{
		sceGuDispBuffer(480,272,(void*)0,512);
		baseAdr = (u8*)(OSL_UVRAM_BASE + ((0x22000 * osl_pixelWidth[pixelFormat])>>3));

		//Displaybuffer et drawbuffer initiaux
		osl_curDrawBuf = (void*)OSL_UVRAM_BASE;
		osl_curDispBuf = osl_curDrawBuf;
	}

    //SAKYA OLD
    /*sceGuDepthBuffer((void*)0x110000, 512);
    sceGuDepthRange(65535, 0);
    sceGuDepthFunc(GU_GEQUAL);
    //sceGuEnable(GU_DEPTH_TEST);
    sceGuFrontFace(GU_CW);
    //sceGuShadeModel(GU_SMOOTH);
    sceGuEnable(GU_CULL_FACE);
    sceGuEnable(GU_CLIP_PLANES);
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);*/
    //END SAKYA

    //SAKYA NEW
										//<-- STAS -->
//    sceGuDepthBuffer((void*)0x110000, 512);		// Absolute address is not good here !
    sceGuDepthBuffer((void*)((u32)baseAdr - (u32)OSL_UVRAM_BASE), 512);
    baseAdr = (u8*)((u32)baseAdr + 0x22000 * 2);	// Add the Depth Buffer length
										//<-- STAS END -->
	sceGuOffset(2048 - 480/2, 2048 - 272/2);
	sceGuViewport(2048, 2048, 480, 272);
    sceGuDepthRange(65535, 0);
	sceGuScissor(0,0,480,272);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
    sceGuEnable(GU_CLIP_PLANES);
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	sceGuEnable(GU_TEXTURE_2D), osl_textureEnabled = 1;
	sceGuDisable(GU_ALPHA_TEST), osl_alphaTestEnabled = 0;
	sceGuDisable(GU_DITHER), osl_ditheringEnabled = 0;
	osl_bilinearFilterEnabled = 0;
	oslSetTextureWrap(OSL_TW_REPEAT, OSL_TW_REPEAT);
	//END SAKYA


	//Initialise le gestionnaire de VRAM
	oslVramMgrInit();
	//Définit le début de la VRAM
	oslVramMgrSetParameters((void*)baseAdr, (u32)OSL_UVRAM_END - (u32)baseAdr);

	/*sceGuDisable(GU_DEPTH_TEST);
	sceGuScissor(0,0,480,272);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuEnable(GU_TEXTURE_2D), osl_textureEnabled = 1;
	sceGuDisable(GU_ALPHA_TEST), osl_alphaTestEnabled = 0;
	sceGuTexFilter(GU_NEAREST,GU_NEAREST);
	sceGuShadeModel(GU_SMOOTH);
	sceGuDisable(GU_DITHER), osl_ditheringEnabled = 0;
	osl_bilinearFilterEnabled = 0;
	oslSetTextureWrap(OSL_TW_REPEAT, OSL_TW_REPEAT);*/

	osl_isDrawingStarted = 0;

	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(1);

	//Remplit l'image par défaut (informations en lecture seule)
	memset(&osl_defaultBufferImage, 0, sizeof(osl_defaultBufferImage));
	osl_defaultBufferImage.sizeX = 480;
	osl_defaultBufferImage.sizeY = 272;
	osl_defaultBufferImage.realSizeX = 512;
	osl_defaultBufferImage.realSizeY = 272;
	osl_defaultBufferImage.pixelFormat = pixelFormat;
	osl_defaultBufferImage.location = OSL_IN_VRAM;
	osl_defaultBufferImage.data = osl_curDrawBuf;
	osl_defaultBufferImage.sysSizeX = 512;
	osl_defaultBufferImage.sysSizeY = 512;
	osl_defaultBufferImage.totalSize = (0x22000 * osl_pixelWidth[pixelFormat])>>3;
	oslSetImageTile(&osl_defaultBufferImage, 0,0, 480,272);
	osl_curBuf = &osl_defaultBufferImage;
	//Le buffer secondaire est identique, à un détail près (le buffer)
	memcpy(&osl_secondaryBufferImage, &osl_defaultBufferImage, sizeof(osl_secondaryBufferImage));
	osl_secondaryBufferImage.data = osl_curDispBuf;
	//A partir d'ici, on peut afficher des messages (fatal error et cie)
	oslStartDrawing();
	oslSetAlpha(OSL_FX_RGBA, 0);

	if (!alreadyInited)
		oslInitConsole();

}

#if 0	//<-- STAS: this algorithm has different side effects for single and double buffering modes
void oslSwapBuffers()
{
	//Seulement si le double buffer est activé
	if (osl_doubleBuffer)			{
		oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
#ifdef PSP
		osl_curDispBuf = osl_defaultBufferImage.data;
		osl_secondaryBufferImage.data = osl_curDispBuf;
		osl_curDrawBuf = oslAddVramPrefixPtr(sceGuSwapBuffers());
#else
		//Sur PC, rien de tout ça
		oslGetUncachedPtr(sceGuSwapBuffers());
#endif
	}
	//Met à jour l'image, assumant qu'il n'y a que l'adresse qui change entre les deux buffers
	osl_defaultBufferImage.data = osl_curDrawBuf;
}

#else					//<-- STAS: much more simple and clear one...  -->
void oslSwapBuffers()
{
	if (osl_curBuf != OSL_DEFAULT_BUFFER)				// Reset the user's draw buffer to OSL_DEFAULT_BUFFER
		oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
	osl_curDispBuf = osl_curDrawBuf;					// The current draw buffer will be the display one

	if (osl_doubleBuffer) {								// Get the new draw buffer pointer
#ifdef PSP
		osl_curDrawBuf = oslAddVramPrefixPtr(sceGuSwapBuffers());
#else
		osl_curDrawBuf = oslGetUncachedPtr(sceGuSwapBuffers());
#endif
	}
	osl_defaultBufferImage.data = osl_curDrawBuf;		// Setup DEFAULT data ptr
	osl_secondaryBufferImage.data = osl_curDispBuf;		// Setup SECONDARY data ptr
}
#endif

void oslSetBilinearFilter(int enabled)
{
	osl_bilinearFilterEnabled = enabled;
	sceGuTexFilter(enabled?GU_LINEAR:GU_NEAREST, enabled?GU_LINEAR:GU_NEAREST);
}

void oslSetDithering(int enabled)
{
	osl_ditheringEnabled = enabled;
	if (enabled)
		sceGuEnable(GU_DITHER);
	else
		sceGuDisable(GU_DITHER);
}

void oslSetAlphaTest(int condition, int value)		{
	if (!osl_alphaTestEnabled)		{
		sceGuAlphaFunc(condition, value, 0xff);
		sceGuEnable(GU_ALPHA_TEST);
	}
	osl_alphaTestEnabled = 1;
}

void oslDisableAlphaTest()		{
	if (osl_alphaTestEnabled)
		sceGuDisable(GU_ALPHA_TEST);
	osl_alphaTestEnabled = 0;
}

void oslClearScreen(int backColor)		{
	sceGuClearColor(backColor);
	sceGuClear(GU_COLOR_BUFFER_BIT);
}


void oslSetScreenClipping(int x0, int y0, int x1, int y1)		{
	sceGuScissor(x0, y0, x1, y1);
	sceGuEnable(GU_SCISSOR_TEST);
}

void oslEndGfx()
{
	sceGuTerm();

	if (osl_list)
		free(osl_list);
}


void oslSetDepthTest(int enabled){
	if (enabled)
		sceGuEnable(GU_DEPTH_TEST);
	else
		sceGuDisable(GU_DEPTH_TEST);
	osl_alphaTestEnabled = enabled;
}
