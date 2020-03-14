#include "game.h"
#include "types.h"

#include <GL/freeglut.h>
#include <stdio.h>

void Input_SpecialKeyHandler(int key, int x, int y) {
    //if (key == GLUT_KEY_DOWN)
}

void Input_GenericKeyHandler(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            Game_Running = FALSE;
            break;
    }
}