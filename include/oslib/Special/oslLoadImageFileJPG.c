#include "../oslib.h"
#include <jpeglib.h>
#include <stdio.h>

OSL_IMAGE *oslLoadImageFileJPG(char *filename, int location, int pixelFormat)
{
    OSL_IMAGE *img = NULL;
    const unsigned char *input, *input_free;
    int input_size = 0;
    //We only keep the location bits
    int imgLocation = location & OSL_LOCATION_MASK;
    VIRTUAL_FILE *f;

    //True color is mandatory for JPG!
    if (osl_pixelWidth[pixelFormat] > 8)
    {
        //Using libjpeg:
        int width = 1600;
        int height = 1200;
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        JSAMPROW row_pointer[1];

        f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_READ);

        if (!f)    return NULL;
        input = (const unsigned char*)oslReadEntireFileToMemory(f, &input_size);
        input_free = input;
        VirtualFileClose(f);

        FILE *infile = fmemopen((void *)input,input_size,"rb");
//        FILE *infile = fopen( filename, "rb" );

        unsigned long location = 0;
        int i = 0;
        int x;

        if ( !infile )
            return img;

        cinfo.err = jpeg_std_error( &jerr );
        jpeg_create_decompress( &cinfo );
        jpeg_stdio_src( &cinfo, infile );
        jpeg_read_header( &cinfo, TRUE );
        width = cinfo.image_width;
        height = cinfo.image_height;

        img = oslCreateImage(width, height, imgLocation, pixelFormat);

        if (img) {
            jpeg_start_decompress( &cinfo );
            row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
            while( cinfo.output_scanline < cinfo.image_height )
            {
                jpeg_read_scanlines( &cinfo, row_pointer, 1 );
                i = 0;
                for( x=0; x<cinfo.image_width;x++ ) {
                    oslSetImagePixel(img, x, cinfo.output_scanline, oslConvertColor(pixelFormat, OSL_PF_8888, row_pointer[0][i] | row_pointer[0][i+1] << 8 | row_pointer[0][i+2] << 16 | 0xFF << 24) );
                    i += cinfo.num_components;
                }
            }
            jpeg_finish_decompress( &cinfo );
        }
        jpeg_destroy_decompress( &cinfo );
        free( row_pointer[0] );
        fclose( infile );
        free((void*)input_free);

        if (img != NULL && oslImageLocationIsSwizzled(location))
            oslSwizzleImage(img);
        oslUncacheImage(img);
/*
        f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_READ);

        if (f)            {
            input = (const unsigned char*)oslReadEntireFileToMemory(f, NULL);
            input_free = input;
            VirtualFileClose(f);

            if (input)            {
                int width, height;
                JPEG_Decoder decoder;

                JPEG_Decoder_ReadHeaders(&decoder, &input);
                width = decoder.frame.width;
                height = decoder.frame.height;

                img = oslCreateImage(width, height, imgLocation, pixelFormat);

                if (img)            {
                    if (!JPEG_Decoder_ReadImage (&decoder, &input, img, img->realSizeX, img->realSizeY))            {
                        oslDeleteImage(img);
                        img = NULL;
                    }
                }

                free((void*)input_free);

                if (img != NULL && oslImageLocationIsSwizzled(location))
                    oslSwizzleImage(img);
                oslUncacheImage(img);
            }
        } */
    }

    if (!img)
        oslHandleLoadNoFailError(filename);
    return img;
}
