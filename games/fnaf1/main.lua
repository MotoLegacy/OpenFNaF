Game_Night = 1

function CoolPrint(string)
    if OF_GetPlatform() == "PSP" then
        PSPDebugPrintf(string .. "\n")
    elseif OF_GetPlatform() == "PC" then
        print(string)
    end
end

function G_SetupAnimatronics()
    CoolPrint("do setup animas here")
end

function G_SetupRooms()
    CoolPrint("do setup rooms here")
end

--[[
//
// G_PlayCall
// Stream Call sound
//
]]
function G_PlayCall()
    local Sound = ""

    if Game_Night == 1 then Sound = "assets/sounds/vo/night1"
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
    --Sound_Precache("assets/sounds/misc/footsteps")
    Sound_Precache("assets/sounds/misc/power_out")
    --Sound_Precache("assets/sounds/anima/scream")
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

    -- Init Animas and Rooms
    G_SetupAnimatronics()
    G_SetupRooms()

    -- Precache Sounds
    G_PrecacheSounds()

    Sound_Play(1, "assets/sounds/misc/power_out", false, 1, 100)

    -- Play Phone Call
    G_PlayCall()
end

--[[
//
// G_GameLoop()
// Called every Frame
//
]]
function G_GameLoop()
    if Input_ButtonDown(INPUT_SELECT) then
        CoolPrint("nice select press bro")
    end

    if Input_KeyDown(INPUT_KEY_F) then
        CoolPrint("respects have been paid, bro")
    end
end