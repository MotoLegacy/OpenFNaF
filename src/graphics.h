#define MAX_IMAGE       64

#ifdef DESKTOP
typedef sfTexture       texture2d_t;
typedef sfSprite        sprite_t;
#elif PSP
typedef OSL_IMAGE       texture2d_t;
#endif

typedef struct {
    texture2d_t* texture;
#ifdef DESKTOP
    sprite_t*    sprite;
#endif
    char*        path;
} image2d_t;

extern texture2d_t* Graphics_CreateTextureFromFile(char* Directory);
extern void         Graphics_DrawImageGeneric(image2d_t* Texture, int X, int Y);
extern void         Graphics_LoadImage(char* Directory);
extern void         Graphics_DrawImage(char* Directory, int X, int Y);

#ifdef DESKTOP
extern void         Graphics_BindTextureToSprite(texture2d_t* Texture, sprite_t* Sprite);
extern sprite_t*    Graphics_CreateSprite();
#endif