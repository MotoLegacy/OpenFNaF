// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

#include "types.h"
#include "input.h"
#include <stdio.h>
#include <GL/freeglut.h> 

void Window_Initialize(int width, int height, char* title, int argc, char* argv[]) {
    // Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE );
    glEnable(GL_MULTISAMPLE);

    // 1280x720 for FNaF 1, 1024x768 for 2/3
    // TODO: Convert Assets to allow for GLOBAL 720p
    glutInitWindowSize(width, height);

    // TODO: Position new Window in center of the Display
    glutInitWindowPosition(100, 100);

    glutCreateWindow(title);

    // Set up keyboard input through GLUT
    glutSpecialFunc(Input_SpecialKeyHandler);
    glutKeyboardFunc(Input_GenericKeyHandler);
}

void Window_Close() {
    glutDestroyWindow(0);
}

void Window_Update() {
    glutMainLoopEvent();
}