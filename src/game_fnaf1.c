// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

//
// includes
//

#include "engine/types.h"
#include "engine/math.h"
#include "engine/version.h"
#include "engine/ai_handler.h"
#include "engine/options.h"
#include "engine/print.h"

#include <stdio.h>


anima_t Animas[G_NUM_ANIMAS];
camera_t Camera;
gamestate_t Game;

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

// Fred follows the set path of:
//
// Show Stage > Dining Area > Restrooms >
// Kitchen > East Hall > East Hall Corner
//
// Will linger, attempts to enter office that fail
// will send him back to the East Hall.
//
// TODO: Flesh out attack mode, footsteps
aitrigger_t AI_Fred_OnMove(void) {
    switch(Animas[A_FRED].Location) {
        case ROOMBIT(RM_SHOW_STAGE):
            Animas[A_FRED].Location = ROOMBIT(RM_DINING_AREA);
            break;
        case ROOMBIT(RM_DINING_AREA):
            Animas[A_FRED].Location = ROOMBIT(RM_RESTROOMS);
            break;
        case ROOMBIT(RM_RESTROOMS):
            Animas[A_FRED].Location = ROOMBIT(RM_KITCHEN);
            break;
        case ROOMBIT(RM_KITCHEN):
            Animas[A_FRED].Location = ROOMBIT(RM_EAST_HALL);
            break;
        case ROOMBIT(RM_EAST_HALL):
            Animas[A_FRED].Location = ROOMBIT(RM_EAST_HALL_CORNER);
            break;
    }

    Print_Normal("Freddy Moved to Room with ID %ld\n", Animas[A_FRED].Location);

    //footstep sound, evaluate next move ahead
    // Animas[A_FRED].Location = /*add stuff here*/;

    // Switch in/out of Attack Mode.
    if (Animas[A_FRED].Location == ROOMBIT(RM_EAST_HALL_CORNER))
        // Moto -- maybe have #defines for modes?
        Animas[A_FRED].AnimaMode = 1;
    else if (Animas[A_FRED].Location == ROOMBIT(RM_EAST_HALL) || Animas[A_FRED].Location == ROOMBIT(RM_OFFICE))
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
    Print_Normal("Foxy Moved\n");
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

// Bird can move randomly to any location on the right side
// of the building. This includes:
//
// * Kitchen (10)
// * Restrooms (9)
// * Dining Area (3)
// * East Hall (5)
// * East Hall Corner (6)
//
// Bird appears on the right door of the office.
//
// TODO: Footsteps, enter office.
aitrigger_t AI_Bird_OnMove(void) {
    u16_t Loc = Math_SeedRandom(101, 5);

    switch(Loc) {
        // Dining Area
        case 1:
            Loc = 3;
            break;
        // Restrooms
        case 2:
            Loc = 10;
            break;
        // Kitchen
        case 3:
            Loc = 9;
            break;
        // East Hall
        case 4:
            Loc = 5;
            break;
        // East Hall Corner
        case 5:
            Loc = 6;
            break;
        // Failsafe - Dining Area
        default:
            Loc = 3;
            break;
    }

    Animas[A_BIRD].Location = ROOMBIT(Loc);

    Print_Normal("Chica Moved to Room with ID %ld\n", Animas[A_BIRD].Location);
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

// Bun can move randomly to any location on the left side
// of the building, excluding PC. This includes:
//
// * Dining Area (3)
// * Backstage (2)
// * West Hall (7)
// * Supply Closet (11)
// * West Hall Corner (8)
//
// Bun appears on the left door of the office.
//
// TODO: Footsteps, enter office.
aitrigger_t AI_Bun_OnMove(void) {
    u16_t Loc = Math_SeedRandom(101, 5);

    switch(Loc) {
        // Dining Area
        case 1:
            Loc = 3;
            break;
        // Backstage
        case 2:
            Loc = 2;
            break;
        // West Hall
        case 3:
            Loc = 7;
            break;
        // Supply Closet
        case 4:
            Loc = 11;
            break;
        // West Hall Corner
        case 5:
            Loc = 8;
            break;
        // Failsafe - Dining Area
        default:
            Loc = 3;
            break;
    }

    Animas[A_BUN].Location = ROOMBIT(Loc);

    Print_Normal("Bonnie Moved to Room with ID %ld\n", Animas[A_BUN].Location);
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
            Animas[A_GFRED].Location = ROOMBIT(RM_OFFICE);
        }
    }
}

