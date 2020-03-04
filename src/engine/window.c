// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

// FIXME - #ifdefs on a per-platform basis
//#ifdef LINUX

// Using GLUT instead of XLIB on Linux because it's easier
#include "types.h"
#include <stdio.h>
#include <GL/glut.h>

void Window_Initialize(int width, int height, char* title, int argc, char* argv[]) {
    // Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // 1280x720 for FNaF 1, 1024x768 for 2/3
    // TODO: Convert Assets to allow for GLOBAL 720p
    glutInitWindowSize(width, height);

    // TODO: Position new Window in center of the Display
    glutInitWindowPosition(100, 100);

    glutCreateWindow(title);


}

void Window_Close() {
    glutDestroyWindow(0);
}

void Window_Update() {
    glutMainLoopEvent();
}

//#endif //LINUX