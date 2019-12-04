//
// includes
//

#include "types.hpp"

//
// externs
//

extern anima_t Animas[G_NUM_ANIMAS];

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
  //?
}

aitrigger_t AI_Fred_OnMove(void) {
  //footstep sound, evaluate next move ahead
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
  //set preference values for how often the player's looking at him
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