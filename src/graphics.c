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
// graphics.c - Graphics handling and wrapper
//

#include "defs.h"

image2d_t* Graphics_Images[MAX_IMAGE];
int        Graphics_LoadedImages;

//
// Graphics_LoadImage(Directory)
// Generic Image Loading function, intended
// for backgrounds or other less-relevant
// images
//
void Graphics_LoadImage(char* Directory) {
    if (Graphics_LoadedImages >= MAX_IMAGE) {
        Print_Normal("Failed to load image %s, reached max of %d!\n", Directory, MAX_IMAGE);
        return;
    }

    // Allocate the memory for a new image
    Graphics_Images[Graphics_LoadedImages] = malloc(sizeof(image2d_t));

    // Set the image's path
    Graphics_Images[Graphics_LoadedImages]->path = Directory;

    // Create the texture2d based on the path
    Graphics_Images[Graphics_LoadedImages]->texture = Graphics_CreateTextureFromFile(Directory);

    // Create a sprite in SFML
#ifdef DESKTOP
    Graphics_Images[Graphics_LoadedImages]->sprite = Graphics_CreateSprite();
    Graphics_BindTextureToSprite(Graphics_Images[Graphics_LoadedImages]->texture, Graphics_Images[Graphics_LoadedImages]->sprite);
#endif

    Graphics_LoadedImages++;
}

//
// Graphics_DrawImage(Directory)
// Draws Generic Images
//
void Graphics_DrawImage(char* Directory, int X, int Y) {
    // Find the image2d linked with this path
    image2d_t* Texture;
    for(int i = 0; i < Graphics_LoadedImages; ++i) {
        if (strcmp(Graphics_Images[i]->path, Directory) == 0) {
            Texture = Graphics_Images[i];
        }
    }

    // Draw it
    Graphics_DrawImageGeneric(Texture, X, Y);
}