// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

textelement_t* TextElements[MAX_UI_ELEMENTS];
uidata_t* UIElements[MAX_UI_ELEMENTS];

// FIXME - cleanup
int Current_Element = 0;
int Current_TextElement = 0;

texture2d_t* RoomTexture;
sprite_t* RoomSprite;

font_t* GameFont;

float RoomPanX = 0;
float CameraPanX = 0;

// Initialization of Room drawings
void Graphics_InitRoom(gameroom_t Room) {

    RoomTexture = Graphics_CreateTextureFromFile(Room.Graphic);

    // If Texture doesn't exist (TODO)
    /*if (!RoomTexture)
        Sys_Error("Failed to initialize texture %s\n", Room.Graphic);*/

    // Init Sprite, bind Texture to it
    RoomSprite = Graphics_CreateSprite();

    Graphics_BindTextureToSprite(RoomTexture, RoomSprite);
}

// Free and then re-init
void Graphics_UpdateRoom(gameroom_t Room) {
    free(RoomTexture);
    free(RoomSprite);

    Graphics_InitRoom(Room);
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

    Graphics_DrawSprite(RoomSprite, GameWindow);
}

// Initialize UI Elements if not already done
void Graphics_InitializeUIElement(uidata_t* UIElement) {
    // If we don't have a graphic, don't bother
    if (!UIElement->Graphic)
        return;

    // Set it's sprites and textures
    UIElement->Texture = Graphics_CreateTextureFromFile(UIElement->Graphic);
    UIElement->Sprite = Graphics_CreateSprite();

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
        if (!UIElements[i]->Initialized) 
            Graphics_InitializeUIElement(UIElements[i]);

        if (UIElements[i]->Visible)
            Graphics_DrawSprite(UIElements[i]->Sprite, GameWindow);
    }
}

// Registration of UI Elements to the array
void Graphics_RegisterUIElement(uidata_t* Element) {
    UIElements[Current_Element] = Element;

    Element->id = Current_Element;

    Current_Element++;
}

// Update UI Elements
void Graphics_UpdateUIElement(uidata_t* Element) {
    for (int i = 0; i < Current_Element; ++i) {
        if (UIElements[i]->id == Element->id) {
            UIElements[i] = Element;
            UIElements[i]->Initialized = FALSE;
        }
    }
}

// Font stuff
void Graphics_InitializeFont() {
    GameFont = Graphics_CreateFontFromFile("assets/font.ttf");
}

// Set up Text Element and bind to TextElem field/lib
void Graphics_InitializeTextElement(textelement_t* Element) {
    // init text
    Element->TextElem = Graphics_CreateText();

    // text content
    Graphics_SetTextString(Element->TextElem, Element->Text);
    Graphics_SetTextFont(Element->TextElem, GameFont);

    // size
    Graphics_SetTextSize(Element->TextElem, Element->Size);

    // color
    Graphics_SetTextColor(Element->TextElem, Element->Color);

    // Determine Our Offset values
    int OffsetX;
    int OffsetY;

    switch(Element->XAnchor) {
        case UI_ANCHOR_LEFT:
            OffsetX = 0;
            break;
        case UI_ANCHOR_CENTER:
            OffsetX = Graphics_GetTextWidth(Element->TextElem)/2;
            break;
        case UI_ANCHOR_RIGHT:
            OffsetX = Graphics_GetTextWidth(Element->TextElem);
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
            OffsetY = Graphics_GetTextHeight(Element->TextElem)/2;
            break;
        case UI_ANCHOR_BOTTOM:
            OffsetY = Graphics_GetTextHeight(Element->TextElem);
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
    Graphics_SetTextPosition(Element->TextElem, XPosition, YPosition);

    // we're intialized! ready to draw
    Element->Initialized = TRUE;
}

// Draw our Text Elements
void Graphics_DrawTextElements() {
    for (int i = 0; i < Current_TextElement; ++i) {
        if (!TextElements[i]->Initialized)
            Graphics_InitializeTextElement(TextElements[i]);

        Graphics_DrawText(TextElements[i]->TextElem, GameWindow);
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