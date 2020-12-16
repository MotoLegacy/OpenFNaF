--[[
==============
   menu.lua
==============
The FNaF 1 menu
]]
-- Menu State
STATE_MENU_MAIN         = 0

Menu_State = -1

function Menu_Initialize()
    -- Stream the theme
    Sound_Stream(0, "sounds/menu/theme", true, 1, 100)

    -- We're in the menu
    Game_State = STATE_IN_MENU
    Menu_State = STATE_MENU_MAIN
end

--[[
//
// MAIN MENU
//
]]
function Menu_Main_Draw()
    --OF_DrawImage()
    --PSPDebugPrintf("main menu xddd")
end

-- Called every frame we're in the menu
function Menu_Loop()
    if Menu_State == STATE_MENU_MAIN then
        Menu_Main_Draw()
        Menu_Main_Key()
    end
end