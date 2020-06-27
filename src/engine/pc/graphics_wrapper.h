#ifndef _GRAPHICS_WRAPPER_H_
#define _GRAPHICS_WRAPPER_H_

#include <SFML/Graphics.h>

typedef sfTexture  texture2d_t;
typedef sfSprite   sprite_t;

extern void Graphics_SetSpritePosition(sprite_t* Sprite, float x, float y);

#endif