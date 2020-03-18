// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)
#include <SFML/Graphics.h>

#include "types.h"
#include "input.h"

sfRenderWindow* GameWindow;

void Window_Initialize(int width, int height, char* title) {
    // Create Game Window
    sfVideoMode VideoMode = {width, height, 32};
    GameWindow = sfRenderWindow_create(VideoMode, title, sfClose, NULL);
}

// Easy Close Function
void Window_Close() {
    sfRenderWindow_close(GameWindow);
}


void Window_Update() {
    sfEvent WindowEvent;

    // Clear
    sfRenderWindow_clear(GameWindow, sfBlack);

    // Update
    sfRenderWindow_display(GameWindow);

    // Process Events (close)
    while (sfRenderWindow_pollEvent(GameWindow, &WindowEvent)) {
        if (WindowEvent.type == sfEvtClosed)
            Window_Close();
    }
}