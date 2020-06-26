// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

//
// graphics_wrapper.c - Redirect basic bindings to SFML equivalents for Win, Mac, Linux
//

#include "graphics_wrapper.h"

int Graphics_GetTextureWidth(texture2d_t* Texture)
{
    return sfTexture_getSize(Texture).x;
}

int Graphics_GetTextureHeight(texture2d_t* Texture)
{
    return sfTexture_getSize(Texture).y;
}