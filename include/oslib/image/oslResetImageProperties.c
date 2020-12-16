#include "oslib.h"

void oslResetImageProperties(OSL_IMAGE *img)
{
	//Positions
	img->x = img->y = 0;

	//Offset
	img->offsetX0 = img->offsetY0 = 0;
	img->offsetX1 = img->sizeX;
	img->offsetY1 = img->sizeY;

	//Rotation	
	img->centerX = img->centerY = img->angle = 0;

	//Scaling
	img->stretchX = img->sizeX;
	img->stretchY = img->sizeY;
}

