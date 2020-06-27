// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

//
// graphics_wrapper.c - Redirect basic bindings to SFML equivalents for Win, Mac, Linux
//

#include "graphics_wrapper.h"

texture2d_t* Graphics_CreateTextureFromFile(char* Directory)
{
    return sfTexture_createFromFile(Directory, NULL);
}

int Graphics_GetTextureWidth(texture2d_t* Texture)
{
    return sfTexture_getSize(Texture).x;
}

int Graphics_GetTextureHeight(texture2d_t* Texture)
{
    return sfTexture_getSize(Texture).y;
}

sprite_t* Graphics_CreateSprite()
{
    return sfSprite_create();
}

void Graphics_DrawSprite(sprite_t* Sprite, window_t* Window)
{
    return sfRenderWindow_drawSprite(Window, Sprite, NULL);
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

font_t* Graphics_CreateFontFromFile(char* Directory)
{
    return sfFont_createFromFile(Directory);
}

text_t* Graphics_CreateText()
{
    return sfText_create();
}

void Graphics_DrawText(text_t* Text, window_t* Window)
{
    sfRenderWindow_drawText(Window, Text, NULL);
}

void Graphics_SetTextString(text_t* Elem, char* Text)
{
    sfText_setString(Elem, Text);
}

void Graphics_SetTextFont(text_t* Elem, font_t* Font)
{
    sfText_setFont(Elem, Font);
}

void Graphics_SetTextSize(text_t* Elem, int Size)
{
    sfText_setCharacterSize(Elem, Size);
}

void Graphics_SetTextColor(text_t* Elem, color_t Color)
{
    sfText_setColor(Elem, Color);
}

void Graphics_SetTextPosition(text_t* Elem, double x, double y)
{
    sfText_setPosition(Elem, (sfVector2f) {x, y});
}

int Graphics_GetTextWidth(text_t* Elem)
{
    return sfText_getGlobalBounds(Elem).width;
}

int Graphics_GetTextHeight(text_t* Elem)
{
    return sfText_getGlobalBounds(Elem).height;
}