#include "oslib.h"

/* source from pnmscale.c in netpbm
**
** modified by Kevin (sosaria at empal.com)
**
** blow is original copyright of netpbm
** Copyright (C) 1989, 1991 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
**
*/


#define RGBA_GETR(pixel)	((pixel)&0xff)
#define RGBA_GETG(pixel)	(((pixel)>>8)&0xff)
#define RGBA_GETB(pixel)	(((pixel)>>16)&0xff)
#define RGBA_GETA(pixel)	(((pixel)>>24)&0xff)
#ifndef MIN
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#endif

static void readRow(OSL_IMAGE *img, int row, unsigned int *xelrow)
{
	int x;
	int pixel;
	for( x = 0; x < img->sizeX; x++ ) {
		pixel = oslGetImagePixel(img, x, row);
		xelrow[x] = (unsigned int)oslConvertColor(OSL_PF_8888, img->pixelFormat, pixel );
	}
}

static void writeRow(OSL_IMAGE *img, unsigned int *xelrow, int startx, int row, int width)
{
	int x;
	int pixel;
	for( x = 0; x < width; x++ ) {
		pixel = oslConvertColor(img->pixelFormat, OSL_PF_8888, xelrow[x]);
		oslSetImagePixel(img, x+startx, row, pixel);
	}
}

static void zeroAccum(int col, float rs[], float gs[], float bs[], float as[])
{
	int x;
	for( x = 0; x < col; x++ ) {
		rs[x] = gs[x] = bs[x] = as[x] = 0.0;
	}
}
static void accumOutputRow(unsigned int* const xelrow, float const fraction,
					float rs[], float gs[], float bs[], float as[], int const cols)
{
/*----------------------------------------------------------------------------
   Take 'fraction' times the color in row xelrow and add it to
   rs/gs/bs.  'fraction' is less than 1.0.
-----------------------------------------------------------------------------*/
	int x;

	for( x = 0; x < cols; x++ ) {
		rs[x] += fraction * RGBA_GETR(xelrow[x]);
		gs[x] += fraction * RGBA_GETG(xelrow[x]);
		bs[x] += fraction * RGBA_GETB(xelrow[x]);
		as[x] += fraction * RGBA_GETA(xelrow[x]);
	}
}

static void horizontalScale(const float rs[], const float gs[], const float bs[], const float as[],
							unsigned int newxelrow[], const int cols, const int newcols, const float xscale)
{
/*----------------------------------------------------------------------------
   Take the input row rs[]/gs[]/bs[]/as[], which is 'cols' columns wide, and
   scale it by a factor of 'xscale', to create
   the output row newxelrow[], which is 'newcols' columns wide.

-----------------------------------------------------------------------------*/
    float r, g, b, a;
    float fraccoltofill, fraccolleft;
    unsigned int col;
    unsigned int newcol;

    newcol = 0;
    fraccoltofill = 1.0;  /* Output column is "empty" now */
    r = g = b = a = 0;          /* initial value */
    for (col = 0; col < cols; ++col) {
        /* Process one pixel from input ('inputxelrow') */
        fraccolleft = xscale;
        /* Output all columns, if any, that can be filled using information
           from this input column, in addition to what's already in the output
           column.
        */
        while (fraccolleft >= fraccoltofill) {
            /* Generate one output pixel in 'newxelrow'.  It will consist
               of anything accumulated from prior input pixels in 'r','g',
               and 'b', plus a fraction of the current input pixel.
            */
			r += fraccoltofill * rs[col];
			g += fraccoltofill * gs[col];
			b += fraccoltofill * bs[col];
			a += fraccoltofill * as[col];
			newxelrow[newcol] = RGBA( MIN(0xff, (int)(r + 0.5) ),
               						  MIN(0xff, (int)(g + 0.5) ),
               						  MIN(0xff, (int)(b + 0.5) ),
               						  MIN(0xff, (int)(a + 0.5) ) );
            fraccolleft -= fraccoltofill;
            /* Set up to start filling next output column */
            newcol++;
            fraccoltofill = 1.0;
            r = g = b = 0.0;
        }
        /* There's not enough left in the current input pixel to fill up
           a whole output column, so just accumulate the remainder of the
           pixel into the current output column.
        */
        if (fraccolleft > 0.0) {
			r += fraccolleft * rs[col];
			g += fraccolleft * gs[col];
			b += fraccolleft * bs[col];
			a += fraccolleft * as[col];
            fraccoltofill -= fraccolleft;
        }
    }

//    if (newcol < newcols-1 || newcol > newcols)
//        pm_error("Internal error: last column filled is %d, but %d "
//                 "is the rightmost output column.",
//                 newcol, newcols-1);

    if (newcol < newcols ) {
        /* We were still working on the last output column when we
           ran out of input columns.  This would be because of rounding
           down, and we should be missing only a tiny fraction of that
           last output column.
        */
		r += fraccoltofill * rs[cols-1];
		g += fraccoltofill * gs[cols-1];
		b += fraccoltofill * bs[cols-1];
		a += fraccoltofill * as[cols-1];
		newxelrow[newcol] = RGBA( MIN(0xff, (int)(r + 0.5) ),
								  MIN(0xff, (int)(g + 0.5) ),
								  MIN(0xff, (int)(b + 0.5) ),
								  MIN(0xff, (int)(a + 0.5) ) );
	}
}

