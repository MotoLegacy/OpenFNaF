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