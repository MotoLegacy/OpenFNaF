#include "oslib.h"

#define LARG 480
#define HAUT 272
#define TILE 8
#define max(x, y)		(((x) > (y)) ? (x) : (y))
#define min(x, y)		(((x) < (y)) ? (x) : (y))
#define TABW			(LARG/TILE)
#define TABH			(HAUT/TILE)


static void LogoDrawTiles(OSL_IMAGE *img, float positions[TABH][TABW])
{
	OSL_FAST_VERTEX *vertices;
	int nbVertices, x, y;
	oslSetTexture(img);

	for (y=0; y < HAUT/TILE; y++) 
	{
		vertices = (OSL_FAST_VERTEX*)sceGuGetMemory((LARG/TILE) * 2 * sizeof(OSL_FAST_VERTEX));
		nbVertices = 0;
		for (x=0;x<LARG/TILE;x++)
		{
			vertices[nbVertices].u = x * TILE;
			vertices[nbVertices].v = y * TILE;
			vertices[nbVertices].x = x * TILE;
			vertices[nbVertices].y = positions[y][x];
			vertices[nbVertices].z = 0;
			nbVertices ++;
			vertices[nbVertices].u = x * TILE + TILE;
			vertices[nbVertices].v = y * TILE + TILE;
			vertices[nbVertices].x = x * TILE + TILE;
			vertices[nbVertices].y = positions[y][x] + TILE;
			vertices[nbVertices].z = 0;
			nbVertices ++;
		}
		//Draw
		if (nbVertices > 0)
			sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, nbVertices, 0, vertices);
	}
}



int oslShowSplashScreen2()
{
	OSL_IMAGE *logo, *temp;
	int skip = 0;
	float y, vy;
	int frameNb, phase;
	float vyList[TABW];
	float speeds[TABH][TABW];
	int i, j, fade;
	float positions[TABH][TABW];

	temp = oslLoadImageFile("logo/neoflash.png", OSL_IN_RAM, OSL_PF_5650);
	if (!temp)
		return 0;
	logo = oslCreateSwizzledImage(temp, OSL_IN_VRAM);
	oslDeleteImage(temp);
	if (!logo)
		return 0;
	y = -logo->sizeY;
	vy = 1.0f;
	frameNb = 0;
	phase = 1;
	fade = 0;
	for (i=0;i<LARG/TILE;i++)
		vyList[i] = (rand() % 7500) / 10000.f + 0.25f;
	for (j=0;j<HAUT/TILE;j++)
		for (i=0;i<LARG/TILE;i++)		{
			speeds[j][i] = vyList[i] - (HAUT/TILE - j) * ((rand() % 1000) / 10000.0f + 0.04f);
			positions[j][i] = j * TILE;
		}


	while(!osl_quit && fade < 31 && frameNb < 220)		{
		oslReadKeys();
		if (phase < 9)		{
			y += vy;
			if (y + logo->sizeY >= HAUT)		{
				if (vy > 0)
					vy -= 0.8f;
				else
					vy -= 0.4f;
				if (phase % 2 == 1)
					phase++;
			}
			else if (phase == 1)
				vy += 0.4f;
			else		{
				if (vy > 0)
					vy += 0.4f;
				else
					vy += 0.8f;
				if (phase % 2 == 0)
					phase++;
			}
		}
		else		{
			y = 0;
			frameNb++;
		}
		if (frameNb > 120)		{
			for (j=0;j<HAUT/TILE;j++)
				for (i=0;i<LARG/TILE;i++)		{
					speeds[j][i] += 0.15f;
					if (speeds[j][i] > 0.0f)
						positions[j][i] += speeds[j][i];
				}
		}
		if (fade > 0)
			fade++;
		if ((osl_keys->pressed.value & (OSL_KEYMASK_START|OSL_KEYMASK_CIRCLE|OSL_KEYMASK_CROSS)) && fade == 0)
			fade = 1;
		if (!skip)		{
			oslStartDrawing();
			oslClearScreen(0);
			if (frameNb <= 120)		{
				logo->y = y;
				if (y < 0)		{
					logo->stretchY = logo->sizeY;
					logo->stretchX = logo->sizeX;
					logo->x = 0;
				}
				else	{
					logo->stretchY = logo->sizeY - y;
					logo->stretchX = logo->sizeX + y;
					logo->x = - y / 2;
				}

				oslDrawImage(logo);
			}
			else
				LogoDrawTiles(logo, positions);
			if (fade > 0)		{
				oslSetAlpha(OSL_FX_RGBA, 0);
				oslDrawFillRect(0, 0, LARG, HAUT, RGBA(0, 0, 0, fade<<3));
/*				oslSetAlpha(OSL_FX_SUB, 0xff);
				oslDrawFillRect(0, 0, LARG, HAUT, RGB(fade<<3, fade<<3, fade<<3));
				oslSetAlpha(OSL_FX_DEFAULT, 0);*/
			}
			oslEndDrawing();
		}
		skip = oslSyncFrameEx(1,4,0);
	}

	oslDeleteImage(logo);
	return 1;
}

