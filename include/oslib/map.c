#include "oslib.h"

OSL_MAP *oslCreateMap(OSL_IMAGE *img, void *map_data, int tileX, int tileY, int mapSizeX, int mapSizeY, int map_format)
{
	OSL_MAP *m;
	m = (OSL_MAP*)malloc(sizeof(OSL_MAP));
	if (!m)
		return NULL;
	if (map_format != OSL_MF_U16 && map_format != OSL_MF_U16_GBA)		{
		oslFatalError("Invalid map format");
		free(m);
		return NULL;
	}
	m->format = map_format;
	//For the GBA format, we set the number of tiles bits to 10 by default. You can increase it up to 14.
	if (map_format == OSL_MF_U16_GBA)
		m->addit1 = 10;

	//By default, the first tile is always transparent
	m->flags = OSL_MF_TILE1_TRANSPARENT;
	//Does the map satisfy criterias to be treated as simple (drawn faster)?
//	if (m->tileX == oslGetNextPower2(m->tileX) && m->tileY == oslGetNextPower2(m->tileY) && m->img->sizeX == oslGetNextPower2(m->img->sizeX))
//		m->flags |= OSL_MF_SIMPLE;

	m->img = img;
	m->map = map_data;
	m->tileX = tileX;
	m->tileY = tileY;
	m->mapSizeX = mapSizeX;
	m->mapSizeY = mapSizeY;
	m->scrollX = m->scrollY = 0;
	m->drawSizeX = -1;
	m->drawSizeY = -1;
	return m;
}

//tileX, tileY, img->sizeX doivent être des puissances de deux, et la première tile est toujours transparente!
void oslDrawMapSimple(OSL_MAP *m)
{
/*	int x, y, v, ttX=0, i, sX, sY, tX=0, tY=0, mX, mY, dX, bY, dsX, dsY;
	u16 *map = (u16*)m->map;
	OSL_FAST_VERTEX *vertices, *vptr;
	int nbVertices;

	oslSetTexture(m->img);
	//Calcule le drawSize s'il n'est pas fourni
	if (m->drawSizeX < 0 || m->drawSizeY < 0)			{
		dsX = osl_curBuf->sizeX/m->tileX+1;
		if (osl_curBuf->sizeX%m->tileX)		dsX++;
		dsY = osl_curBuf->sizeY/m->tileY+1;
		if (osl_curBuf->sizeY%m->tileY)		dsY++;
	}
	else
		dsX = m->drawSizeX, dsY = m->drawSizeY;

	//Trouve les puissances de deux optimisées
	for (i=3;i<=9;i++)			{
		if (ttX == 0 && m->img->sysSizeX <= 1<<i)
			ttX = i;
		if (tX == 0 && m->tileX <= 1<<i)
			tX = i;
		if (tY == 0 && m->tileY <= 1<<i)
			tY = i;
	}

	sX = m->scrollX&((1<<tX)-1);
	sY = m->scrollY&((1<<tY)-1);
	dX = (m->scrollX>>tX)%m->mapSizeX;
	mY = (m->scrollY>>tY)%m->mapSizeY;
	//Pour parer au modulo négatif
	if (dX < 0)		dX += m->mapSizeX;
	if (mY < 0)		mY += m->mapSizeY;

	for (y=0;y<dsY;y++)			{
		bY = m->mapSizeX*mY;
		mX = dX;
		vertices = (OSL_FAST_VERTEX*)sceGuGetMemory(dsX * 2 * sizeof(OSL_FAST_VERTEX));
		vptr = vertices;
		nbVertices = 0;
		for (x=0;x<dsX;x++)			{
			v = map[bY+mX];
			//Plus optimisé
			if (v)				{
				vertices[nbVertices].u = ((v<<tX)&((1<<ttX)-1));
				vertices[nbVertices].v = ((v<<tX)>>ttX)<<tY;
				vertices[nbVertices].x = (x<<tX)-sX;
				vertices[nbVertices].y = (y<<tY)-sY;
				vertices[nbVertices].z = 0;
				nbVertices ++;
				vertices[nbVertices].u = ((v<<tX)&((1<<ttX)-1)) + m->tileX;
				vertices[nbVertices].v = (((v<<tX)>>ttX)<<tY) + m->tileY;
				vertices[nbVertices].x = ((x<<tX)-sX) + m->tileX;
				vertices[nbVertices].y = ((y<<tY)-sY) + m->tileY;
				vertices[nbVertices].z = 0;
				nbVertices ++;
			}
			mX++;
			if (mX >= m->mapSizeX)
				mX -= m->mapSizeX;
		}
		//Dessine
		if (nbVertices > 0)
			sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, nbVertices, 0, vertices);
		mY++;
		if (mY >= m->mapSizeY)
			mY -= m->mapSizeY;
	}*/
	oslDrawMap(m);
}

