// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SFML/Graphics.h>
#include "types.h"

extern sfTexture* RoomTexture;
extern sfSprite* RoomSprite;
extern void Graphics_UpdateRoom(gameroom_t Room);
extern void Graphics_DrawRoom(gameroom_t Room);

#endif