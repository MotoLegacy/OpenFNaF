#include "oslib.h"
#include "gif/gif_lib.h"

/*
	Note: la palette temporaire ici est 32 bits mais ce serait facile de l'adapter à autre chose, cf Palette[i] = RGBA(...).
*/
u32 *osl_gifTempPalette; // Utilisé pour stocker la palette des gifs...
const short InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
const short InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
GifPixelType osl_gifLineBuf[2048]; // Buffer temporaire

int fnGifReadFunc(GifFileType* GifFile, GifByteType* buf, int count)
{
/*    char* ptr = (char*)GifFile->UserData;
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;*/
    VirtualFileRead(buf, 1, count, (VIRTUAL_FILE*)GifFile->UserData);

    return count;
}

void fnCopyLine(void* dst, void* src, int count, int pixelFormat, int transparentColor)			{
	int x;
	u8 *p_dest1 = (u8*)dst;
	u16 *p_dest2 = (u16*)dst;
	u32 *p_dest4 = (u32*)dst;
	u8 *p_src = (u8*)src;
	u32 pixel_value;

	for (x=0;x<count;x++)			{
		//Next pixel palette entry
		pixel_value = p_src[x];
		//True color mode => convert the temporary color to destination format
		if (osl_pixelWidth[pixelFormat] > 8)
			pixel_value = oslConvertColor(pixelFormat, OSL_PF_8888, osl_gifTempPalette[pixel_value]);

		if (osl_pixelWidth[pixelFormat] == 32)			{
			p_dest4[x] = pixel_value;
		}
		else if (osl_pixelWidth[pixelFormat] == 16)
			p_dest2[x] = pixel_value;
		else if (osl_pixelWidth[pixelFormat] == 8)
			p_dest1[x] = pixel_value;
		else if (osl_pixelWidth[pixelFormat] == 4)			{
			p_dest1[x >> 1] &= ~(15 << ((x & 1) << 2));
			p_dest1[x >> 1] |= (pixel_value & 15) << ((x & 1) << 2);
		}
	}

}


int DGifGetLineByte(GifFileType *GifFile, GifPixelType *Line, int LineLen, int pixelFormat, int transparentColor)				{
	int result = DGifGetLine(GifFile, osl_gifLineBuf, LineLen);							// Nouvelle ligne
	fnCopyLine(Line, osl_gifLineBuf, LineLen, pixelFormat, transparentColor);		// Ecrit dans le buffer
	return result;
}

OSL_IMAGE *oslLoadImageFileGIF(char *filename, int location, int pixelFormat)
{
	OSL_IMAGE *img = NULL;
 	int i, j, alpha, Row=0, Col=0, Width, Height, ExtCode;
	u32 *Palette = NULL;
	GifRecordType RecordType;
	GifByteType *Extension;
	GifFileType *GifFile;
	ColorMapObject *ColorMap;
	int transparentColor = -1;
	//We only keep the location bits
	int imgLocation = location & OSL_LOCATION_MASK;
	VIRTUAL_FILE *f;

	if (osl_pixelWidth[pixelFormat] > 8)			{ 									// En mode true color, on utilise une palette temporaire...
		osl_gifTempPalette = (u32*)malloc(256 * sizeof(u32));								// Ecran temporaire
		if (!osl_gifTempPalette)
			return NULL;
		Palette = osl_gifTempPalette;
	}
	else
		osl_gifTempPalette = NULL;

	f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_READ);
	if (f)			{
		GifFile = DGifOpen(f, fnGifReadFunc);

		// Scan the content of the GIF file and load the image(s) in:
		do {
			DGifGetRecordType(GifFile, &RecordType);

			switch (RecordType) {
				case IMAGE_DESC_RECORD_TYPE:
					DGifGetImageDesc(GifFile);
					/*if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
					PrintGifError();
					return EXIT_FAILURE;
					}*/
	//					Row = GifFile->Image.Top; /* Image Position relative to Screen. */
	//					Col = GifFile->Image.Left;
					//Je n'en tiens pas compte car il faudrait aggrandir l'image, c'est peut être utile pour les gifs animés remarque
					Row = Col = 0;
					Width = GifFile->Image.Width;
					Height = GifFile->Image.Height;

					// Update Color map
					ColorMap = (GifFile->Image.ColorMap	? GifFile->Image.ColorMap: GifFile->SColorMap);

					// Crée l'image dans laquelle on mettra nos données
					img = oslCreateImage(Width, Height, imgLocation, pixelFormat);
					if (osl_pixelWidth[pixelFormat] <= 8)		{
						ColorMap->ColorCount = oslMin(ColorMap->ColorCount, 1 << osl_paletteSizes[pixelFormat]);
						img->palette = oslCreatePalette(ColorMap->ColorCount, OSL_PF_8888);
						Palette = (u32*)img->palette->data;
					}

					//Tenons compte de la couleur transparente
					i = ColorMap->ColorCount;
					while (--i >= 0)  {
						GifColorType* pColor = &ColorMap->Colors[i];
						if (i == transparentColor || (osl_colorKeyEnabled && RGBA(pColor->Red, pColor->Green, pColor->Blue, 0) == (osl_colorKeyValue & 0x00ffffff)))
							alpha = 0;
						else
							alpha = 0xff;
						Palette[i] = RGBA(pColor->Red, pColor->Green, pColor->Blue, alpha);
					}

					/* if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
					GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
					return EXIT_FAILURE;
				   }*/

					if (GifFile->Image.Interlace) {
						// Need to perform 4 passes on the images:
						for (i = 0; i < 4; i++) {
							for (j = Row + InterlacedOffset[i]; j < Row + Height;
								j += InterlacedJumps[i]) {
								DGifGetLineByte(GifFile, (GifPixelType*)oslGetImagePixelAdr(img, Col, j), Width, pixelFormat, transparentColor);
								/*if (DGifGetLineByte(GifFile, &ScreenBuff[j][Col],
									Width) == GIF_ERROR) {
									PrintGifError();
									return EXIT_FAILURE;
								}*/
							}
						}
					}
					else {
						for (i = 0; i < Height; i++) {
							DGifGetLineByte(GifFile, (GifPixelType*)oslGetImagePixelAdr(img, Col, Row), Width, pixelFormat, transparentColor);
							Row++;
							/*
							if (DGifGetLineByte(GifFile, &ScreenBuff[Row++][Col],
							Width) == GIF_ERROR) {
								PrintGifError();
								return EXIT_FAILURE;
							}*/
						}
					}
					break;

				case EXTENSION_RECORD_TYPE:
					// Skip any extension blocks in file:
					DGifGetExtension(GifFile, &ExtCode, &Extension);

					while (Extension != NULL) {
						//Couleur transparente
						if (ExtCode == 249)			{
						   if (Extension[1] & 1)			{
							  transparentColor = Extension[4];
						   }
						}
						DGifGetExtensionNext(GifFile, &Extension);
				   }
					break;

				case TERMINATE_RECORD_TYPE:
					break;
				default:		    /* Should be traps by DGifGetRecordType. */
					break;
			}
		} while (RecordType != TERMINATE_RECORD_TYPE);

		 /* Close file when done */
		DGifCloseFile(GifFile);
		VirtualFileClose(f);

		if (osl_pixelWidth[pixelFormat] > 8)
			free(osl_gifTempPalette); 					//Libère la mémoire allouée pour la palette fixe

		if (oslImageLocationIsSwizzled(location))
			oslSwizzleImage(img);

		//Uncache the image so that it's ready to use!
		oslUncacheImage(img);
	}

	if (!img)
		oslHandleLoadNoFailError(filename);
	return img;
}

