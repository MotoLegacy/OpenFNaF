//
// types.hpp
///////////////////////////////////////
// added anima defs. -xaa 12/4/2019
//

#ifndef _TYPES_H_
#define _TYPES_H_

#include "version.hpp"

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

typedef u64_t bitfield_t;

// !
typedef void    func_t;
typedef func_t  aitrigger_t;

//graphics types
// FIXME - make platform independent and replace u64 - moto
typedef u64_t texture2d_t;

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
  texture2d_t Graphic;
  s32_t CamBound;    //negative for left, positive for right. assumes we can go CamBound pixels right and (CamBound)(-1) pixels left
  u64_t guid;
} gameroom_t;

// anima_t
// holds animatronic data
typedef struct {
  u32_t AnimaMode             = 0; // 0: Normal, 1: Attack/Running (Freddy/Foxy, respectively)
  bool IsLockedDown;
  u32_t AiLevel               = 0;
  u32_t MoveChance            = 0;
  u32_t Location;
  func_t* OnUpdate;
  func_t* OnAmbientUpdate;
  func_t* OnCamUpdate;
  func_t* OnDraw;
  func_t* OnMove;
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

  //general variables
  u64_t State;
  u64_t GState; //?
  u64_t LoadState = 0;
  u64_t GameInit = 0;
  s64_t ScrollX = -124; //scroll state of the room
  u64_t GM_HOUR;
  u64_t GM_NIGHT;
  
  //animatronic states
  animastate_t GM_BEAR;
  animastate_t GM_BUNNY;
  animastate_t GM_BIRD;
  animastate_t GM_FOX;

  //light & power states
  lightstate_e LightLeft;
  lightstate_e LightRight;
  u64_t power;
  
  //door states
  doorstate_e DoorLeftSwitched;
  doorstate_e DoorRightSwitched;
  
  //other switches
  bool PressC    = false;
  bool PressT    = false;
  bool FredScare = false;
  
  //various game timers
  u64_t ScareTicker  = 0;
  u64_t StaticTicker = 0;
  u64_t FanTicker = 0;
  u64_t FlickerTicker = 0;
  u64_t IntroTicker = 0;
  u64_t HourTicker = 0;

} gamestate_t;

// vswap_t
// holds static pointers to graphics.
// name is a holdover from Wolf3D, change at your discretion
// (in other words, i was lazy)
typedef struct {

  //methods
  //example: Graphics.DrawText(...);
  // FIXME - make platform independent and replace null pointers - moto
  func_t* DrawText    = nullptr;
  func_t* DrawTexture = nullptr;
  func_t* Free        = nullptr;
  func_t* Load        = nullptr;

  //static texture pages start
  //scene
  texture2d_t gfont;
  texture2d_t clock;
  texture2d_t office_on;
  texture2d_t office_onL;
  texture2d_t office_onR;
  texture2d_t fan1;
  texture2d_t fan2;
  texture2d_t fan3;
  texture2d_t ldoor1;
  texture2d_t ldoor2;
  texture2d_t ldoor3;
  texture2d_t ldoor4;
  texture2d_t ldoor5;
  texture2d_t ldoor6;
  texture2d_t ldoor7;
  texture2d_t ldoor8;
  texture2d_t ldoor9;
  texture2d_t ldoor10;
  texture2d_t ldoor11;
  texture2d_t ldoor12;
  texture2d_t ldoor13;
  texture2d_t rdoor1;
  texture2d_t rdoor2;
  texture2d_t rdoor3;
  texture2d_t rdoor4;
  texture2d_t rdoor5;
  texture2d_t rdoor6;
  texture2d_t rdoor7;
  texture2d_t rdoor8;
  texture2d_t rdoor9;
  texture2d_t rdoor10;
  texture2d_t rdoor11;
  texture2d_t rdoor12;
  texture2d_t rdoor13;

  //static
  texture2d_t staticc0;
  texture2d_t staticc1;
  texture2d_t staticc2;
  texture2d_t staticc3;
  texture2d_t staticc4;
  texture2d_t staticc5;
  texture2d_t staticc6;
  texture2d_t staticc7;

  //fredly fazman
  texture2d_t fredscare1;
  texture2d_t fredscare2;
  texture2d_t fredscare3;
  texture2d_t fredscare4;
  texture2d_t fredscare5;
  texture2d_t fredscare6;
  texture2d_t fredscare7;
  texture2d_t fredscare8;
  texture2d_t fredscare9;
  texture2d_t fredscare10;
  texture2d_t fredscare11;
  texture2d_t fredscare12;
  texture2d_t fredscare13;
  texture2d_t fredscare14;
  texture2d_t fredscare15;
  texture2d_t fredscare16;
  texture2d_t fredscare17;
  texture2d_t fredscare18;
  texture2d_t fredscare19;
  texture2d_t fredscare20;
  texture2d_t fredscare21;
  texture2d_t fredscare22;
  texture2d_t fredscare23;
  texture2d_t fredscare24;
  texture2d_t fredscare25;
  texture2d_t fredscare26;
  texture2d_t fredscare27;
  texture2d_t fredscare28;
  texture2d_t fredscare29;
  texture2d_t fredscare30;

} vswap_t;

#endif