#include "game.h"
#include "types.h"

#include <GL/freeglut.h>
#include <stdio.h>

// 'Special' Keys (Arrows, SHIFT, FX, etc.)
void Input_SpecialKeyHandler(int key, int x, int y) {
    //if (key == GLUT_KEY_DOWN)
}

// 'Normal' Keys
void Input_GenericKeyHandler(unsigned char key, int x, int y) {
    switch(key) {
        case 27: // Kill on Escape Key press
            Game_Running = FALSE;
            break;
    }
}

// Get Mouse Movement, also handle room scrolling
void Input_MousePassiveHandler(int x, int y) {
    //printf("MOUSE X: %d\n", x);
    //printf("MOUSE Y: %d\n", y);
}

// Left/Right Click
void Input_MouseClickHandler(int button, int state, int x, int y) {
    /*if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        printf("Left Click\n");
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        printf("Right Click\n");
    }*/
}