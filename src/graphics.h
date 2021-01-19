#define MAX_IMAGE       64

#ifdef DESKTOP
typedef Texture2D       texture2d_t;
#elif PSP
typedef OSL_IMAGE       texture2d_t;
#endif

typedef struct {
    texture2d_t* texture;
    char*        path;
} image2d_t;

extern texture2d_t* Graphics_CreateTextureFromFile(char* Directory);
extern void         Graphics_DrawImageGeneric(texture2d_t* Image, int X, int Y);
extern void         Graphics_LoadImage(char* Directory);
extern void         Graphics_DrawImage(char* Directory, int X, int Y);
extern void         Graphics_StartDrawing();
extern void         Graphics_StopDrawing();