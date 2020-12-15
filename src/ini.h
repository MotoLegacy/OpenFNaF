// Game Data Struct
typedef struct {
    bool    occupied;
    bool    supports_pc;
    bool    supports_psp;
    int     window_width;
    int     window_height;
    char*   name;
    char*   game_path;
} gamedata_t;

extern void INI_Initialize();

// Currently limited to only finding/dumping 8 total games..
// (FIXME??)
extern gamedata_t INI_Games[8];