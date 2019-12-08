#include "menu.hpp"
#include "game.hpp"
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"
#include <vita2d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

int menuInit = 0;
int paperTicker = 0;
int splashTicker = 0;
int eggTicker = 0;
int menuState = 0;
int rectY = -50;
int bearLevel = 1;
int bunnyLevel = 3;
int birdLevel = 3;
int foxLevel = 1;
int selected = 0;
int oldS = 0;
int fadeOutTicker = 0;
bool moveL = false;
bool moveR = false;
bool moveU = false;
bool moveD = false;
bool moveU2 = false;
bool moveD2 = false;
int specialFredTicker = 0;
vita2d_font *font;
vita2d_texture *menu0;
vita2d_texture *menu1;
vita2d_texture *menu2;
vita2d_texture *menu3;
vita2d_texture *specialHead;
vita2d_texture *static0;
vita2d_texture *static1;
vita2d_texture *static2;
vita2d_texture *static3;
vita2d_texture *static4;
vita2d_texture *static5;
vita2d_texture *static6;
vita2d_texture *static7;
vita2d_texture *bear;
vita2d_texture *bunny;
vita2d_texture *bird;
vita2d_texture *fox;
vita2d_texture *gfreddy;
vita2d_texture *paper;
SoLoud::Soloud gSoloud; // SoLoud engine
SoLoud::WavStream darkness;
SoLoud::WavStream staticcc2;
SoLoud::WavStream xscream2;

void menuSplash() {
    splashTicker++;

    vita2d_font_draw_text(font,420, 200, RGBA8(255,255,255,255), 25.0f, "WARNING!");
    vita2d_font_draw_text(font,150, 300, RGBA8(255,255,255,255), 25.0f, "This game contains flashing lights, loud");
    vita2d_font_draw_text(font,220, 330, RGBA8(255,255,255,255), 25.0f, "noises, and lots of jumpscares!");

    if (splashTicker >= 300) {
        menuState = 1;
        darkness.setLooping(1);
        gSoloud.play(darkness);
        gSoloud.play(staticcc2);
    }
}

