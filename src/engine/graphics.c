// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "graphics.h"
#include "window.h"
#include "game.h"

#include <stdio.h>

textelement_t* TextElements[MAX_UI_ELEMENTS];
uidata_t UIElements[MAX_UI_ELEMENTS];

// FIXME - cleanup
int Current_Element = 0;
int Current_TextElement = 0;

texture2d_t* RoomTexture;
sprite_t* RoomSprite;

sfFont* GameFont;

float RoomPanX = 0;
float CameraPanX = 0;

// Free and then re-init
void Graphics_UpdateRoom(gameroom_t Room) {
    free(RoomTexture);
    free(RoomSprite);

    Graphics_InitRoom(Room);
}

// Initialization of Room drawings
void Graphics_InitRoom(gameroom_t Room) {

    RoomTexture = sfTexture_createFromFile(Room.Graphic, NULL);

    // If Texture doesn't exist (TODO)
    /*if (!RoomTexture)
        Sys_Error("Failed to initialize texture %s\n", Room.Graphic);*/

    // Init Sprite, bind Texture to it
    RoomSprite = sfSprite_create();

    Graphics_BindTextureToSprite(RoomTexture, RoomSprite);
}

// Do the actual Room Drawing
void Graphics_DrawRoom(gameroom_t Room) {
    // Initialize the Room if not already
    if (!RoomTexture || !RoomSprite)
        Graphics_InitRoom(Room);

    if (Scroll_Method == SCROLL_MANUAL)
        Graphics_SetSpritePosition(RoomSprite, RoomPanX, 0);
    else if (Scroll_Method == SCROLL_AUTOMATIC)
        Graphics_SetSpritePosition(RoomSprite, CameraPanX, 0);
    else
        Graphics_SetSpritePosition(RoomSprite, 0, 0);

    sfRenderWindow_drawSprite(GameWindow, RoomSprite, NULL);
}

// Initialize UI Elements if not already done
void Graphics_InitializeUIElement(uidata_t* UIElement) {
    // If we don't have a graphic, don't bother
    if (!UIElement->Graphic)
        return;

    // Set it's sprites and textures
    UIElement->Texture = sfTexture_createFromFile(UIElement->Graphic, NULL);
    UIElement->Sprite = sfSprite_create();

    // Bind Sprite and Texture
    Graphics_BindTextureToSprite(UIElement->Texture, UIElement->Sprite);

    // Determine Our Offset values
    int OffsetX;
    int OffsetY;

    switch(UIElement->XAnchor) {
        case UI_ANCHOR_LEFT:
            OffsetX = 0;
            break;
        case UI_ANCHOR_CENTER:
            OffsetX = (Graphics_GetTextureWidth(UIElement->Texture))/2;
            break;
        case UI_ANCHOR_RIGHT:
            OffsetX = Graphics_GetTextureWidth(UIElement->Texture);
            break;
        default:
            OffsetX = 0;
            break;
    }

    switch(UIElement->YAnchor) {
        case UI_ANCHOR_TOP:
            OffsetY = 0;
            break;
        case UI_ANCHOR_CENTER:
            OffsetY = (Graphics_GetTextureHeight(UIElement->Texture))/2;
            break;
        case UI_ANCHOR_BOTTOM:
            OffsetY = Graphics_GetTextureHeight(UIElement->Texture);
            break;
        default:
            OffsetY = 0;
            break;
    }

    // Get our X & Y based on Anchors and Percentage

    int XPosition = I_GAME_WIDTH;
    int YPosition = I_GAME_HEIGHT;

    XPosition /= (100/UIElement->XPosPercent);
    YPosition /= (100/UIElement->YPosPercent);

    XPosition -= OffsetX;
    YPosition -= OffsetY;

    // Finally, set our Position
    Graphics_SetSpritePosition(UIElement->Sprite, XPosition, YPosition);
    

    // We're Initialized! Ready to Draw.
    UIElement->Initialized = TRUE;
}

