// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// Typedefs
typedef sfColor             color_t;
typedef sfText              text_t;
typedef sfTexture           texture2d_t;
typedef sfSprite            sprite_t;
typedef sfRenderWindow      window_t;
typedef sfFont              font_t;

// Colors
#define COLOR_RED           (sfColor) {255, 0, 0, 255}
#define COLOR_GREEN         (sfColor) {0, 255, 0, 255}
#define COLOR_BLUE          (sfColor) {0, 0, 255, 255}
#define COLOR_WHITE         (sfColor) {255, 255, 255, 255}
#define COLOR_BLACK         (sfColor) {0, 0, 0, 255}

// Wrapped Functions
extern texture2d_t* Graphics_CreateTextureFromFile(char* Directory);
extern int Graphics_GetTextureWidth(texture2d_t* Texture);
extern int Graphics_GetTextureHeight(texture2d_t* Texture);
extern sprite_t* Graphics_CreateSprite();
extern void Graphics_DrawSprite(sprite_t* Sprite, window_t* Window);
extern int Graphics_GetSpriteXPosition(sprite_t* Sprite);
extern int Graphics_GetSpriteYPosition(sprite_t* Sprite);
extern void Graphics_SetSpritePosition(sprite_t* Sprite, float x, float y);
extern void Graphics_BindTextureToSprite(texture2d_t* Texture, sprite_t* Sprite);
extern font_t* Graphics_CreateFontFromFile(char* Directory);
extern text_t* Graphics_CreateText();
extern void Graphics_DrawText(text_t* Text, window_t* Window);
extern void Graphics_SetTextString(text_t* Elem, char* Text);
extern void Graphics_SetTextFont(text_t* Elem, font_t* Font);
extern void Graphics_SetTextSize(text_t* Elem, int Size);
extern void Graphics_SetTextColor(text_t* Elem, color_t Color);
extern void Graphics_SetTextPosition(text_t* Elem, double x, double y);
extern int Graphics_GetTextWidth(text_t* Elem);
extern int Graphics_GetTextHeight(text_t* Elem);