void menuMain() {   
    SceCtrlData pad;
    memset(&pad, 0, sizeof(pad));
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    sceCtrlPeekBufferPositive(0, &pad, 1);

    int shiz = rand() % 7; 
    int fredhead = rand() % 119; 

    if (specialFredTicker != 0) {
        specialFredTicker++;
        vita2d_draw_texture_scale(specialHead, 0, 0, 1, 1);  

        if (specialFredTicker >= 8) {
            specialFredTicker = 0;
        }
    } else {
        if (fredhead == 39) {
            specialHead = menu1;
            specialFredTicker++;
            vita2d_draw_texture_scale(menu1, 0, 0, 1, 1);  
        } else if (fredhead == 79) {
            specialHead = menu2;
            specialFredTicker++;
            vita2d_draw_texture_scale(menu2, 0, 0, 1, 1);  
        } else if (fredhead == 119) {
            specialHead = menu3;
            specialFredTicker++;
            vita2d_draw_texture_scale(menu3, 0, 0, 1, 1);  
        } else {
            vita2d_draw_texture_scale(menu0, 0, 0, 1, 1);  
        }
    }
    

    vita2d_draw_rectangle(0, rectY, 960, 25, RGBA8(255, 255, 255, 32));

    switch(shiz) {
        case 0:
            vita2d_draw_texture_tint(static0, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 1:
            vita2d_draw_texture_tint(static1, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 2:
            vita2d_draw_texture_tint(static2, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 3:
            vita2d_draw_texture_tint(static3, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 4:
            vita2d_draw_texture_tint(static4, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 5:
            vita2d_draw_texture_tint(static5, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 6:
            vita2d_draw_texture_tint(static6, 0, 0, RGBA8(255, 255, 255, 128));
            break;
        case 7:
            vita2d_draw_texture_tint(static7, 0, 0, RGBA8(255, 255, 255, 128));
            break;
    }

    vita2d_font_draw_text(font, 130, 90, RGBA8(255, 255, 255, 255), 25.0f, "Five");
    vita2d_font_draw_text(font, 130, 120, RGBA8(255, 255, 255, 255), 25.0f, "Nights");
    vita2d_font_draw_text(font, 130, 150, RGBA8(255, 255, 255, 255), 25.0f, "at");
    vita2d_font_draw_text(font, 130, 180, RGBA8(255, 255, 255, 255), 25.0f, "Freddy's");

    //vita2d_font_draw_text(font, 130, 330, RGBA8(255, 255, 255, 255), 25.0f, "Custom Night");
    vita2d_font_draw_text(font, 130, 330, RGBA8(255, 255, 255, 255), 25.0f, "New Game");
    vita2d_font_draw_text(font, 95, 330, RGBA8(255, 255, 255, 255), 25.0f, ">> ");

    rectY += 1;

    if (rectY >= 970) {
        rectY = -50;
    }

    if (pad.buttons & SCE_CTRL_CROSS) {
        menuState = 4; // new game
        //menuState = 2; // custom night
    }
}

void menuCustom() {
    SceCtrlData pad;
    memset(&pad, 0, sizeof(pad));
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    sceCtrlPeekBufferPositive(0, &pad, 1);

    vita2d_font_draw_text(font,350, 50, RGBA8(255,255,255,255), 25.0f, "Customize Night");

    vita2d_font_draw_text(font,84, 121, RGBA8(255,255,255,255), 25.0f, "Freddy");
    vita2d_font_draw_text(font,313, 121, RGBA8(255,255,255,255), 25.0f, "Bonnie");
    vita2d_font_draw_text(font,551, 121, RGBA8(255,255,255,255), 25.0f, "Chica");
    vita2d_font_draw_text(font,784, 121, RGBA8(255,255,255,255), 25.0f, "Foxy");

    vita2d_font_draw_textf(font,124, 415, RGBA8(255,255,255,255), 35.0f, "%d", bearLevel);
    vita2d_font_draw_textf(font,348, 415, RGBA8(255,255,255,255), 35.0f, "%d", bunnyLevel);
    vita2d_font_draw_textf(font,576, 415, RGBA8(255,255,255,255), 35.0f, "%d", birdLevel);
    vita2d_font_draw_textf(font,804, 415, RGBA8(255,255,255,255), 35.0f, "%d", foxLevel);

    vita2d_font_draw_text(font,62, 370, RGBA8(255,255,255,255), 25.0f, "A.I. Level");
    vita2d_font_draw_text(font,288, 370, RGBA8(255,255,255,255), 25.0f, "A.I. Level");
    vita2d_font_draw_text(font,515, 370, RGBA8(255,255,255,255), 25.0f, "A.I. Level");
    vita2d_font_draw_text(font,742, 370, RGBA8(255,255,255,255), 25.0f, "A.I. Level");

    if (selected == 1) {
        vita2d_font_draw_text(font,52, 415, RGBA8(255,255,255,255), 35.0f, "<        >");
    } else if (selected == 2) {
        vita2d_font_draw_text(font,278, 415, RGBA8(255,255,255,255), 35.0f, "<        >");
    } else if (selected == 3) {
        vita2d_font_draw_text(font,505, 415, RGBA8(255,255,255,255), 35.0f, "<        >");
    } else if (selected == 4) {
        vita2d_font_draw_text(font,732, 415, RGBA8(255,255,255,255), 35.0f, "<        >");
    }

    if (selected == 5) {
        vita2d_draw_rectangle(800, 513, 108, 5, RGBA8(255, 255, 255, 255));
    }

    vita2d_font_draw_text(font,52, 520, RGBA8(255,255,255,255), 15.0f, "(0-2) Easy    (3-6) Med    (7-12) Hard    (13-20) Extreme");
    vita2d_font_draw_text(font,800, 510, RGBA8(255,255,255,255), 30.0f, "READY");

    vita2d_draw_texture_scale(bear, 52, 150, 1, 1);
    vita2d_draw_texture_scale(bunny, 278, 150, 1, 1);
    vita2d_draw_texture_scale(bird, 505, 150, 1, 1);
    vita2d_draw_texture_scale(fox, 732, 150, 1, 1);

    if (pad.buttons & SCE_CTRL_CROSS) {
        if (selected != 5) 
            return;

        if (bearLevel == 1 && bunnyLevel == 9 && birdLevel == 8 && foxLevel == 7) {
            staticcc2.stop();
            darkness.stop();
            gSoloud.play(xscream2);
            menuState = 3;
        }
    }

    if (pad.buttons & SCE_CTRL_DOWN) {
        if (selected == 5)
            return;

        oldS = selected;
        selected = 5;
    }

    if (pad.buttons & SCE_CTRL_UP) {
        if (selected != 5)
            return;

        selected = oldS;

        if (selected == 0)
            selected = 1;
    }

    if (pad.buttons & SCE_CTRL_LTRIGGER && !moveL) {
        if (selected == 5)
            return;
        
        selected--;

        if (selected == 0)
            selected = 4;
        
        moveL = true;
    } else if (!(pad.buttons & SCE_CTRL_LTRIGGER)) {
        moveL = false;
    }

    if (pad.buttons & SCE_CTRL_RTRIGGER && !moveR) {
        if (selected == 5)
            return;
        
        selected++;

        if (selected == 5)
            selected = 1;

        moveR = true;
    } else if (!(pad.buttons & SCE_CTRL_RTRIGGER)) {
        moveR = false;
    }

    if (pad.buttons & SCE_CTRL_LEFT && !moveD) {
        switch (selected) {
            case 1:
                bearLevel--;
                if (bearLevel <= -1) {
                    bearLevel = 20;
                }
                break;
            case 2:
                bunnyLevel--;
                if (bunnyLevel <= -1) {
                    bunnyLevel = 20;
                }
                break;
            case 3:
                birdLevel--;
                if (birdLevel <= -1) {
                    birdLevel = 20;
                }
                break;
            case 4:
                foxLevel--;
                if (foxLevel <= -1) {
                    foxLevel = 20;
                }
                break;
        }

        moveD = true;
    } else if (!(pad.buttons & SCE_CTRL_LEFT)) {
        moveD = false;
    }

    if (pad.buttons & SCE_CTRL_RIGHT && !moveU) {
        switch (selected) {
            case 1:
                bearLevel++;
                if (bearLevel > 20) {
                    bearLevel = 0;
                }
                break;
            case 2:
                bunnyLevel++;
                if (bunnyLevel > 20) {
                    bunnyLevel = 0;
                }
                break;
            case 3:
                birdLevel++;
                if (birdLevel > 20) {
                    birdLevel = 0;
                }
                break;
            case 4:
                foxLevel++;
                if (foxLevel > 20) {
                    foxLevel = 0;
                }
                break;
        }

        moveU = true;
    } else if (!(pad.buttons & SCE_CTRL_RIGHT)) {
        moveU = false;
    }
}

void egg1987() {
    vita2d_draw_texture_scale(gfreddy, 0, 0, 1, 1);

    eggTicker++;

    if (eggTicker >= 150) {
        vita2d_fini();
        sceKernelExitProcess(0); 
    }
}

void cleanMenu() {
    vita2d_free_font(font);
    vita2d_free_texture(menu0);
    vita2d_free_texture(menu1);
    vita2d_free_texture(menu2);
    vita2d_free_texture(menu3);
    vita2d_free_texture(static0);
    vita2d_free_texture(static1);
    vita2d_free_texture(static2);
    vita2d_free_texture(static3);
    vita2d_free_texture(static4);
    vita2d_free_texture(static5);
    vita2d_free_texture(static6);
    vita2d_free_texture(static7);
    vita2d_free_texture(bear);
    vita2d_free_texture(bunny);
    vita2d_free_texture(bird);
    vita2d_free_texture(fox);
    //vita2d_free_texture(paper);

    gSoloud.deinit(); // Clean up!
}

void newGamePaper() {
    vita2d_draw_texture_scale(paper, 0, 0, 1, 1);
    paperTicker++;

    if (paperTicker >= 360) {
        staticcc2.stop();
        darkness.stop();
        GM_NIGHT = 1;
        GM_BEAR = 1;
        GM_BUNNY = 3;
        GM_BIRD = 3;
        GM_FOX = 1;
        gameState = 1;
        cleanMenu();
    }
}

void restart() {
    menuInit = 0;
    menuState = 1;
}

void doMenu() {
    if (!menuInit) {
        font = vita2d_load_font_file("app0:/font/FNaF.ttf");
        menu0 = vita2d_load_PNG_file("app0:/images/menu0.png");
        menu1 = vita2d_load_PNG_file("app0:/images/menu1.png");
        menu2 = vita2d_load_PNG_file("app0:/images/menu2.png");
        menu3 = vita2d_load_PNG_file("app0:/images/menu3.png");
        static0 = vita2d_load_PNG_file("app0:/images/static0.png");
        static1 = vita2d_load_PNG_file("app0:/images/static1.png");
        static2 = vita2d_load_PNG_file("app0:/images/static2.png");
        static3 = vita2d_load_PNG_file("app0:/images/static3.png");
        static4 = vita2d_load_PNG_file("app0:/images/static4.png");
        static5 = vita2d_load_PNG_file("app0:/images/static5.png");
        static6 = vita2d_load_PNG_file("app0:/images/static6.png");
        static7 = vita2d_load_PNG_file("app0:/images/static7.png");
        gfreddy = vita2d_load_PNG_file("app0:/images/gfreddy.png");
        bear = vita2d_load_PNG_file("app0:/images/bear.png");
        bunny = vita2d_load_PNG_file("app0:/images/bunny.png");
        bird = vita2d_load_PNG_file("app0:/images/bird.png");
        fox = vita2d_load_PNG_file("app0:/images/fox.png");
        paper = vita2d_load_PNG_file("app0:/images/paper.png");
        gSoloud.init(); // Initialize SoLoud
        darkness.load("app0:/sounds/menu.wav");
        staticcc2.load("app0:/sounds/static2.wav");
        xscream2.load("app0:/sounds/XSCREAM2.wav");
        menuInit = 1;
    }
    
    switch(menuState) {
        case 0:
            menuSplash();
            break;
        case 1:
            menuMain();
            break;
        case 2:
            menuCustom();
            break;
        case 3:
            egg1987();
            break;
        case 4:
            newGamePaper();
            break;
    }
}