void oslDrawMap(OSL_MAP *m)
{
	int x, y, v, sX, sY, mX, mY, dX, bY, dsX, dsY, xTile, yTile, i;
	u32 tilesPerLine = m->img->sizeX / m->tileX, tilesPerLineOpt = 0;
	u32 firstTileOpaque = !(m->flags & OSL_MF_TILE1_TRANSPARENT);
	u16 *map = (u16*)m->map;
	OSL_FAST_VERTEX *vertices;
	int nbVertices;

	oslSetTexture(m->img);
	if (m->drawSizeX < 0 || m->drawSizeY < 0)			{
		dsX = osl_curBuf->sizeX/m->tileX+1;
		if (osl_curBuf->sizeX%m->tileX)		dsX++;
		dsY = osl_curBuf->sizeY/m->tileY+1;
		if (osl_curBuf->sizeY%m->tileY)		dsY++;
	}
	else
		dsX = m->drawSizeX, dsY = m->drawSizeY;

	//Try to optimize for shifting. From 1 to 256 tiles per line, should be a wide enough range.
	for (i=1;i<=8;i++)			{
		if (tilesPerLine >= 1<<i)
			tilesPerLineOpt = i;
	}

	sX = m->scrollX%m->tileX;
	sY = m->scrollY%m->tileY;
	//Pour parer au modulo négatif
	if (sX < 0)		sX += m->tileX;
	if (sY < 0)		sY += m->tileY;
	dX = (((m->scrollX)<0?(m->scrollX-m->tileX+1):(m->scrollX))/m->tileX)%m->mapSizeX;
	mY = (((m->scrollY)<0?(m->scrollY-m->tileY+1):(m->scrollY))/m->tileY)%m->mapSizeY;
	//Pour parer au modulo négatif
	if (dX < 0)		dX += m->mapSizeX;
	if (mY < 0)		mY += m->mapSizeY;

	yTile = -sY;

	switch (m->format)			{
		case OSL_MF_U16:
			for (y=0;y<dsY;y++)			{
				bY = m->mapSizeX * mY;
				mX = dX;
				xTile = -sX;
				vertices = (OSL_FAST_VERTEX*)sceGuGetMemory(dsX * 2 * sizeof(OSL_FAST_VERTEX));
				nbVertices = 0;
				if (tilesPerLineOpt)			{
					for (x=0;x<dsX;x++)			{
						v = map[bY+mX];
						//Plus compréhensible
						if (v || firstTileOpaque)			{
							//Optimized with shift & and. Multiplication doesn't need to be optimized as it requires 12 cycles (2 cycles + 10 overlap). Division requires 75 cycles instead. DDIV is 135, and DMULT is 2 + 18.
							vertices[nbVertices].u = (v & ((1 << tilesPerLineOpt) - 1)) * m->tileX;
							vertices[nbVertices].v = (v >> tilesPerLineOpt) * m->tileY;
							vertices[nbVertices].x = xTile;
							vertices[nbVertices].y = yTile;
							vertices[nbVertices].z = 0;
							vertices[nbVertices+1].u = vertices[nbVertices].u + m->tileX;
							vertices[nbVertices+1].v = vertices[nbVertices].v + m->tileY;
							vertices[nbVertices+1].x = vertices[nbVertices].x + m->tileX;
							vertices[nbVertices+1].y = vertices[nbVertices].y + m->tileY;
							vertices[nbVertices+1].z = 0;
							nbVertices += 2;
						}
						xTile += m->tileX;
						mX++;
						if (mX >= m->mapSizeX)
							mX -= m->mapSizeX;
					}
				}
				else	{
					for (x=0;x<dsX;x++)			{
						v = map[bY+mX];
						//Plus compréhensible
						if (v || firstTileOpaque)			{
							//Unoptimized method
							vertices[nbVertices].u = (v % tilesPerLine) * m->tileX;
							vertices[nbVertices].v = (v / tilesPerLine) * m->tileY;
							vertices[nbVertices].x = xTile;
							vertices[nbVertices].y = yTile;
							vertices[nbVertices].z = 0;
							vertices[nbVertices+1].u = vertices[nbVertices].u + m->tileX;
							vertices[nbVertices+1].v = vertices[nbVertices].v + m->tileY;
							vertices[nbVertices+1].x = vertices[nbVertices].x + m->tileX;
							vertices[nbVertices+1].y = vertices[nbVertices].y + m->tileY;
							vertices[nbVertices+1].z = 0;
							nbVertices += 2;
						}
						xTile += m->tileX;
						mX++;
						if (mX >= m->mapSizeX)
							mX -= m->mapSizeX;
					}
				}
				//Dessine
				if (nbVertices > 0)
					sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, nbVertices, 0, vertices);
				mY++;
				if (mY >= m->mapSizeY)
					mY -= m->mapSizeY;
				yTile += m->tileY;
			}
			break;

		case OSL_MF_U16_GBA:
			for (y=0;y<dsY;y++)			{
				bY = m->mapSizeX * mY;
				mX = dX;
				xTile = -sX;
				vertices = (OSL_FAST_VERTEX*)sceGuGetMemory(dsX * 2 * sizeof(OSL_FAST_VERTEX));
				nbVertices = 0;
				for (x=0;x<dsX;x++)			{
					int flags;
					v = map[bY+mX];
					flags = v & ~((1 << m->addit1) - 1);
					v &= ((1 << m->addit1) - 1);
					//Plus compréhensible
					if (v || firstTileOpaque)			{
						//Unoptimized method
						vertices[nbVertices].u = (v % tilesPerLine) * m->tileX;
						vertices[nbVertices].v = (v / tilesPerLine) * m->tileY;
						vertices[nbVertices].x = xTile;
						vertices[nbVertices].y = yTile;
						vertices[nbVertices].z = 0;
						vertices[nbVertices+1].u = vertices[nbVertices].u + m->tileX;
						vertices[nbVertices+1].v = vertices[nbVertices].v + m->tileY;
						vertices[nbVertices+1].x = vertices[nbVertices].x + m->tileX;
						vertices[nbVertices+1].y = vertices[nbVertices].y + m->tileY;
						vertices[nbVertices+1].z = 0;

						//Mirroir horizontal?
						if (flags & (1 << m->addit1))			 {
							int exchg = vertices[nbVertices].u;
							vertices[nbVertices].u = vertices[nbVertices+1].u;
							vertices[nbVertices+1].u = exchg;
						}
						if (flags & (1 << (m->addit1 + 1)))			 {
							int exchg = vertices[nbVertices].v;
							vertices[nbVertices].v = vertices[nbVertices+1].v;
							vertices[nbVertices+1].v = exchg;
						}

						nbVertices += 2;
					}
					xTile += m->tileX;
					mX++;
					if (mX >= m->mapSizeX)
						mX -= m->mapSizeX;
				}
				//Dessine
				if (nbVertices > 0)
					sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, nbVertices, 0, vertices);
				mY++;
				if (mY >= m->mapSizeY)
					mY -= m->mapSizeY;
				yTile += m->tileY;
			}
			break;
	}
}

void oslDeleteMap(OSL_MAP *m)		{
	free(m);
}
