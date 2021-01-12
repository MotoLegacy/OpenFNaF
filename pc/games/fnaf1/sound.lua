--[[
=============
  sound.lua
=============
OpenFNaF Sound wrapper in LUA
]]

function Sound_Precache(Sound)
    if OF_GetPlatform() == "PSP" then
        Sound = Sound .. ".wav"
    elseif OF_GetPlatform() == "PC" then
        Sound = Sound .. ".ogg"
    end

    OF_PrecacheSound(Sound)
end

function Sound_Play(Channel, Sound, Loop, Pitch, Volume)
    if OF_GetPlatform() == "PSP" then
        Sound = Sound .. ".wav"
    elseif OF_GetPlatform() == "PC" then
        Sound = Sound .. ".ogg"
    end

    OF_PlaySound(Channel, Sound, Loop, Pitch, Volume)
end

function Sound_Stream(Channel, Sound, Loop, Pitch, Volume)
    if OF_GetPlatform() == "PSP" then
        Sound = Sound .. ".mp3"
    elseif OF_GetPlatform() == "PC" then
        Sound = Sound .. ".ogg"
    end

    OF_StreamSound(Channel, Sound, Loop, Pitch, Volume)
end