// Scan our UI array/structs and draw accordingly
void Graphics_DrawUI() {
    for (int i = 0; i < Current_Element; ++i) {
        if (!UIElements[i].Initialized) 
            Graphics_InitializeUIElement(&UIElements[i]);

        sfRenderWindow_drawSprite(GameWindow, UIElements[i].Sprite, NULL);
    }
}

// Registration of UI Elements to the array
void Graphics_RegisterUIElement(char* Graphic, int XPosPercent, int YPosPercent, int XAnchor, int YAnchor, bool Need_Clicked, void (*func)) {
    UIElements[Current_Element].Graphic = Graphic;
    UIElements[Current_Element].XPosPercent = XPosPercent;
    UIElements[Current_Element].YPosPercent = YPosPercent;
    UIElements[Current_Element].XAnchor = XAnchor;
    UIElements[Current_Element].YAnchor = YAnchor;
    UIElements[Current_Element].func = func;
    UIElements[Current_Element].Need_Clicked = Need_Clicked;
    UIElements[Current_Element].Activated = FALSE;

    Current_Element++;
}

// Font stuff
void Graphics_InitializeFont() {
    GameFont = sfFont_createFromFile("assets/font.ttf");
}

// Set up Text Element and bind to TextElem field/lib
void Graphics_InitializeTextElement(textelement_t* Element) {
    // init text
    Element->TextElem = sfText_create();

    // text content
    sfText_setString(Element->TextElem, Element->Text);
    sfText_setFont(Element->TextElem, GameFont);

    // size
    sfText_setCharacterSize(Element->TextElem, Element->Size);

    // color
    sfText_setColor(Element->TextElem, Element->Color);

    // Determine Our Offset values
    int OffsetX;
    int OffsetY;

    switch(Element->XAnchor) {
        case UI_ANCHOR_LEFT:
            OffsetX = 0;
            break;
        case UI_ANCHOR_CENTER:
            OffsetX = (sfText_getGlobalBounds(Element->TextElem).width)/2;
            break;
        case UI_ANCHOR_RIGHT:
            OffsetX = sfText_getGlobalBounds(Element->TextElem).width;
            break;
        default:
            OffsetX = 0;
            break;
    }

    switch(Element->YAnchor) {
        case UI_ANCHOR_TOP:
            OffsetY = 0;
            break;
        case UI_ANCHOR_CENTER:
            OffsetY = (sfText_getGlobalBounds(Element->TextElem).height)/2;
            break;
        case UI_ANCHOR_BOTTOM:
            OffsetY = sfText_getGlobalBounds(Element->TextElem).height;
            break;
        default:
            OffsetY = 0;
            break;
    }

    // Get our X & Y based on Anchors and Percentage
    double XPosition = I_GAME_WIDTH;
    double YPosition = I_GAME_HEIGHT;

    XPosition *= (Element->XPosPercent/(double)100);
    YPosition *= (Element->YPosPercent/(double)100);

    XPosition -= OffsetX;
    YPosition -= OffsetY;

    // Finally, set our Position
    sfText_setPosition(Element->TextElem, (sfVector2f) {XPosition, YPosition});

    // we're intialized! ready to draw
    Element->Initialized = TRUE;
}

// Draw our Text Elements
void Graphics_DrawText() {
    for (int i = 0; i < Current_TextElement; ++i) {
        if (!TextElements[i]->Initialized)
            Graphics_InitializeTextElement(TextElements[i]);

        sfRenderWindow_drawText(GameWindow, TextElements[i]->TextElem, NULL);
    }
}

// Register Text elements and add to array to draw
void Graphics_RegisterTextElement(textelement_t* Element) {
    TextElements[Current_TextElement] = Element;

    Element->id = Current_TextElement;

    Current_TextElement++;
}

// Update Text Elements (color, text, etc.)
void Graphics_UpdateTextElement(textelement_t* Element) {
    for (int i = 0; i < Current_TextElement; ++i) {
        if (TextElements[i]->id == Element->id) {
            TextElements[i] = Element;
            TextElements[i]->Initialized = FALSE;
        }
    }
}