// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "graphics.h"

extern sfRenderWindow* GameWindow;

extern void Window_Initialize(int width, int height, char* title);
extern void Window_Close();
extern void Window_Update();
extern void Window_Clear();

#endif
