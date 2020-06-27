// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#ifdef DESKTOP
#include "pc/graphics_wrapper.h"
// FIXME move this
#include "pc/general_wrapper.h"
#endif

#include "types.h"

// Total max number of UI elements in game
#define MAX_UI_ELEMENTS     64

// UI Anchor Points
#define UI_ANCHOR_LEFT      0
#define UI_ANCHOR_CENTER    1
#define UI_ANCHOR_RIGHT     2
#define UI_ANCHOR_TOP       3
#define UI_ANCHOR_BOTTOM    4

// Text struct
typedef struct {
    color_t Color;
    bool Initialized;
    text_t* TextElem;
    char* Text;
    int XAnchor;
    int YAnchor;
    int XPosPercent;
    int YPosPercent;
    int Size;
    int id;
} textelement_t;

// UI struct
typedef struct {
    bool Activated;
    bool Initialized;
    bool Need_Clicked;
    texture2d_t* Texture;
    sprite_t* Sprite;
    int XAnchor;
    int YAnchor;
    int XPosPercent;
    int YPosPercent;
    char* Graphic;
    void (*func)();
} uidata_t;

extern uidata_t UIElements[MAX_UI_ELEMENTS];
extern int Current_Element;

extern float RoomPanX;
extern float CameraPanX;

extern texture2d_t* RoomTexture;
extern sprite_t* RoomSprite;

extern void Graphics_InitializeFont();
extern void Graphics_RegisterTextElement(textelement_t* Element);
extern void Graphics_UpdateTextElement(textelement_t* Element);
extern void Graphics_UpdateRoom(gameroom_t Room);
extern void Graphics_DrawRoom(gameroom_t Room);
extern void Graphics_DrawUI();
extern void Graphics_DrawTextElements();
extern void Graphics_RegisterUIElement(char* Graphic, int XPosPercent, int YPosPercent, int XAnchor, int YAnchor, bool Need_Clicked, void (*func));

#endif