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

    if OF_GetPlatform() == "PSP" then
        Sound = Sound .. ".mp3"
    elseif OF_GetPlatform() == "PC" then
        Sound = Sound .. ".ogg"
    end

    OF_StreamSound(0, Sound, false, 1, 100)
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

    -- Init Animas and Rooms
    G_SetupAnimatronics()
    G_SetupRooms()

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