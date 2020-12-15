//
// MIT License
//
// Copyright (c) 2020 MotoLegacy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

//
// window.c - initialization and updates for the PC Game Window
//

#include "../defs.h"

sfRenderWindow* GameWindow;

void Window_Initialize(int width, int height, char* title) {
    // Create Game Window
    sfVideoMode VideoMode = {width, height, 32};
    GameWindow = sfRenderWindow_create(VideoMode, title, sfClose, NULL);
}

// Easy Close Function
void Window_Close() {
    sfRenderWindow_close(GameWindow);
    Game_Running = FALSE;
}

void Window_Update() {
    sfEvent WindowEvent;

    // Update
    sfRenderWindow_display(GameWindow);

    // Process Events (close)
    while (sfRenderWindow_pollEvent(GameWindow, &WindowEvent)) {
        if (WindowEvent.type == sfEvtClosed)
            Window_Close();
    }
}

void Window_Clear() {
    sfRenderWindow_clear(GameWindow, sfBlack);
}