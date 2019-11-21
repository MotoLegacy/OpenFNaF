//
// main.cpp
///////////////////////////////////////
// changes:
// ~ moved switch for cleanliness purposes.
// ~ types.hpp.
// -xaa 11/17/19
//

//
//includes
//

#include "game.hpp"
#include "menu.hpp"
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>

int main() 
{   
    vita2d_init();
    bool quit = false;

    while (!quit) {
        vita2d_start_drawing();
        vita2d_clear_screen();

          D_GameLoop();

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}