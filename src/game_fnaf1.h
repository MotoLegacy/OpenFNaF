#ifndef _GFNAF1_H_
#define _GFNAF1_H_

#include "engine/types.h"
#include "engine/math.h"
#include "engine/version.h"
#include "engine/ai_handler.h"
#include "engine/options.h"
#include "engine/print.h"
#include "engine/input.h"
#include "engine/graphics.h"
#include "engine/game.h"
#include "engine/convert.h"

#include <stdio.h>

//
// room data
//
#define RM_OFFICE             0
#define RM_SHOW_STAGE         1
#define RM_BACKSTAGE          2
#define RM_DINING_AREA        3
#define RM_PIRATE_COVE        4
#define RM_EAST_HALL          5
#define RM_EAST_HALL_CORNER   6
#define RM_WEST_HALL          7
#define RM_WEST_HALL_CORNER   8
#define RM_RESTROOMS          9
#define RM_KITCHEN            10
#define RM_SUPPLY_CLOSET      11

//
// anima states
//
state_t Patches[] = {
    { "NONE",                   "",                                         NULL,           NULL },
    { "bonnie_in_showstage",    "assets/textures/rooms/",                   RM_SHOW_STAGE,  NULL },
    { "bonnie_in_office",       "assets/textures/rooms/office/bon_0.png",   RM_OFFICE,      NULL },
};

typedef enum {
    NONE                = 0,
    bonnie_in_showstage = 1,
    bonnie_in_office    = 2,
} animastate_t;

#endif