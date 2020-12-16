-- Game States
STATE_NONE              = 0
STATE_IN_MENU           = 1
STATE_IN_GAME           = 2

Game_Night = 1
Game_State = STATE_NONE

function G_SetupAnimatronics()
    --CoolPrint("do setup animas here")
end

function G_SetupRooms()
    --CoolPrint("do setup rooms here")
end

--[[
//
// G_PlayCall
// Stream Call sound
//
]]
function G_PlayCall()
    local Sound = ""

    if Game_Night == 1 then Sound = "sounds/vo/night1"
    end

    Sound_Stream(0, Sound, false, 1, 100)
end

--[[
//
// G_PrecacheSounds()
// Precache all loaded (non-streamed) sounds
//
]]
function G_PrecacheSounds()
    --Sound_Precache("sounds/misc/footsteps")
    Sound_Precache("sounds/misc/power_out")
    --Sound_Precache("sounds/anima/scream")
end

--[[
//
// G_Main()
// Game-specific night initialization
//
]]
function G_Main()
    -- Load our other scripts
    OF_LoadScript("input.lua")
    OF_LoadScript("sound.lua")
    OF_LoadScript("menu.lua")

    -- Initialize the Menu
    Menu_Initialize()

    -- Init Animas and Rooms
    --[[G_SetupAnimatronics()
    G_SetupRooms()

    -- Precache Sounds
    G_PrecacheSounds()

    Sound_Play(1, "assets/sounds/misc/power_out", false, 1, 100)

    -- Play Phone Call
    G_PlayCall()]]--
end

--[[
//
// G_GameLoop()
// Called every Frame
//
]]
function G_GameLoop()
    if Game_State == STATE_IN_MENU then
        Menu_Loop()
    end
    --[[if Input_ButtonDown(INPUT_SELECT) then
        CoolPrint("nice select press bro")
    end

    if Input_KeyDown(INPUT_KEY_F) then
        CoolPrint("respects have been paid, bro")
    end]]
end