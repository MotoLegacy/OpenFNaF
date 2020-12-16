#include "oslib.h"

bool oslMoveImageTo(OSL_IMAGE *img, int newLocation)		{
	bool success = 0;
	//On ne gère pas ça sur PC
#ifndef PSP
	return 1;
#endif

	//Moves the image
	if (img->location != newLocation)		{
		//We keep the old pointer in order to recopy the old data to the new image
		OSL_IMAGE oldImage = *img;

		//Allocate some new data for the image. Setting data to NULL prevents oslAllocImageData from freeing memory automatically (we need this memory later to copy the old image contents).
		img->data = NULL;

		oslAllocImageData(img, newLocation);
		if (img->data)		{
			//Copy the old image to the new one
			memcpy(img->data, oldImage.data, img->totalSize);

			//We can now free the old image data
			oslFreeImageData(&oldImage);
			
			oslUncacheImageData(img);
			success = 1;
		}
	}
	
	return success;
}

