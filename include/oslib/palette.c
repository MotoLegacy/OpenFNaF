#include "oslib.h"

/*
	PALETTE
*/
OSL_PALETTE *oslCreatePaletteEx(int size, int location, short pixelFormat)
{
	OSL_PALETTE *p;
	p = (OSL_PALETTE*)malloc(sizeof(OSL_PALETTE));
	if (!p)
		return NULL;
	p->pixelFormat = pixelFormat;
	p->nElements = size;
	p->location = location;
	if (location == OSL_IN_RAM)
		p->data = memalign(16, (size * osl_pixelWidth[pixelFormat]) >> 3);
	else if (location == OSL_IN_VRAM)
		p->data = oslVramMgrAllocBlock((size * osl_pixelWidth[pixelFormat]) >> 3);
	else
		p->data = NULL;
	//Allocation ratée
	if (!p->data)			{
		free(p);
		return NULL;
	}
	memset(p->data, 0, (size * osl_pixelWidth[pixelFormat]) >> 3);
	return p;
}

OSL_PALETTE *oslCreatePaletteFrom(void *data, int size, short pixelFormat)
{
	OSL_PALETTE *p;
	p = (OSL_PALETTE*)malloc(sizeof(OSL_PALETTE));
	if (!p)
		return NULL;
	//We'll not free it later
	p->location = OSL_IN_NONE;
	p->pixelFormat = pixelFormat;
	p->nElements = size;
	p->data = data;
	return p;
}

void oslDeletePalette(OSL_PALETTE *p)		{
	if (!p)
		return;
	if (p->location == OSL_IN_RAM)
		free(p->data);
	else if (p->location == OSL_IN_VRAM)
		oslVramMgrFreeBlock(p->data, p->nElements * osl_pixelWidth[p->pixelFormat]);
	free(p);
}

void oslUncachePalette(OSL_PALETTE *pal)
{
	sceKernelDcacheWritebackInvalidateRange(pal->data, (pal->nElements * osl_pixelWidth[pal->pixelFormat]) >> 3);
	//A réuploader la prochaine fois :)
	if (pal == osl_curPalette)
		osl_curPalette = NULL;
}

int oslGetPaletteColor(OSL_PALETTE *p, int index)		{
	u32 value = 0;
	if (osl_pixelWidth[p->pixelFormat] == 32)
		value = ((u32*)p->data)[index];
	else if (osl_pixelWidth[p->pixelFormat] == 16)
		value = ((u16*)p->data)[index];
	return value;
}

