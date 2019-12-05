//
// includes
//

#include "types.hpp"

//
// externs
//

extern anima_t Animas[G_NUM_ANIMAS];
extern camera_t Camera;

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
    // restrict fred's movement if we're starring at him
    if (Camera.ViewLocation == Animas[A_FRED].Location) {
        Animas[A_FRED].Update_Multiplier = 0;
    } else {
        Animas[A_FRED].Update_Multiplier = 1;
    }
}

aitrigger_t AI_Fred_OnMove(void) {
  //footstep sound, evaluate next move ahead
  // Animas[A_FRED].Location = /*add stuff here*/;
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

    // see if we're looking at pirate cove
    if (Camera.ViewLocation == CAM_3) {
        // if we look too often and ai level is high enough, increase multiplier
        // moto FIXME - find real ai level and add frequency check
        if (Animas[A_FOX].AiLevel >= 5 /*&& (FIXME - frequency check)*/)
            Animas[A_FOX].Update_Multiplier = 2;
        else // else, just restrict movement
            Animas[A_FOX].Update_Multiplier = 0;
    }
}

aitrigger_t AI_Fox_OnMove(void) {
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
  //footstep sound, evaluate next move ahead
  // Animas[A_BUN].Location = /*add stuff here*/;
}

aitrigger_t AI_Bun_OnKill(void) {
  //jumpscare trigger
}

//
// G_SetupAnimatronics()
//

func_t G_SetupAnimatronics(void) {
  //fred
  Animas[A_FRED].OnUpdate = &AI_Fred_OnUpdate;

  //fox
  Animas[A_FOX].OnUpdate = &AI_Fox_OnUpdate1;

  //bird
  Animas[A_BIRD].OnUpdate = &AI_Bird_OnUpdate;

  //bun
  Animas[A_BUN].OnUpdate = &AI_Bun_OnUpdate;
}