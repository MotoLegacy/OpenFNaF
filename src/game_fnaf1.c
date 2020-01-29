//
// includes
//

#include "engine/types.h"
#include "engine/math.h"
#include "engine/version.h"
#include "engine/ai_handler.h"

// TEMP
#include <stdio.h>

//
// externs
//

anima_t Animas[G_NUM_ANIMAS];
camera_t Camera;

//
// globals
//

gameroom_t Rooms[G_NUM_ROOMS];

//
// ai functions
//

//
// generic
//

aitrigger_t AI_Generic_OnMove(void) {
  //evaluate next possible moves in a more generic way
}

aitrigger_t AI_Generic_NoAction(void) {
  //do nothing
}

//
// fred
//

aitrigger_t AI_Fred_OnUpdate(void) {
  //move if allowed
}

aitrigger_t AI_Fred_OnAmbientUpdate(void) {
  //play creepy laugh
}

aitrigger_t AI_Fred_OnCamUpdate(void) {
  // If we're in attack mode.
  if (Animas[A_FRED].AnimaMode == 1) {
    if (Camera.ViewLocation == CAM_4B)
      Animas[A_FRED].IsLockedDown = TRUE;
    else
      Animas[A_FRED].IsLockedDown = FALSE;
  } else { // We're roamin' like normal
    if (Camera.CameraInUse)
      Animas[A_FRED].IsLockedDown = TRUE;
    else
      Animas[A_FRED].IsLockedDown = FALSE;
  }
}

aitrigger_t AI_Fred_OnMove(void) {
  printf("FRED MOVED\n");
  //footstep sound, evaluate next move ahead
  // Animas[A_FRED].Location = /*add stuff here*/;

  // Switch in/out of Attack Mode.
  if (Animas[A_FRED].Location == RM_EAST_HALL_CORNER)
    // Moto -- maybe have #defines for modes?
    Animas[A_FRED].AnimaMode = 1;
  else if (Animas[A_FRED].Location == RM_EAST_HALL || Animas[A_FRED].Location == RM_OFFICE)
    Animas[A_FRED].AnimaMode = 0;
}

aitrigger_t AI_Fred_OnKill(void) {
  //does fred set any specific game over variables?
  //jumpscare trigger
}

aitrigger_t AI_Fred_OnPowerLoss(void) {
  //the usual that happens when you lose power
}

//
// fox
//

aitrigger_t AI_Fox_OnUpdate1(void) {
  //phases of leaving PC
  //if we've totally left PC, retarget OnUpdate = OnUpdate2
}

aitrigger_t AI_Fox_OnUpdate2(void) {
  //run down the hall, drain door power if moving into office prohibited
}

aitrigger_t AI_Fox_OnAmbientUpdate(void) {
  //pirate jig sound
}

aitrigger_t AI_Fox_OnCamUpdate(void) {
    // decide whether or not to try and advance outta the cove.
    if (Camera.CameraInUse && Animas[A_FOX].AnimaMode != 2) {
      Animas[A_FOX].IsLockedDown = TRUE;
    } else {
      Animas[A_FOX].IsLockedDown = FALSE;
    }
}

aitrigger_t AI_Fox_OnMove(void) {
  printf("FOX MOVED\n");
  //in case we're forgetting anything special about him running down the hall
}

aitrigger_t AI_Fox_OnKill(void) {
  //jumpscare trigger
}

//
// bird
//

aitrigger_t AI_Bird_OnUpdate(void) {
  //move if allowed
}

aitrigger_t AI_Bird_OnCamUpdate(void) {
  //?
}

aitrigger_t AI_Bird_OnMove(void) {
  printf("BIRD MOVED\n");
  //footstep sound, evaluate next move ahead
  // Animas[A_BIRD].Location = /*add stuff here*/;
}

aitrigger_t AI_Bird_OnKill(void) {
  //jumpscare trigger
}

//
// bun
//

aitrigger_t AI_Bun_OnUpdate(void) {
  //move if allowed
}

aitrigger_t AI_Bun_OnCamUpdate(void) {
  //?
}

aitrigger_t AI_Bun_OnMove(void) {
  printf("BUN MOVED\n");
  //footstep sound, evaluate next move ahead
  // Animas[A_BUN].Location = /*add stuff here*/;
}

aitrigger_t AI_Bun_OnKill(void) {
  //jumpscare trigger
}

//
// gfred
//

