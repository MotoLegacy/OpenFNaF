#include "game.hpp"
#include "menu.hpp"
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"
#include <vita2d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include "types.hpp"

//////////////////////////////////////////
// replace all this shit with types.hpp //
//////////////////////////////////////////

int gameState;
int gState;
int loadState = 0;
int gameInit = 0;
int introTicker = 0;
int hourTicker = 0;
int GM_HOUR;
int GM_NIGHT;
int GM_BEAR;
int GM_BUNNY;
int GM_BIRD;
int GM_FOX;

int fanTicker = 1;
int flickTicker = 0;
int flickDur = 0;
int officeX = -124;

bool powerL = false;
bool powerLFlic = false;
bool powerR = false;
bool powerRFlic = false;
bool pressC = false;
bool pressT = false;

const char *vo;

bool fredScare = false;
int scareTicker = 0;

int staticTicker = 0;

vita2d_font *gfont;
vita2d_texture *clock;
vita2d_texture *office_on;
vita2d_texture *office_onL;
vita2d_texture *office_onR;
vita2d_texture *fan1;
vita2d_texture *fan2;
vita2d_texture *fan3;
vita2d_texture *ldoor1;
vita2d_texture *ldoor2;
vita2d_texture *ldoor3;
vita2d_texture *ldoor4;
vita2d_texture *ldoor5;
vita2d_texture *ldoor6;
vita2d_texture *ldoor7;
vita2d_texture *ldoor8;
vita2d_texture *ldoor9;
vita2d_texture *ldoor10;
vita2d_texture *ldoor11;
vita2d_texture *ldoor12;
vita2d_texture *ldoor13;
vita2d_texture *rdoor1;
vita2d_texture *rdoor2;
vita2d_texture *rdoor3;
vita2d_texture *rdoor4;
vita2d_texture *rdoor5;
vita2d_texture *rdoor6;
vita2d_texture *rdoor7;
vita2d_texture *rdoor8;
vita2d_texture *rdoor9;
vita2d_texture *rdoor10;
vita2d_texture *rdoor11;
vita2d_texture *rdoor12;
vita2d_texture *rdoor13;

vita2d_texture *staticc0;
vita2d_texture *staticc1;
vita2d_texture *staticc2;
vita2d_texture *staticc3;
vita2d_texture *staticc4;
vita2d_texture *staticc5;
vita2d_texture *staticc6;
vita2d_texture *staticc7;

vita2d_texture *fredscare1;
vita2d_texture *fredscare2;
vita2d_texture *fredscare3;
vita2d_texture *fredscare4;
vita2d_texture *fredscare5;
vita2d_texture *fredscare6;
vita2d_texture *fredscare7;
vita2d_texture *fredscare8;
vita2d_texture *fredscare9;
vita2d_texture *fredscare10;
vita2d_texture *fredscare11;
vita2d_texture *fredscare12;
vita2d_texture *fredscare13;
vita2d_texture *fredscare14;
vita2d_texture *fredscare15;
vita2d_texture *fredscare16;
vita2d_texture *fredscare17;
vita2d_texture *fredscare18;
vita2d_texture *fredscare19;
vita2d_texture *fredscare20;
vita2d_texture *fredscare21;
vita2d_texture *fredscare22;
vita2d_texture *fredscare23;
vita2d_texture *fredscare24;
vita2d_texture *fredscare25;
vita2d_texture *fredscare26;
vita2d_texture *fredscare27;
vita2d_texture *fredscare28;
vita2d_texture *fredscare29;
vita2d_texture *fredscare30;

/////////////////
// end replace //
/////////////////

SoLoud::Soloud ggSoloud; // SoLoud engine
SoLoud::WavStream blip;
SoLoud::WavStream fanbuzz;
SoLoud::WavStream voiceover;
SoLoud::WavStream lightbuzz;
SoLoud::WavStream doorshut;
SoLoud::WavStream xscream;
SoLoud::WavStream staticcc3;

