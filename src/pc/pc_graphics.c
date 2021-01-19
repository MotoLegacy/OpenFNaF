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
// pc_graphics.c - PC graphics handling
//

#include "../defs.h"

texture2d_t* Graphics_CreateTextureFromFile(char* Directory)
{
    texture2d_t TempTexture;
    texture2d_t* TempTexturePtr;
    TempTexture = LoadTexture(Directory);
    TempTexturePtr = malloc(sizeof(texture2d_t));

    TempTexturePtr->id = TempTexture.id;
    TempTexturePtr->width = TempTexture.width;
    TempTexturePtr->height = TempTexture.height;
    TempTexturePtr->mipmaps = TempTexture.mipmaps;
    TempTexturePtr->format = TempTexture.format;


    return TempTexturePtr;
}

void Graphics_DrawImageGeneric(texture2d_t* Image, int X, int Y)
{
    DrawTexture(*Image, X, Y, WHITE);
}

void Graphics_StartDrawing()
{
    BeginDrawing();
}

void Graphics_StopDrawing()
{
    EndDrawing();
}