void oslScaleImage(OSL_IMAGE *dstImg, OSL_IMAGE *srcImg, int newX, int newY, int newWidth, int newHeight)
{
	float rowsleft;
	/* The number of rows of output that need to be formed from the
       current input row (the one in tuplerow[]), less the number that
       have already been formed (either in accumulator[]
       or output to the file).  This can be fractional because of the
       way we define rows as having height.
    */
	float fracrowtofill;
        /* The fraction of the current output row (the one in vertScaledRow[])
           that hasn't yet been filled in from an input row.
        */
	int rowsread;
		/* Number of rows of the input file that have been read */
	int row;

	unsigned int* orgxelrow;
	unsigned int* newxelrow;
	float* rs;
	float* gs;
	float* bs;
	float* as;

	float xscale = (float)newWidth / srcImg->sizeX;
	float yscale = (float)newHeight / srcImg->sizeY;

	rowsread = 0;
	rowsleft = 0.0;
	fracrowtofill = 1.0;

	orgxelrow = malloc( srcImg->sizeX * sizeof(orgxelrow[0]) );
	rs = malloc( srcImg->sizeX * sizeof(rs[0]) );
	gs = malloc( srcImg->sizeX * sizeof(gs[0]) );
	bs = malloc( srcImg->sizeX * sizeof(bs[0]) );
	as = malloc( srcImg->sizeX * sizeof(bs[0]) );
	newxelrow = malloc( newWidth * sizeof(newxelrow[0]) );

	for( row = 0; row < newHeight; ++row ) {
		/* First scane Y from orgxelrow[] into vertScanedRow[]. */

		if ( newHeight == srcImg->sizeY ) { /* shortcut Y scaling if possible */
			readRow( srcImg, row, orgxelrow );
		}
		else {
			zeroAccum(srcImg->sizeX, rs, gs, bs, as);
			while (fracrowtofill > 0) {
				if (rowsleft <= 0.0) {
					if (rowsread < srcImg->sizeY) {
						readRow( srcImg, rowsread, orgxelrow );
						++rowsread;
					}
					else {
	                    /* We need another input row to fill up this
	                       output row, but there aren't any more.
	                       That's because of rounding down on our
	                       scaling arithmetic.  So we go ahead with
	                       the data from the last row we read, which
	                       amounts to stretching out the last output
	                       row.
	                    */
					}
					rowsleft = yscale;
				}
				if (rowsleft < fracrowtofill) {
					accumOutputRow(orgxelrow, rowsleft, rs, gs, bs, as, srcImg->sizeX);
					fracrowtofill -= rowsleft;
					rowsleft = 0.0;
				}
				else {
					accumOutputRow(orgxelrow, fracrowtofill, rs, gs, bs, as, srcImg->sizeX);
					rowsleft = rowsleft - fracrowtofill;
					fracrowtofill = 0.0;
				}
			}
			fracrowtofill = 1.0;
		}

        /* Now scale rs/gs/bs horizontally into newxelrow and write
           it dstImg.
        */

		horizontalScale(rs, gs, bs, as, newxelrow, srcImg->sizeX, newWidth, xscale);
		writeRow(dstImg, newxelrow, newX, newY + row, newWidth);

	}
	free( newxelrow );
	free( as );
	free( bs );
	free( gs );
	free( rs );
	free( orgxelrow );

	return;
}

OSL_IMAGE *oslScaleImageCreate(OSL_IMAGE *img, short newLocation, int newWidth, int newHeight, short newPixelFormat)
{
	OSL_IMAGE* newImg;
	if(!img)
		return NULL;
	newImg = oslCreateImage(newWidth, newHeight, newLocation, newPixelFormat);
	if (newImg) {
		oslScaleImage(newImg, img, 0, 0, newWidth, newHeight);
		if (newImg != NULL && oslImageLocationIsSwizzled(newLocation))
			oslSwizzleImage(newImg);
        oslUncacheImage(newImg);
	}
	return newImg;
}