gamestate_t Game;
vswap_t     Graphics;

//
// D_GameLoop(void);
////////////////////////////////////////////
// handles main game loop, from main.cpp.
// -xaa 11/17/19
//

void D_GameLoop(void) {
          switch(Game.State) {
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
}

void gameIntro() {
    const char* nite;

    switch(GM_NIGHT) {
        case 1:
            nite = "1st Night";
            vo = "app0:/sounds/voiceover1.wav";
            break;
        case 2:
            nite = "2nd Night";
            vo = "app0:/sounds/voiceover2.wav";
            break;
        case 3:
            nite = "3rd Night";
            vo = "app0:/sounds/voiceover3.wav";
            break;
        case 4:
            nite = "4th Night";
            vo = "app0:/sounds/voiceover4.wav";
            break;
        case 5:
            nite = "5th Night";
            vo = "app0:/sounds/voiceover5.wav";
            break;
        case 6:
            nite = "6th Night";
            break;
        case 7:
            nite = "7th Night";
            break;
    }

    vita2d_font_draw_text(gfont,420, 200, RGBA8(255,255,255,255), 25.0f, "12:00 AM");
    vita2d_font_draw_text(gfont,410, 230, RGBA8(255,255,255,255), 25.0f, nite);

    introTicker++;

    if (introTicker >= 180) {
        gState = 1;
    }
}

void unloadGame() {
    vita2d_free_font(gfont);
    vita2d_free_texture(fredscare1);
    vita2d_free_texture(fredscare2);
    vita2d_free_texture(fredscare3);
    vita2d_free_texture(fredscare4);
    vita2d_free_texture(fredscare5);
    vita2d_free_texture(fredscare6);
    vita2d_free_texture(fredscare7);
    vita2d_free_texture(fredscare8);
    vita2d_free_texture(fredscare9);
    vita2d_free_texture(fredscare10);
    vita2d_free_texture(fredscare11);
    vita2d_free_texture(fredscare12);
    vita2d_free_texture(fredscare13);
    vita2d_free_texture(fredscare14);
    vita2d_free_texture(fredscare15);
    vita2d_free_texture(fredscare16);
    vita2d_free_texture(fredscare17);
    vita2d_free_texture(fredscare18);
    vita2d_free_texture(fredscare19);
    vita2d_free_texture(fredscare20);
    vita2d_free_texture(fredscare21);
    vita2d_free_texture(fredscare22);
    vita2d_free_texture(fredscare23);
    vita2d_free_texture(fredscare24);
    vita2d_free_texture(fredscare25);
    vita2d_free_texture(fredscare26);
    vita2d_free_texture(fredscare27);
    vita2d_free_texture(fredscare28);
    vita2d_free_texture(fredscare29);
    //vita2d_free_texture(fredscare30);
    ggSoloud.deinit(); // Clean up!
}

void loadAssets() {
    office_on = vita2d_load_PNG_file("app0:/images/office_on.png");
    office_onL = vita2d_load_PNG_file("app0:/images/office_onL.png");
    office_onR = vita2d_load_PNG_file("app0:/images/office_onR.png");
    fan1 = vita2d_load_PNG_file("app0:/images/fan1.png");
    fan2 = vita2d_load_PNG_file("app0:/images/fan2.png");
    fan3 = vita2d_load_PNG_file("app0:/images/fan3.png");
    ldoor1 = vita2d_load_PNG_file("app0:/images/ldoor1.png");
    ldoor2 = vita2d_load_PNG_file("app0:/images/ldoor2.png");
    ldoor3 = vita2d_load_PNG_file("app0:/images/ldoor3.png");
    ldoor4 = vita2d_load_PNG_file("app0:/images/ldoor4.png");
    ldoor5 = vita2d_load_PNG_file("app0:/images/ldoor5.png");
    ldoor6 = vita2d_load_PNG_file("app0:/images/ldoor6.png");
    ldoor7 = vita2d_load_PNG_file("app0:/images/ldoor7.png");
    ldoor8 = vita2d_load_PNG_file("app0:/images/ldoor8.png");
    ldoor9 = vita2d_load_PNG_file("app0:/images/ldoor9.png");
    ldoor10 = vita2d_load_PNG_file("app0:/images/ldoor10.png");
    ldoor11 = vita2d_load_PNG_file("app0:/images/ldoor11.png");
    ldoor12 = vita2d_load_PNG_file("app0:/images/ldoor12.png");
    ldoor13 = vita2d_load_PNG_file("app0:/images/ldoor13.png");
    rdoor1 = vita2d_load_PNG_file("app0:/images/rdoor1.png");
    rdoor2 = vita2d_load_PNG_file("app0:/images/rdoor2.png");
    rdoor3 = vita2d_load_PNG_file("app0:/images/rdoor3.png");
    rdoor4 = vita2d_load_PNG_file("app0:/images/rdoor4.png");
    rdoor5 = vita2d_load_PNG_file("app0:/images/rdoor5.png");
    rdoor6 = vita2d_load_PNG_file("app0:/images/rdoor6.png");
    rdoor7 = vita2d_load_PNG_file("app0:/images/rdoor7.png");
    rdoor8 = vita2d_load_PNG_file("app0:/images/rdoor8.png");
    rdoor9 = vita2d_load_PNG_file("app0:/images/rdoor9.png");
    rdoor10 = vita2d_load_PNG_file("app0:/images/rdoor10.png");
    rdoor11 = vita2d_load_PNG_file("app0:/images/rdoor11.png");
    rdoor12 = vita2d_load_PNG_file("app0:/images/rdoor12.png");
    rdoor13 = vita2d_load_PNG_file("app0:/images/rdoor13.png");
    fredscare1 = vita2d_load_PNG_file("app0:/images/fredscare1.png");
    fredscare2 = vita2d_load_PNG_file("app0:/images/fredscare2.png");
    fredscare3 = vita2d_load_PNG_file("app0:/images/fredscare3.png");
    fredscare4 = vita2d_load_PNG_file("app0:/images/fredscare4.png");
    fredscare5 = vita2d_load_PNG_file("app0:/images/fredscare5.png");
    fredscare6 = vita2d_load_PNG_file("app0:/images/fredscare6.png");
    fredscare7 = vita2d_load_PNG_file("app0:/images/fredscare7.png");
    fredscare8 = vita2d_load_PNG_file("app0:/images/fredscare8.png");
    fredscare9 = vita2d_load_PNG_file("app0:/images/fredscare9.png");
    fredscare10 = vita2d_load_PNG_file("app0:/images/fredscare10.png");
    fredscare11 = vita2d_load_PNG_file("app0:/images/fredscare11.png");
    fredscare12 = vita2d_load_PNG_file("app0:/images/fredscare12.png");
    fredscare13 = vita2d_load_PNG_file("app0:/images/fredscare13.png");
    fredscare14 = vita2d_load_PNG_file("app0:/images/fredscare14.png");
    fredscare15 = vita2d_load_PNG_file("app0:/images/fredscare15.png");
    fredscare16 = vita2d_load_PNG_file("app0:/images/fredscare16.png");
    fredscare17 = vita2d_load_PNG_file("app0:/images/fredscare17.png");
    fredscare18 = vita2d_load_PNG_file("app0:/images/fredscare18.png");
    fredscare19 = vita2d_load_PNG_file("app0:/images/fredscare19.png");
    fredscare20 = vita2d_load_PNG_file("app0:/images/fredscare20.png");
    fredscare21 = vita2d_load_PNG_file("app0:/images/fredscare21.png");
    fredscare22 = vita2d_load_PNG_file("app0:/images/fredscare22.png");
    fredscare23 = vita2d_load_PNG_file("app0:/images/fredscare23.png");
    fredscare24 = vita2d_load_PNG_file("app0:/images/fredscare24.png");
    fredscare25 = vita2d_load_PNG_file("app0:/images/fredscare25.png");
    fredscare26 = vita2d_load_PNG_file("app0:/images/fredscare26.png");
    fredscare27 = vita2d_load_PNG_file("app0:/images/fredscare27.png");
    fredscare28 = vita2d_load_PNG_file("app0:/images/fredscare28.png");
    fredscare29 = vita2d_load_PNG_file("app0:/images/fredscare29.png");
    fredscare30 = vita2d_load_PNG_file("app0:/images/fredscare30.png");
    staticc0 = vita2d_load_PNG_file("app0:/images/static0.png");
    staticc1 = vita2d_load_PNG_file("app0:/images/static1.png");
    staticc2 = vita2d_load_PNG_file("app0:/images/static2.png");
    staticc3 = vita2d_load_PNG_file("app0:/images/static3.png");
    staticc4 = vita2d_load_PNG_file("app0:/images/static4.png");
    staticc5 = vita2d_load_PNG_file("app0:/images/static5.png");
    staticc6 = vita2d_load_PNG_file("app0:/images/static6.png");
    staticc7 = vita2d_load_PNG_file("app0:/images/static7.png");

    fanbuzz.load("app0:/sounds/fanbuzz.wav");
    lightbuzz.load("app0:/sounds/lightbuzz.wav");
    doorshut.load("app0:/sounds/doorshut.wav");
    xscream.load("app0:/sounds/XSCREAM.wav");
    staticcc3.load("app0:/sounds/static2.wav");
    fanbuzz.setLooping(1);
    lightbuzz.setLooping(1);
    ggSoloud.play(fanbuzz);

    if (GM_NIGHT < 6) {
        voiceover.load(vo);
        ggSoloud.play(voiceover);
    }
}

void loadOffice() {
    vita2d_draw_texture_scale(clock, 920, 504, 1, 1);
    loadState++;

    if (loadState >= 10) {
        loadAssets();
        gState = 2;
    }
    
}

void gameClock() {
    const char *night;
    const char *hour;

    switch(GM_NIGHT) {
        case 1:
            night = "Night 1";
            break;
        case 2:
            night = "Night 2";
            break;
        case 3:
            night = "Night 3";
            break;
        case 4:
            night = "Night 4";
            break;
        case 5:
            night = "Night 5";
            break;
        case 6:
            night = "Night 6";
            break;
        case 7:
            night = "Night 7";
            break;
    }

    switch(GM_HOUR) {
        case 0:
            hour = "12 AM";
            break;
        case 1:
            hour = "1 AM";
            break;
        case 2:
            hour = "2 AM";
            break;
        case 3:
            hour = "3 AM";
            break;
        case 4:
            hour = "4 AM";
            break;
        case 5:
            hour = "5 AM";
            break;
        case 6:
            hour = "6 AM";
            break;
    }

    hourTicker++;

    if (hourTicker >= (60 * 86)) {
        hourTicker = 0;
        GM_HOUR++;

        if (GM_HOUR == 6) {
            gameState = 3;
        }
    }

    vita2d_font_draw_text(gfont, 850, 50, RGBA8(255, 255, 255, 255), 30.0f, hour);
    vita2d_font_draw_text(gfont, 855, 70, RGBA8(255, 255, 255, 255), 20.0f, night);
}

void scareFreddy1() {
    switch(scareTicker) {
        case 0:
        case 1:
            vita2d_draw_texture_scale(fredscare1, officeX, 0, 1, 1);
            break;
        case 2:
        case 3:
            vita2d_draw_texture_scale(fredscare2, officeX, 0, 1, 1);
            break;
        case 4:
        case 5:
            vita2d_draw_texture_scale(fredscare3, officeX, 0, 1, 1);
            break;
        case 6:
        case 7:
            vita2d_draw_texture_scale(fredscare4, officeX, 0, 1, 1);
            break;
        case 8:
        case 9:
            vita2d_draw_texture_scale(fredscare5, officeX, 0, 1, 1);
            break;
        case 10:
        case 11:
            vita2d_draw_texture_scale(fredscare6, officeX, 0, 1, 1);
            break;
        case 12:
        case 13:
            vita2d_draw_texture_scale(fredscare7, officeX, 0, 1, 1);
            break;
        case 14:
        case 15:
            vita2d_draw_texture_scale(fredscare8, officeX, 0, 1, 1);
            break;
        case 16:
        case 17:
            vita2d_draw_texture_scale(fredscare9, officeX, 0, 1, 1);
            break;
        case 18:
        case 19:
            vita2d_draw_texture_scale(fredscare10, officeX, 0, 1, 1);
            break;
        case 20:
        case 21:
            vita2d_draw_texture_scale(fredscare11, officeX, 0, 1, 1);
            break;
        case 22:
        case 23:
            vita2d_draw_texture_scale(fredscare12, officeX, 0, 1, 1);
            break;
        case 24:
        case 25:
            vita2d_draw_texture_scale(fredscare13, officeX, 0, 1, 1);
            break;
        case 26:
        case 27:
            vita2d_draw_texture_scale(fredscare14, officeX, 0, 1, 1);
            break;
        case 28:
        case 29:
            vita2d_draw_texture_scale(fredscare15, officeX, 0, 1, 1);
            break;
        case 30:
        case 31:
            vita2d_draw_texture_scale(fredscare16, officeX, 0, 1, 1);
            break;
        case 32:
        case 33:
            vita2d_draw_texture_scale(fredscare17, officeX, 0, 1, 1);
            break;
        case 34:
        case 35:
            vita2d_draw_texture_scale(fredscare18, officeX, 0, 1, 1);
            break;
        case 36:
        case 37:
            vita2d_draw_texture_scale(fredscare19, officeX, 0, 1, 1);
            break;
        case 38:
        case 39:
            vita2d_draw_texture_scale(fredscare20, officeX, 0, 1, 1);
            break;
        case 40:
        case 41:
            vita2d_draw_texture_scale(fredscare21, officeX, 0, 1, 1);
            break;
        case 42:
        case 43:
            vita2d_draw_texture_scale(fredscare22, officeX, 0, 1, 1);
            break;
        case 44:
        case 45:
            vita2d_draw_texture_scale(fredscare23, officeX, 0, 1, 1);
            break;
        case 46:
        case 47:
            vita2d_draw_texture_scale(fredscare24, officeX, 0, 1, 1);
            break;
        case 48:
        case 49:
            vita2d_draw_texture_scale(fredscare25, officeX, 0, 1, 1);
            break;
        case 50:
        case 51:
            vita2d_draw_texture_scale(fredscare26, officeX, 0, 1, 1);
            break;
        case 52:
        case 53:
            vita2d_draw_texture_scale(fredscare27, officeX, 0, 1, 1);
            break;
        case 54:
        case 55:
            vita2d_draw_texture_scale(fredscare28, officeX, 0, 1, 1);
            break;
        case 56:
        case 57:
            vita2d_draw_texture_scale(fredscare29, officeX, 0, 1, 1);
            break;
        case 58:
        case 59:
            vita2d_draw_texture_scale(fredscare30, officeX, 0, 1, 1);
            break;
    }
    scareTicker++;

    gameClock();

    if (scareTicker >= 60) {
        xscream.stop();
        ggSoloud.play(staticcc3);
        //gameState = 3;
    }
}

// free unneeded shit for jumpscares
void freeScare() {
    vita2d_free_texture(office_on);
    vita2d_free_texture(office_onL);
    vita2d_free_texture(office_onR);
    vita2d_free_texture(fan1);
    vita2d_free_texture(fan2);
    vita2d_free_texture(fan3);
    vita2d_free_texture(ldoor1);
    vita2d_free_texture(ldoor2);
    vita2d_free_texture(ldoor3);
    vita2d_free_texture(ldoor4);
    vita2d_free_texture(ldoor5);
    vita2d_free_texture(ldoor6);
    vita2d_free_texture(ldoor7);
    vita2d_free_texture(ldoor8);
    vita2d_free_texture(ldoor9);
    vita2d_free_texture(ldoor10);
    vita2d_free_texture(ldoor11);
    vita2d_free_texture(ldoor12);
    vita2d_free_texture(ldoor13);
    vita2d_free_texture(rdoor1);
    vita2d_free_texture(rdoor2);
    vita2d_free_texture(rdoor3);
    vita2d_free_texture(rdoor4);
    vita2d_free_texture(rdoor5);
    vita2d_free_texture(rdoor6);
    vita2d_free_texture(rdoor7);
    vita2d_free_texture(rdoor8);
    vita2d_free_texture(rdoor9);
    vita2d_free_texture(rdoor10);
    vita2d_free_texture(rdoor11);
    vita2d_free_texture(rdoor12);
    vita2d_free_texture(rdoor13);
}

void gameStuff() {
    SceCtrlData pad;
    memset(&pad, 0, sizeof(pad));
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    sceCtrlPeekBufferPositive(0, &pad, 1);

    if (fredScare) {
        scareFreddy1();
        return;
    }

    int powerFlic = rand() % 14;

    if (powerFlic == 8 && !flickTicker) {
        flickTicker = 1;
        flickDur = rand() % 7 + 3;

        if (powerL) {
            lightbuzz.stop();
            powerLFlic = true;
            powerL = false;
        } else if (powerR) {
            lightbuzz.stop();
            powerRFlic = true;
            powerR = false;
        }
    }

    if (flickTicker != 0) {
        flickTicker++;

        if (flickTicker >= flickDur) {
            flickTicker = 0;
            flickDur = 0;

            if (powerLFlic) {
                powerLFlic = false;

                if (!powerR && !powerRFlic) {
                    ggSoloud.play(lightbuzz);
                    powerL = true;
                }
            } else if (powerRFlic) {
                powerRFlic = false;

                if (!powerL && !powerLFlic) {
                    ggSoloud.play(lightbuzz);
                    powerR = true;
                }
            }
        }
    } 

    //LDOOR: 53, 0

    if (powerL) {
        vita2d_draw_texture_scale(office_onL, officeX, 0, 1, 1);
    } else if (powerR) {
        vita2d_draw_texture_scale(office_onR, officeX, 0, 1, 1);
    } else {
        vita2d_draw_texture_scale(office_on, officeX, 0, 1, 1);
    }

    if (doorGoDownL) {
        doorLTicker++;
    } else if (doorGoUpL) {
        doorLTicker--;
    }

    switch(doorLTicker) {
        case 1:
        case 2:
            if (doorGoUpL && doorLTicker == 1) {
                doorLTicker = 0;
                doorGoUpL = false;
                doorDownL = false;
            }
            vita2d_draw_texture_scale(ldoor1, 53 + officeX, 0, 1, 1);
            break;
        case 3:
        case 4:
            vita2d_draw_texture_scale(ldoor2, 53 + officeX, 0, 1, 1);
            break;
        case 5:
        case 6:
            vita2d_draw_texture_scale(ldoor3, 53 + officeX, 0, 1, 1);
            break;
        case 7:
        case 8:
            vita2d_draw_texture_scale(ldoor4, 53 + officeX, 0, 1, 1);
            break;
        case 9:
        case 10:
            vita2d_draw_texture_scale(ldoor5, 53 + officeX, 0, 1, 1);
            break;
        case 11:
        case 12:
            vita2d_draw_texture_scale(ldoor6, 53 + officeX, 0, 1, 1);
            break;
        case 13:
        case 14:
            vita2d_draw_texture_scale(ldoor7, 53 + officeX, 0, 1, 1);
            break;
        case 15:
        case 16:
            vita2d_draw_texture_scale(ldoor8, 53 + officeX, 0, 1, 1);
            break;
        case 17:
        case 18:
            vita2d_draw_texture_scale(ldoor9, 53 + officeX, 0, 1, 1);
            break;
        case 19:
        case 20:
            vita2d_draw_texture_scale(ldoor10, 53 + officeX, 0, 1, 1);
            break;
        case 21:
        case 22:
            vita2d_draw_texture_scale(ldoor11, 53 + officeX, 0, 1, 1);
            break;
        case 23:
        case 24:
            vita2d_draw_texture_scale(ldoor12, 53 + officeX, 0, 1, 1);
            break;
        case 25:
        case 26:
            if (doorGoDownL && doorLTicker == 26) {
                doorGoDownL = false;
                doorDownL = true;
                doorLTicker = 0;
            }
            vita2d_draw_texture_scale(ldoor13, 53 + officeX, 0, 1, 1);
            break;
    }

    if (doorDownL && !doorLTicker) {
        vita2d_draw_texture_scale(ldoor13, 53 + officeX, 0, 1, 1);
    }

    if (doorGoDownR) {
        doorRTicker++;
    } else if (doorGoUpR) {
        doorRTicker--;
    }

    switch(doorRTicker) {
        case 1:
        case 2:
            if (doorGoUpR && doorRTicker == 1) {
                doorRTicker = 0;
                doorGoUpR = false;
                doorDownR = false;
            }
            vita2d_draw_texture_scale(rdoor1, 961 + officeX, 0, 1, 1);
            break;
        case 3:
        case 4:
            vita2d_draw_texture_scale(rdoor2, 961 + officeX, 0, 1, 1);
            break;
        case 5:
        case 6:
            vita2d_draw_texture_scale(rdoor3, 961 + officeX, 0, 1, 1);
            break;
        case 7:
        case 8:
            vita2d_draw_texture_scale(rdoor4, 961 + officeX, 0, 1, 1);
            break;
        case 9:
        case 10:
            vita2d_draw_texture_scale(rdoor5, 961 + officeX, 0, 1, 1);
            break;
        case 11:
        case 12:
            vita2d_draw_texture_scale(rdoor6, 961 + officeX, 0, 1, 1);
            break;
        case 13:
        case 14:
            vita2d_draw_texture_scale(rdoor7, 961 + officeX, 0, 1, 1);
            break;
        case 15:
        case 16:
            vita2d_draw_texture_scale(rdoor8, 961 + officeX, 0, 1, 1);
            break;
        case 17:
        case 18:
            vita2d_draw_texture_scale(rdoor9, 961 + officeX, 0, 1, 1);
            break;
        case 19:
        case 20:
            vita2d_draw_texture_scale(rdoor10, 961 + officeX, 0, 1, 1);
            break;
        case 21:
        case 22:
            vita2d_draw_texture_scale(rdoor11, 961 + officeX, 0, 1, 1);
            break;
        case 23:
        case 24:
            vita2d_draw_texture_scale(rdoor12, 961 + officeX, 0, 1, 1);
            break;
        case 25:
        case 26:
            if (doorGoDownR && doorRTicker == 26) {
                doorGoDownR = false;
                doorDownR = true;
                doorRTicker = 0;
            }
            vita2d_draw_texture_scale(rdoor13, 961 + officeX, 0, 1, 1);
            break;
    }

    if (doorDownR && !doorRTicker) {
        vita2d_draw_texture_scale(rdoor13, 961 + officeX, 0, 1, 1);
    }

    switch(fanTicker) {
        case 1:
        case 2:
            vita2d_draw_texture_scale(fan1, 589 + officeX, 229, 1, 1);
            break;
        case 3:
        case 4:
            vita2d_draw_texture_scale(fan2, 589 + officeX, 229, 1, 1);
            break;
        case 5:
        case 6:
            vita2d_draw_texture_scale(fan3, 589 + officeX, 229, 1, 1);
            break;
    }

    fanTicker++;

    if (fanTicker > 6) {
        fanTicker = 1;
    }

    if (pad.lx < 77.5) {
        officeX += 10;

        if (officeX > 0)
            officeX = 0;
    }

    if (pad.lx > 177.5) {
        officeX -= 10;

        if (officeX < -249)
            officeX = -249;
    }

    gameClock();

    if (pad.buttons & SCE_CTRL_CROSS && !pressC) {
        if (officeX > -124) {
            if (powerR || powerRFlic) {
                powerRFlic = false;
                powerR = false;
                lightbuzz.stop();
            }
            
            if (powerL) {
                powerL = false;
                powerLFlic = false;
                lightbuzz.stop();
            } else {
                powerL = true;
                ggSoloud.play(lightbuzz);
            }
        } else {
            if (powerL || powerLFlic) {
                powerL = false;
                powerLFlic = false;
                lightbuzz.stop();
            }

            if (powerR) {
                powerRFlic = false;
                powerR = false;
                lightbuzz.stop();
            } else {
                powerR = true;
                ggSoloud.play(lightbuzz);
            }
        }

        pressC = true;
    } else if (!(pad.buttons & SCE_CTRL_CROSS)) {
        pressC = false;
    }

    if (pad.buttons & SCE_CTRL_TRIANGLE && !pressT) {
        if (officeX > -124) {
            if (!doorDownL && !doorGoDownL) {
                doorGoDownL = true;
                ggSoloud.play(doorshut);
            } else if (doorDownL && !doorGoUpL) {
                ggSoloud.play(doorshut);
                doorGoUpL = true;
                doorLTicker = 25;
            }
        } else {
            if (!doorDownR && !doorGoDownR) {
                doorGoDownR = true;
                ggSoloud.play(doorshut);
            } else if (doorDownR && !doorGoUpR) {
                ggSoloud.play(doorshut);
                doorGoUpR = true;
                doorRTicker = 25;
            }
        }
        pressT = true;
    } else if (!(pad.buttons & SCE_CTRL_TRIANGLE)) {
        pressT = false;
    }

    if (pad.buttons & SCE_CTRL_LEFT && !fredScare) {
        lightbuzz.stop();
        voiceover.stop();
        fanbuzz.stop();
        ggSoloud.play(xscream);
        freeScare();
        fredScare = true;
    }
}

void staticfx() {
    int shiz = rand() % 7; 

    switch(shiz) {
        case 0:
            vita2d_draw_texture_tint(staticc0, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 1:
            vita2d_draw_texture_tint(staticc1, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 2:
            vita2d_draw_texture_tint(staticc2, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 3:
            vita2d_draw_texture_tint(staticc3, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 4:
            vita2d_draw_texture_tint(staticc4, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 5:
            vita2d_draw_texture_tint(staticc5, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 6:
            vita2d_draw_texture_tint(staticc6, 0, 0, RGBA8(255, 255, 255, 255));
            break;
        case 7:
            vita2d_draw_texture_tint(staticc7, 0, 0, RGBA8(255, 255, 255, 255));
            break;
    }

    staticTicker++;

    if (staticTicker >= 600) {
        staticcc3.stop();
        gameState = 4;
    }
}

void endGame() {
    unloadGame();
    gState = 0;
    gameState = 0;
    restart();
}

void doGame() {
    if (!gameInit) {
        gfont = vita2d_load_font_file("app0:/font/FNaF.ttf");
        clock = vita2d_load_PNG_file("app0:/images/clock.png");
        ggSoloud.init(); // Initialize SoLoud
        blip.load("app0:/sounds/blip.wav");
        ggSoloud.play(blip);
        gameInit = 1;
    }
    switch(gState) {
        case 0:
            gameIntro();
            break;
        case 1:
            loadOffice();
            break;
        case 2:
            gameStuff();
            break;
        case 4:
            staticfx();
        case 3:
            endGame();
            break;
    }
}