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

int Graphics_GetSpriteXPosition(sprite_t* Sprite)
{
    return sfSprite_getPosition(Sprite).x;
}

int Graphics_GetSpriteYPosition(sprite_t* Sprite)
{
    return sfSprite_getPosition(Sprite).y;
}

void Graphics_SetSpritePosition(sprite_t* Sprite, float x, float y)
{
    sfSprite_setPosition(Sprite, (sfVector2f) {x, y});
}

void Graphics_BindTextureToSprite(texture2d_t* Texture, sprite_t* Sprite)
{
    sfSprite_setTexture(Sprite, Texture, sfTrue);
}