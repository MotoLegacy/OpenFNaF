#include "../oslib.h"

//Generic, includes support for all image types!
int oslWriteImageFile(OSL_IMAGE *img, const char* filename, int flags)
{
	const char *ext;
	char extension[10];
	int i;

	//Impossible to write swizzled images
	if (oslImageIsSwizzled(img))
		return 0;
	ext = strrchr(filename, '.');
	if (!ext)
		return 0;
	i = 0;
	while(ext[i] && i < sizeof(extension) - 2)
	{
		extension[i] = tolower((unsigned char) ext[i]);
		i++;
	}
	extension[i] = 0;
	if (!strcmp(extension, ".png"))
		return oslWriteImageFilePNG(img, filename, flags);
	return 0;
}

