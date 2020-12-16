#include "oslib.h"

/*OSL_COLOR oslBlendColor(OSL_COLOR c)		{
	if (osl_currentAlphaCoeff == 0xffffffff)
		return c;

	//Old method - only for 32-bit processors
	#if 0
		OSL_COLOR a1 = c & 0xffff, a2 = c >> 16, b1 = osl_currentAlphaCoeff & 0xffff, b2 = osl_currentAlphaCoeff >> 16, c1, c2;
		c1 =
			(((a1 & 0xff00) * (b1 & 0xff00)) >> 16) |
			(((a1 & 0xff) * (b1 & 0xff)) >> 8);
		c2 =
			(((a2 & 0xff00) * (b2 & 0xff00)) >> 16) |
			(((a2 & 0xff) * (b2 & 0xff)) >> 8);
		return c1 | c2 << 16;
	#else
		//Nouvelle méthode
		if ((osl_currentAlphaCoeff & 0x00ffffff) == 0x00ffffff)		{
			//Pas de tinting, c'est plus simple
			return (c & 0xffffff) | (((c >> 24) * (osl_currentAlphaCoeff >> 24) >> 8) << 24);
		}
		else	{
			return
				((((u64)(c & 0xff000000) * (osl_currentAlphaCoeff & 0xff000000)) >> 32) & 0xff000000) |
				((((u64)(c & 0xff0000) * (osl_currentAlphaCoeff & 0xff0000)) >> 24) & 0xff0000) |
				((((c & 0xff00) * (osl_currentAlphaCoeff & 0xff00)) >> 16) && 0xff00) |
				(((c & 0xff) * (osl_currentAlphaCoeff & 0xff)) >> 8);
		}
	#endif
}*/

OSL_COLOR oslBlendColors(OSL_COLOR c1, OSL_COLOR c2)		{
	if (c2 == 0xffffffff)
		return c1;

	//Old method - only for 32-bit processors
	#if 0
		OSL_COLOR a1 = c & 0xffff, a2 = c >> 16, b1 = osl_currentAlphaCoeff & 0xffff, b2 = osl_currentAlphaCoeff >> 16, c1, c2;
		c1 =
			(((a1 & 0xff00) * (b1 & 0xff00)) >> 16) |
			(((a1 & 0xff) * (b1 & 0xff)) >> 8);
		c2 =
			(((a2 & 0xff00) * (b2 & 0xff00)) >> 16) |
			(((a2 & 0xff) * (b2 & 0xff)) >> 8);
		return c1 | c2 << 16;
	#else
		//Nouvelle méthode
		if ((c2 & 0x00ffffff) == 0x00ffffff)		{
			//Pas de tinting, c'est plus simple
			return (c1 & 0xffffff) | (((c1 >> 24) * (c2 >> 24) >> 8) << 24);
		}
		else	{
			return
				((((u64)(c1 & 0xff000000) * (c2 & 0xff000000)) >> 32) & 0xff000000) |
				((((u64)(c1 & 0xff0000) * (c2 & 0xff0000)) >> 24) & 0xff0000) |
				((((c1 & 0xff00) * (c2 & 0xff00)) >> 16) && 0xff00) |
				(((c1 & 0xff) * (c2 & 0xff)) >> 8);
		}
	#endif
}

//Optionnel: image décrivant les effets (alpha, ...)
//Sinon laisse le dernier effet utilisé
void oslDrawLine(int x0, int y0, int x1, int y1, OSL_COLOR color)		{
		OSL_LINE_VERTEX* vertices;

		vertices = (OSL_LINE_VERTEX*)sceGuGetMemory(2 * sizeof(OSL_LINE_VERTEX));
		color = oslBlendColor(color);

		vertices[0].color = color;
		vertices[0].x = x0;
		vertices[0].y = y0;
		vertices[0].z = 0;

		vertices[1].color = color;
		vertices[1].x = x1;
		vertices[1].y = y1;
		vertices[1].z = 0;

		int wasEnable = osl_textureEnabled;
		oslDisableTexturing();
		sceGuDrawArray(GU_LINES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);
        sceKernelDcacheWritebackRange(vertices, 2 * sizeof(OSL_LINE_VERTEX)); //SAKYA
		if (wasEnable)
			oslEnableTexturing();
}

