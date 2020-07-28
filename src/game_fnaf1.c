// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

//
// includes
//

#include "engine/defs.h"
#include <stdio.h>


anima_t Animas[G_NUM_ANIMAS];
camera_t Camera;
gamestate_t Game;
textelement_t Hour_Text;
textelement_t Night_Text;

textelement_t Power_Text;
textelement_t Power_Percent_Text;

char* NightString;
char* HourString;

char* PowerString;
char* PowerPercentString;

int Scroll_Method = SCROLL_DISABLED;

// Power
int Power;
int PowerDecreaseRate;

// Relating to Freddy after Power's out
int FreddyPowerCounter;
int FreddyState;

//
// globals
//

gameroom_t Rooms[G_NUM_ROOMS];
gameroom_t Current_Room;

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
// TODO: Flesh out attack mode
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

    // Footstep Sound
    // TODO: Change volume dependent on distance(?)
    Sound_Play(1, "assets/sounds/misc/footsteps.ogg", FALSE, 1, 100);

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

// Kill the player, spooky!
aitrigger_t AI_Fred_OnPowerLoss(void) {
    // Set the graphic to a spooky face (temporary)
    Current_Room.Graphic = "assets/textures/temp/spooky.png";
    Graphics_UpdateRoom(Current_Room);

    // Play the spooky scream
    Sound_Play(0, "assets/sounds/anima/scream.ogg", FALSE, 1, 100);
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
// TODO: enter office.
aitrigger_t AI_Bird_OnMove(void) {
    u16_t Loc = Math_SeedRandom(5);

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

    // Footstep Sound
    // TODO: Change volume dependent on distance(?)
    Sound_Play(1, "assets/sounds/misc/footsteps.ogg", FALSE, 1, 100);

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
// TODO: enter office.
aitrigger_t AI_Bun_OnMove(void) {
    u16_t Loc = Math_SeedRandom(5);

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

    // Footstep Sound
    // TODO: Change volume dependent on distance(?)
    Sound_Play(1, "assets/sounds/misc/footsteps.ogg", FALSE, 1, 100);

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
            case 4: return ((Math_GenerateChance(50)) ? 1 : 2);
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
    Animas[A_FRED].UpdateTime = Animas[A_FRED].UpdateInterval = 3.02;
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
    Animas[A_BUN].UpdateTime = Animas[A_BUN].UpdateInterval = 4.97;
    Animas[A_BUN].AiLevel = G_GetAILevel(A_BUN);
    Animas[A_BUN].Location = ROOMBIT(RM_SHOW_STAGE);

    //goldfred
    Animas[A_GFRED].OnCamUpdate = &AI_GFred_OnCamUpdate;
}

//
// G_SetupRooms()
// Link Rooms, set up graphics
//
func_t G_SetupRooms(void) {
    // Show Stage
    Rooms[RM_SHOW_STAGE].CanTravelTo = ROOMBIT(RM_DINING_AREA);
    Rooms[RM_SHOW_STAGE].Graphic = "assets/textures/rooms/show_stage/normal.png";

    // Dining Area
    Rooms[RM_DINING_AREA].CanTravelTo = ROOMBIT(RM_SHOW_STAGE) | ROOMBIT(RM_BACKSTAGE) | ROOMBIT(RM_RESTROOMS)
                                        | ROOMBIT(RM_PIRATE_COVE) | ROOMBIT(RM_WEST_HALL) | ROOMBIT(RM_EAST_HALL)
                                        | ROOMBIT(RM_KITCHEN);
    Rooms[RM_DINING_AREA].Graphic = "assets/textures/rooms/dining_room/normal.png";

    // Back Stage, Restrooms, Kitchen, & Pirate Cove
    Rooms[RM_BACKSTAGE].CanTravelTo = Rooms[RM_RESTROOMS].CanTravelTo = Rooms[RM_KITCHEN].CanTravelTo =
                                    Rooms[RM_PIRATE_COVE].CanTravelTo =  ROOMBIT(RM_DINING_AREA);
    Rooms[RM_BACKSTAGE].Graphic = "assets/textures/rooms/back_stage/normal.png";

    // West hall
    Rooms[RM_WEST_HALL].CanTravelTo = ROOMBIT(RM_WEST_HALL_CORNER) | ROOMBIT(RM_DINING_AREA) | ROOMBIT(RM_SUPPLY_CLOSET);
    Rooms[RM_WEST_HALL].Graphic = "assets/textures/rooms/west_hall/normal.png";

    // West Hall Corner
    Rooms[RM_WEST_HALL_CORNER].CanTravelTo = ROOMBIT(RM_OFFICE) | ROOMBIT(RM_WEST_HALL);
    Rooms[RM_WEST_HALL_CORNER].Graphic = "assets/textures/rooms/west_hall_corner/normal.png";

    // East Hall 
    Rooms[RM_EAST_HALL].CanTravelTo = ROOMBIT(RM_EAST_HALL_CORNER) | ROOMBIT(RM_DINING_AREA);
    Rooms[RM_EAST_HALL].Graphic = "assets/textures/rooms/east_hall/normal.png";

    // East Hall Corner
    Rooms[RM_EAST_HALL_CORNER].CanTravelTo = ROOMBIT(RM_EAST_HALL) | ROOMBIT(RM_OFFICE);
    Rooms[RM_EAST_HALL_CORNER].Graphic = "assets/textures/rooms/east_hall_corner/normal.png";

    // Office
    Rooms[RM_OFFICE].CanTravelTo = ROOMBIT(RM_EAST_HALL_CORNER) | ROOMBIT(RM_WEST_HALL_CORNER);
    Rooms[RM_OFFICE].Graphic = "assets/textures/rooms/office/normal.png";
}

//
// G_PrecacheSounds()
// Precache all loaded (non-streamed) sounds
//
func_t G_PrecacheSounds() {
    Sound_Precache("assets/sounds/misc/footsteps.ogg");
    Sound_Precache("assets/sounds/misc/power_out.ogg");
    Sound_Precache("assets/sounds/anima/scream.ogg");
}

// TEMP (FIXME -- Set up room GUIDs!)
func_t G_SwapRooms() {
    if (Game.State != 0)
        return;

    if (Current_Room.Graphic == Rooms[RM_OFFICE].Graphic) {
        Current_Room = Rooms[RM_BACKSTAGE];
        Scroll_Method = SCROLL_AUTOMATIC;
    } else {
        Current_Room = Rooms[RM_OFFICE];
        Scroll_Method = SCROLL_MANUAL;
    }

    Graphics_UpdateRoom(Current_Room);
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
// G_NightText()
// Set up Night-related HUD things
//
func_t G_NightText() {
    // X AM
    HourString = append_int_to_char("", G_GetPrettyTime());
    HourString = append_char_to_char(HourString, " AM");
    Hour_Text.Text = HourString;
    Hour_Text.XAnchor = UI_ANCHOR_RIGHT;
    Hour_Text.YAnchor = UI_ANCHOR_TOP;
    Hour_Text.Color = COLOR_GREEN;
    Hour_Text.Size = 35;
    Hour_Text.XPosPercent = 99;
    Hour_Text.YPosPercent = 0;

    // Night X
    NightString = "Night ";
    NightString = append_int_to_char(NightString, Game.Night);
    Night_Text.Text = NightString;
    Night_Text.XAnchor = UI_ANCHOR_RIGHT;
    Night_Text.YAnchor = UI_ANCHOR_TOP;
    Night_Text.Color = COLOR_GREEN;
    Night_Text.Size = 25;
    Night_Text.XPosPercent = 99;
    Night_Text.YPosPercent = 5;

    Graphics_RegisterTextElement(&Hour_Text);
    Graphics_RegisterTextElement(&Night_Text);
}

//
// G_PowerText()
// Text related to the Power meter
// TODO: Draw the meter bars
//
func_t G_PowerText() {
    PowerString = "Power left: ";
    Power_Text.Text = PowerString;
    Power_Text.XAnchor = UI_ANCHOR_LEFT;
    Power_Text.YAnchor = UI_ANCHOR_BOTTOM;
    Power_Text.Color = COLOR_GREEN;
    Power_Text.Size = 25;
    Power_Text.XPosPercent = 2;
    Power_Text.YPosPercent = 90;

    PowerPercentString = append_int_to_char("", Power);
    PowerPercentString = append_char_to_char(PowerPercentString, "%");
    Power_Percent_Text.Text = PowerPercentString;
    Power_Percent_Text.XAnchor = UI_ANCHOR_LEFT;
    Power_Percent_Text.YAnchor = UI_ANCHOR_BOTTOM;
    Power_Percent_Text.Color = COLOR_GREEN;
    Power_Percent_Text.Size = 30;
    Power_Percent_Text.XPosPercent = 12;
    Power_Percent_Text.YPosPercent = 90;

    Graphics_RegisterTextElement(&Power_Text);
    Graphics_RegisterTextElement(&Power_Percent_Text);
}

//
// G_PlayCall()
// Stream Phone OGG.
//
func_t G_PlayCall() {
    char* Sound;

    switch(Game.Night) {
        case 1:
            Sound = "assets/sounds/vo/night1.ogg";
            break;
        default:
            Sound = "assets/sounds/vo/night1.ogg";
            break;
    }

    Sound_Stream(0, Sound, FALSE, 1, 100);
}

//
// G_Main()
// Game-specific initialization.
//
func_t G_Main(u16_t night) {
    // Init Animas and Rooms
    G_SetupAnimatronics();
    G_SetupRooms();

    // Precache Sounds
    G_PrecacheSounds();

    // Define Game States
    Game.Night = night;
    Game.Hour = 0;
    Game.State = 0;

    // Power Percentage
    Power = 100;

    // Power Decrease Rate (in seconds)
    PowerDecreaseRate = ((Game.Night < 5) ? (6 - (Game.Night - 2)) : 3);

    if (Game.Night == 1)
        PowerDecreaseRate = 1000;

    // Set up controls
    Input_RegisterKey(KEY_RIGHT, G_SwapRooms);

    // Camera Enter Button
    Graphics_RegisterUIElement("assets/textures/ui/misc/open_camera.png", 50, 100, 
                                UI_ANCHOR_CENTER, UI_ANCHOR_BOTTOM, FALSE, G_SwapRooms);

    // Our HUD Elements
    G_NightText();
    G_PowerText();

    // Throw us into the Office
    Current_Room = Rooms[RM_OFFICE];

    // Manual Room Scrolling
    Scroll_Method = SCROLL_MANUAL;

    // Play Phone Call
    G_PlayCall();

    // Notify game start
    Print_Normal("Game Started on Night %d\n", Game.Night);

    // Reserve timedelay id 3 for power drain
    Time_FrameDelay(1000*PowerDecreaseRate, 3);
}

//
// G_EndGame()
// Called when the game reaches 6 am
// start new night, save, set all animas inactive
//
func_t G_EndGame() {
    // Print
    Print_Normal("Night %d Reached 6 AM, advancing..\n", Game.Night);

    // Deactivate all animas
    for(u16_t i = 0; i < G_NUM_ANIMAS; i++) {
        Animas[i].IsLockedDown = TRUE;
    }

    // Save, reset hour, start new night
    Game.Hour = 0;
    Save_SetValue("level", Game.Night + 1);
    G_Main(Game.Night + 1);
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
            if (Animas[A_BUN].AiLevel != 20) { Animas[A_BUN].AiLevel++; } break;
        case 3:
        case 4:
            if (Animas[A_BUN].AiLevel != 20) { Animas[A_BUN].AiLevel++ ; }
            if (Animas[A_BIRD].AiLevel != 20) { Animas[A_BIRD].AiLevel++ ; }
            if (Animas[A_FOX].AiLevel != 20) { Animas[A_FOX].AiLevel++ ; }
            break;
    }

    // End Game or update time on HUD
    if (Game.Hour == 6)
        G_EndGame();
    else
        Print_Normal("Time Advanced to %d AM\n", G_GetPrettyTime());

    HourString = append_int_to_char("", G_GetPrettyTime());
    HourString = append_char_to_char(HourString, " AM");
    Hour_Text.Text = HourString;
    NightString = "Night ";
    NightString = append_int_to_char(NightString, Game.Night);
    Night_Text.Text = NightString;
    Graphics_UpdateTextElement(&Hour_Text);
    Graphics_UpdateTextElement(&Night_Text);
}

//
// G_PowerOut
// Called when the Player runs out of Power
// TODO: Fred patch, hide camera button
//
func_t G_PowerOut() {
    // Cull all sounds
    for (int i = 0; i < MAX_SOUND_CHANNELS; ++i) {
        Sound_Stream(i, "assets/sounds/null.ogg", FALSE, 1, 100);
    }

    // Play the power shutoff sound
    Sound_Play(1, "assets/sounds/misc/power_out.ogg", FALSE, 1, 100);

    // Update room graphic and send us back to the office
    Current_Room = Rooms[RM_OFFICE];
    Scroll_Method = SCROLL_MANUAL;
    Current_Room.Graphic = "assets/textures/rooms/office/lights_out/whos_there.png";
    Graphics_UpdateRoom(Current_Room);

    // Hide all HUD elements
    Hour_Text.Text = "";
    Night_Text.Text = "";
    Power_Percent_Text.Text = "";
    Power_Text.Text = "";
    Graphics_UpdateTextElement(&Hour_Text);
    Graphics_UpdateTextElement(&Night_Text);
    Graphics_UpdateTextElement(&Power_Percent_Text);
    Graphics_UpdateTextElement(&Power_Text);

    // Update the Game State
    Game.State = 1;
}

//
// G_BlackOut()
// Looks like we didn't make it, wait to DIE
//
func_t G_BlackOut() {
    // Make room completely dark
    Scroll_Method = SCROLL_DISABLED;
    Current_Room.Graphic = "assets/textures/misc/black.png";
    Graphics_UpdateRoom(Current_Room);

    // Stop the marchin'!
    Sound_Stream(0, "assets/sounds/null.ogg", FALSE, 1, 100);

    // Update Game state
    Game.State = 2;
}

//
// G_GameLoop()
// Called every frame
//
func_t G_GameLoop(void) {
    // Normal Game Loop
    if (Game.State == 0) {
        // Run the AI movement checks
        for(int i = 0; i < G_NUM_ANIMAS; i++) {
            // We don't wanna check for golden freddy
            if (i != A_GFRED) {
                AI_CheckMovement(&Animas[i]);
            }
        }
        
        // Decrease Power over time
        if (Time_FrameReady(3)) {
            Power--;

            // Update the HUD
            PowerPercentString = append_int_to_char("", Power);
            PowerPercentString = append_char_to_char(PowerPercentString, "%");
            Power_Percent_Text.Text = PowerPercentString;
            Graphics_UpdateTextElement(&Power_Percent_Text);

            // Keep progressing through the night
            if (Power > 0) {
                Time_FrameDelay(PowerDecreaseRate*1000, 3);
            } else {
                // Power's Out!
                G_PowerOut();
                FreddyState = 0;
                Time_FrameDelay(5000, 3);
            }
        }
    }
    // Power Out Game Loop
    else if (Game.State == 1) {
        // Every 5s, there is a 20% chance freddy can do something
        if (Time_FrameReady(3)) {
            if (Math_GenerateChance(20) || FreddyPowerCounter >= 4) {
                if (FreddyState == 0) {
                    FreddyState = 1;
                    FreddyPowerCounter = 0;
                    Sound_Stream(0, "assets/sounds/music/toreador.ogg", FALSE, 1, 100);
                } else if (FreddyState == 1) {
                    G_BlackOut();
                    Time_FrameDelay(2000, 3);
                } 
            } else {
                FreddyPowerCounter++;
            }

            Time_FrameDelay(5000, 3);
        }
    }
    // Wait to die after BlackOut
    else if (Game.State == 2) {
        if (Time_FrameReady(3)) {
            // We're Losers ;(
            if (Math_GenerateChance(20)) {
                AI_Fred_OnPowerLoss();
                Game.State = 100; //temporary
                Time_FrameDelay(34464, 3);
            } else {
                Time_FrameDelay(2000, 3);
            }
        }
    }
}
