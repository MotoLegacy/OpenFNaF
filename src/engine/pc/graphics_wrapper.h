#ifndef _GRAPHICS_WRAPPER_H_
#define _GRAPHICS_WRAPPER_H_

#include <SFML/Graphics.h>

typedef sfTexture  texture2d_t;
typedef sfSprite   sprite_t;

extern int Graphics_GetTextureWidth(texture2d_t* Texture);
extern int Graphics_GetTextureHeight(texture2d_t* Texture);
extern int Graphics_GetSpriteXPosition(sprite_t* Sprite);
extern int Graphics_GetSpriteYPosition(sprite_t* Sprite);
extern void Graphics_SetSpritePosition(sprite_t* Sprite, float x, float y);
extern void Graphics_BindTextureToSprite(texture2d_t* Texture, sprite_t* Sprite);

#endif