//
// G_GetAILevel()
// Return AI Level based on Anima and Night
//
u16_t G_GetAILevel(u16_t id) {
    if (id == A_FRED) {
          switch(Game.Night) {
            case 1: return 0;
            case 2: return 0;
            case 3: return 1;
            case 4: return 1; //FIXME - 50% chance of being 2 as opposed to 1
            case 5: return 3;
            default: return 0;
          }
    } else if (id == A_BUN) {
          switch(Game.Night) {
            case 1: return 0;
            case 2: return 3;
            case 3: return 0;
            case 4: return 2;
            case 5: return 5;
            default: return 0;
          }
    } else if (id == A_BIRD) {
          switch(Game.Night) {
            case 1: return 0;
            case 2: return 1;
            case 3: return 5;
            case 4: return 4;
            case 5: return 12;
            default: return 0;
          }
    } else if (id == A_FOX) {
          switch(Game.Night) {
            case 1: return 0;
            case 2: return 1;
            case 3: return 2;
            case 4: return 6;
            case 5: return 5;
            default: return 0;
          }
    }
              
    return 0;
}

//
// G_SetupAnimatronics()
// Set initial stats and updates for animatronics.
//

func_t G_SetupAnimatronics(void) {
  //fred
  Animas[A_FRED].OnUpdate = &AI_Fred_OnUpdate;
  Animas[A_FRED].OnCamUpdate = &AI_Fred_OnCamUpdate;
  Animas[A_FRED].OnMove = &AI_Fred_OnMove;
  Animas[A_FRED].UpdateTime = Animas[A_FRED].UpdateInterval = 3.03;
  Animas[A_FRED].AiLevel = G_GetAILevel(A_FRED);
  Animas[A_FRED].Location = ROOMBIT(RM_SHOW_STAGE);

  //fox
  Animas[A_FOX].OnUpdate = &AI_Fox_OnUpdate1;
  Animas[A_FOX].OnCamUpdate = &AI_Fox_OnCamUpdate;
  Animas[A_FOX].OnMove = &AI_Fox_OnMove;
  Animas[A_FOX].UpdateTime = Animas[A_FOX].UpdateInterval = 5.01;
  Animas[A_FOX].AiLevel = G_GetAILevel(A_FOX);
  Animas[A_FOX].Location = ROOMBIT(RM_PIRATE_COVE);

  //bird
  Animas[A_BIRD].OnUpdate = &AI_Bird_OnUpdate;
  Animas[A_BIRD].OnMove = &AI_Bird_OnMove;
  Animas[A_BIRD].UpdateTime = Animas[A_BIRD].UpdateInterval = 4.98;
  Animas[A_BIRD].AiLevel = G_GetAILevel(A_BIRD);
  Animas[A_BIRD].Location = ROOMBIT(RM_SHOW_STAGE);

  //bun
  Animas[A_BUN].OnUpdate = &AI_Bun_OnUpdate;
  Animas[A_BUN].OnMove = &AI_Bun_OnMove;
  Animas[A_BUN].UpdateTime = Animas[A_BUN].UpdateInterval = 4.96;
  Animas[A_BUN].AiLevel = G_GetAILevel(A_BUN);
  Animas[A_BUN].Location = ROOMBIT(RM_SHOW_STAGE);

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
// G_GetPrettyTime()
// Returns the AM time (12-6)
//
u16_t G_GetPrettyTime() {
    if (Game.Hour == 0)
        return 12;
    else
        return Game.Hour;
}

//
// G_AdvanceTime()
// Called to update the game Hour and do whatever
// else needs done per hour, such as ai updating
//
func_t G_AdvanceTime(void) {
    // Increase Hour var
    Game.Hour++;

    // Update ai values
    switch(Game.Hour) {
        case 2:
            if (Animas[A_BUN].AiLevel != 20) Animas[A_BUN].AiLevel++; break;
        case 3:
        case 4:
            if (Animas[A_BUN].AiLevel != 20) Animas[A_BUN].AiLevel++;
            if (Animas[A_BIRD].AiLevel != 20) Animas[A_BIRD].AiLevel++;
            if (Animas[A_FOX].AiLevel != 20) Animas[A_FOX].AiLevel++;
            break;
    }

    /*if (Game.Hour == 6)
        EndGame();*/

    Print_Normal("Time Advanced to %d AM\n", G_GetPrettyTime());
}

//
// G_Main()
// Game-specific initialization.
//

func_t G_Main(u16_t night) {
    // Init Animas and Rooms
    G_SetupAnimatronics();
    G_SetupRooms();

    // Define Night and Hour
    Game.Night = night;
    Game.Hour = 0;

    // Notify game start
    Print_Normal("Game Started on Night %d\n", Game.Night);
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

        AI_CheckMovement(&Animas[i]);
    }
    return;
}
