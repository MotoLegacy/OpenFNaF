// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

//
// types.hpp
///////////////////////////////////////
// added anima defs. -xaa 12/4/2019
//

#ifndef _TYPES_H_
#define _TYPES_H_

#include "version.h"

#define TRUE    1
#define FALSE   0

//
// macros - generic
//

// rooms
#define ROOMBIT(x) (1 << x)

//
// macros - fnaf1
//

#ifdef GAME_FNAF1

  // animas
  #define A_FRED    0
  #define A_FOX     1
  #define A_BIRD    2
  #define A_BUN     3
  #define A_GFRED   4

  // camera (fnaf1 had sub-room cameras that do not appear in other games)
  // substitute xA cams for full num (e.g. cam 1a is cam 1)
  #define CAM_1B    12
  #define CAM_1C    13
  #define CAM_2B    14
  #define CAM_4B    15

  // rooms
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

  // clock
  #define SECONDS_PER_HOUR      90

#endif

//
// short types
//

typedef unsigned char       u8_t;
typedef unsigned short      u16_t;
typedef unsigned long       u32_t;
typedef unsigned long long  u64_t;

typedef signed char       s8_t;
typedef signed short      s16_t;
typedef signed long       s32_t;
typedef signed long long  s64_t;

typedef u64_t bool;

typedef u64_t bitfield_t;

// !
typedef void    func_t;
typedef func_t  aitrigger_t;

// game specific types
typedef u64_t animastate_t;
  

//
// enums
//

// doorstate_e
// if non-zero, we should do something to the door.
// the idea is that this can be used as a tic counter,
// stopping when we reach zero
typedef enum {
  door_opening = -512,
  door_closed  = 0,
  door_closing = 512
} doorstate_e;

// lightstate_e
// determines the state of the light
// for example, if a light should flicker:
// if(lightstate_e > light_solid) lightstate_e--;
typedef enum {
  light_off          = 0,
  light_solid        = 1,
  light_flickermax   = 65535
} lightstate_e;

//
// structs
//

// gameroom_t
// holds information on rooms
typedef struct {
  bitfield_t CanTravelTo : 64;
  char* Graphic;
  s32_t CamBound;    //negative for left, positive for right. assumes we can go CamBound pixels right and (CamBound)(-1) pixels left
  u64_t guid;
} gameroom_t;

// anima_t
// holds animatronic data
typedef struct {
  u32_t AnimaMode; // 0: Normal, 1: Attack/Running (Freddy/Foxy, respectively)
  float UpdateTime; // FIXME - make typedef for floats?
  float UpdateInterval;
  bool IsLockedDown;
  u32_t AiLevel;
  u32_t MoveChance;
  u32_t Location;
  func_t* OnUpdate;
  func_t* OnAmbientUpdate;
  func_t* OnCamUpdate;
  func_t* OnDraw;
  void (*OnMove)(); // Why can't we use our typedef for function pointers..?
  func_t* OnKill;
  func_t* OnPowerLoss;
} anima_t;

// camera_t
// holds player camera information
typedef struct {
  u32_t ViewLocation;
  bool CameraInUse;
} camera_t;

// gamestate_t
// holds all the game variables
typedef struct {
  u8_t Hour;
  u8_t Night;
} gamestate_t;

// vswap_t
// holds static pointers to graphics.
// name is a holdover from Wolf3D, change at your discretion
// (in other words, i was lazy)
typedef struct {

  //methods
  //example: Graphics.DrawText(...);
  // FIXME - make platform independent and replace null pointers - moto
  func_t* DrawText;
  func_t* DrawTexture;
  func_t* Free;
  func_t* Load;
} vswap_t;

#endif