void oslDrawRect(int x0, int y0, int x1, int y1, OSL_COLOR color)		{
		OSL_LINE_VERTEX* vertices;
		int i;

		vertices = (OSL_LINE_VERTEX*)sceGuGetMemory(8 * sizeof(OSL_LINE_VERTEX));
		color = oslBlendColor(color);

		//Swap si nécessaire
		if (x1<x0)		{
			i=x0;
			x0=x1;
			x1=i;
		}
		if (y1<y0)		{
			i=y0;
			y0=y1;
			y1=i;
		}

		vertices[0].color = color;
		vertices[0].x = x0;
		vertices[0].y = y0+1;
		vertices[0].z = 0;

		vertices[1].color = color;
		vertices[1].x = x0;
		vertices[1].y = y1;
		vertices[1].z = 0;

		vertices[2].color = color;
		vertices[2].x = x0+1;
		vertices[2].y = y1-1;
		vertices[2].z = 0;

		vertices[3].color = color;
		vertices[3].x = x1;
		vertices[3].y = y1-1;
		vertices[3].z = 0;

		vertices[4].color = color;
		vertices[4].x = x1-1;
		vertices[4].y = y0;
		vertices[4].z = 0;

		vertices[5].color = color;
		vertices[5].x = x1-1;
		vertices[5].y = y1-1;
		vertices[5].z = 0;

		vertices[6].color = color;
		vertices[6].x = x0;
		vertices[6].y = y0;
		vertices[6].z = 0;

		vertices[7].color = color;
		vertices[7].x = x1-1;
		vertices[7].y = y0;
		vertices[7].z = 0;

		int wasEnable = osl_textureEnabled;
		oslDisableTexturing();

		sceGuDrawArray(GU_LINES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 8, 0, vertices);
    	sceKernelDcacheWritebackRange(vertices, 8 * sizeof(OSL_LINE_VERTEX)); //SAKYA
		if (wasEnable)
			oslEnableTexturing();
}

void oslDrawFillRect(int x0, int y0, int x1, int y1, OSL_COLOR color)		{
    OSL_LINE_VERTEX* vertices;

    vertices = (OSL_LINE_VERTEX*)sceGuGetMemory(2 * sizeof(OSL_LINE_VERTEX));
    color = oslBlendColor(color);

    vertices[0].color = color;
    vertices[0].x = x0;
    vertices[0].y = y0;
    vertices[0].z = 0;

    vertices[1].color = color;
    vertices[1].x = x1;
    vertices[1].y = y1;
    vertices[1].z = 0;

    int wasEnable = osl_textureEnabled;
    oslDisableTexturing();

    sceGuDrawArray(GU_SPRITES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);
	sceKernelDcacheWritebackRange(vertices, 2 * sizeof(OSL_LINE_VERTEX)); //SAKYA
    if (wasEnable)
        oslEnableTexturing();
}

//Va de 1*couleur1 à 15/16*couleur2 + 1/16*couleur1 (pas jusqu'au fond, limite hardware). Idem pour couleur2 et couleur4. Dessiner un rectangle à l'envers pour prioriser l'autre coin (couleur1 ou couleur3), càd x1,y0,x0,y1.
void oslDrawGradientRect(int x0, int y0, int x1, int y1, OSL_COLOR c1, OSL_COLOR c2, OSL_COLOR c3, OSL_COLOR c4)
{
	OSL_LINE_VERTEX* vertices;

	vertices = (OSL_LINE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_LINE_VERTEX));
	c1 = oslBlendColor(c1);
	c2 = oslBlendColor(c2);
	c3 = oslBlendColor(c3);
	c4 = oslBlendColor(c4);

//	sceGuShadeModel(GU_SMOOTH);

	vertices[0].color = c1;
	vertices[0].x = x0;
	vertices[0].y = y0;
	vertices[0].z = 0;

	vertices[1].color = c2;
	vertices[1].x = x1;
	vertices[1].y = y0;
	vertices[1].z = 0;

	vertices[2].color = c3;
	vertices[2].x = x0;
	vertices[2].y = y1;
	vertices[2].z = 0;

	vertices[3].color = c4;
	vertices[3].x = x1;
	vertices[3].y = y1;
	vertices[3].z = 0;

    int wasEnable = osl_textureEnabled;
    oslDisableTexturing();

	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);
	sceKernelDcacheWritebackRange(vertices, 4 * sizeof(OSL_LINE_VERTEX)); //SAKYA
    if (wasEnable)
        oslEnableTexturing();

}