aitrigger_t AI_GFred_OnCamUpdate(void) {
    // if we're looking at east hall corner
    if (Camera.ViewLocation == CAM_2B) {
        // 5% chance of occuring
        if (Math_GenerateChance(5)) {
            // FIXME - insert some garbage about poster here
            // FIXME - maybe have an if to check if can tp here (for consistency)
            Animas[A_GFRED].Location = Rooms[RM_OFFICE].guid;
        }
    }
}

//
// G_SetupAnimatronics()
// Set initial stats and updates for animatronics.
//

// FIXME - just using night 2 ai levels for now
func_t G_SetupAnimatronics(void) {
  //fred
  Animas[A_FRED].OnUpdate = &AI_Fred_OnUpdate;
  Animas[A_FRED].OnCamUpdate = &AI_Fred_OnCamUpdate;
  Animas[A_FRED].OnMove = &AI_Fred_OnMove;
  Animas[A_FRED].UpdateTime = 3.02;
  Animas[A_FRED].AiLevel = 0;

  //fox
  Animas[A_FOX].OnUpdate = &AI_Fox_OnUpdate1;
  Animas[A_FOX].OnCamUpdate = &AI_Fox_OnCamUpdate;
  Animas[A_FOX].OnMove = &AI_Fox_OnMove;
  Animas[A_FOX].UpdateTime = 5.01;
  Animas[A_FOX].AiLevel = 1;

  //bird
  Animas[A_BIRD].OnUpdate = &AI_Bird_OnUpdate;
  Animas[A_BIRD].OnMove = &AI_Bird_OnMove;
  Animas[A_BIRD].UpdateTime = 4.98;
  Animas[A_BIRD].AiLevel = 1;

  //bun
  Animas[A_BUN].OnUpdate = &AI_Bun_OnUpdate;
  Animas[A_BUN].OnMove = &AI_Bun_OnMove;
  Animas[A_BUN].UpdateTime = 4.96; //FIXME - 4.97 just.. doesn't work?
  Animas[A_BUN].AiLevel = 3;

  //goldfred
  Animas[A_GFRED].OnCamUpdate = &AI_GFred_OnCamUpdate;
}

//
// G_SetupRooms()
// Link all of the Rooms in the pizzeria together
//

func_t G_SetupRooms(void) {
    // Show Stage
    Rooms[RM_SHOW_STAGE].CanTravelTo = ROOMBIT(RM_DINING_AREA);
    // Dining Area
    Rooms[RM_DINING_AREA].CanTravelTo = ROOMBIT(RM_SHOW_STAGE) | ROOMBIT(RM_BACKSTAGE) | ROOMBIT(RM_RESTROOMS)
                                        | ROOMBIT(RM_PIRATE_COVE) | ROOMBIT(RM_WEST_HALL) | ROOMBIT(RM_EAST_HALL)
                                        | ROOMBIT(RM_KITCHEN);
    // Back Stage, Restrooms, Kitchen, & Pirate Cove
    Rooms[RM_BACKSTAGE].CanTravelTo = Rooms[RM_RESTROOMS].CanTravelTo = Rooms[RM_KITCHEN].CanTravelTo =
                                    Rooms[RM_PIRATE_COVE].CanTravelTo =  ROOMBIT(RM_DINING_AREA);
    // West hall
    Rooms[RM_WEST_HALL].CanTravelTo = ROOMBIT(RM_WEST_HALL_CORNER) | ROOMBIT(RM_DINING_AREA) | ROOMBIT(RM_SUPPLY_CLOSET);
    // West Hall Corner
    Rooms[RM_WEST_HALL_CORNER].CanTravelTo = ROOMBIT(RM_OFFICE) | ROOMBIT(RM_WEST_HALL);
    // East Hall
    Rooms[RM_EAST_HALL].CanTravelTo = ROOMBIT(RM_EAST_HALL_CORNER) | ROOMBIT(RM_DINING_AREA);
    // East Hall Corner
    Rooms[RM_EAST_HALL_CORNER].CanTravelTo = ROOMBIT(RM_EAST_HALL) | ROOMBIT(RM_OFFICE);
    // Office
    Rooms[RM_OFFICE].CanTravelTo = ROOMBIT(RM_EAST_HALL_CORNER) | ROOMBIT(RM_WEST_HALL_CORNER);
}

//
// G_Main()
// Game-specific initialization.
//

func_t G_Main(void) {
    G_SetupAnimatronics();
    G_SetupRooms();
}

//
// G_GameLoop()
// Called every frame
//
func_t G_GameLoop(void) {
    // Run the AI movement checks
    for(int i = 0; i < G_NUM_ANIMAS; i++) {
        // We don't wanna check for golden freddy
        if (i == A_GFRED)
            return;

        AI_CheckMovement(Animas[i]);
    }
    return;
}