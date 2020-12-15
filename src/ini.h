// Game Data Struct
typedef struct {
    char* name;
} gamedata_t;

extern void INI_Initialize();

// Currently limited to only finding/dumping 8 total games..
// (FIXME??)
extern gamedata_t* INI_Games[8];