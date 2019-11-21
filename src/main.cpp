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
        switch(gameState) {
            case 0:
                doMenu();
                break;
            case 1:
                doGame();
                break;
            case 2:
                quit = true;
                break;
        }
        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}