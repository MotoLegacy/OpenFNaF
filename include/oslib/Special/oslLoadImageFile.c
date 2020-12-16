#include "../oslib.h"

//Generic, includes support for all image types!
OSL_IMAGE *oslLoadImageFile(char *filename, int location, int pixelFormat)
{
	char *ext, extension[10];
	int i;
	ext = strrchr(filename, '.');
	if (!ext)
		return NULL;
	i = 0;
	while(ext[i] && i < sizeof(extension) - 2)
	{
		extension[i] = tolower((unsigned char) ext[i]);
		i++;
	}
	extension[i] = 0;
	if (!strcmp(extension, ".png"))
		return oslLoadImageFilePNG(filename, location, pixelFormat);
	else if (!strcmp(extension, ".jpg"))
		return oslLoadImageFileJPG(filename, location, pixelFormat);
	else if (!strcmp(extension, ".gif"))
		return oslLoadImageFileGIF(filename, location, pixelFormat);

	return NULL;
}

