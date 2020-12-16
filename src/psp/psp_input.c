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
// psp_input.c - Input checking system for PSP
//

#include "../defs.h"

int     anub_psp_rx;
int     anub_psp_ry;
int     anub_psp_lx;
int     anub_psp_ly;
boolean btn_psp_cross;
boolean btn_psp_circle;
boolean btn_psp_square;
boolean btn_psp_triangle;
boolean btn_psp_up;
boolean btn_psp_down;
boolean btn_psp_left;
boolean btn_psp_right;
boolean btn_psp_ltrigger;
boolean btn_psp_rtrigger;
boolean btn_psp_start;
boolean btn_psp_select;

SceCtrlData controller;

void Input_Initialize() {
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void Input_Loop() {
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    sceCtrlReadBufferPositive(&controller, 1); 

    if (controller.Buttons & PSP_CTRL_CROSS) {
        btn_psp_cross = TRUE;
    } else {
        btn_psp_cross = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_CIRCLE) {
        btn_psp_circle = TRUE;
    } else {
        btn_psp_circle = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_SQUARE) {
        btn_psp_square = TRUE;
    } else {
        btn_psp_square = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_UP) {
        btn_psp_up = TRUE;
    } else {
        btn_psp_up = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_DOWN) {
        btn_psp_down = TRUE;
    } else {
        btn_psp_down = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_LEFT) {
        btn_psp_left = TRUE;
    } else {
        btn_psp_left = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_RIGHT) {
        btn_psp_right = TRUE;
    } else {
        btn_psp_right = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_LTRIGGER) {
        btn_psp_ltrigger = TRUE;
    } else {
        btn_psp_ltrigger = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_RTRIGGER) {
        btn_psp_rtrigger = TRUE;
    } else {
        btn_psp_rtrigger = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_START) {
        btn_psp_start = TRUE;
    } else {
        btn_psp_start = FALSE;
    }
    if (controller.Buttons & PSP_CTRL_SELECT) {
        btn_psp_select = TRUE;
    } else {
        btn_psp_select = FALSE;
    }

    anub_psp_lx = controller.Lx;
    anub_psp_ly = controller.Ly;
    anub_psp_rx = controller.Rsrv[0];
    anub_psp_ry = controller.Rsrv[1];
}

int Input_ButtonPressedDown(int button) {
    switch(button) {
        case INPUT_PSP_CROSS:
            return btn_psp_cross;
        case INPUT_PSP_CIRCLE:
            return btn_psp_circle;
        case INPUT_PSP_TRIANGLE:
            return btn_psp_triangle;
        case INPUT_PSP_SQUARE:
            return btn_psp_square;
        case INPUT_PSP_UP:
            return btn_psp_up;
        case INPUT_PSP_DOWN:
            return btn_psp_down;
        case INPUT_PSP_LEFT:
            return btn_psp_left;
        case INPUT_PSP_RIGHT:
            return btn_psp_right;
        case INPUT_PSP_LTRIGGER:
            return btn_psp_ltrigger;
        case INPUT_PSP_RTRIGGER:
            return btn_psp_rtrigger;
        case INPUT_PSP_START:
            return btn_psp_start;
        case INPUT_PSP_SELECT:
            return btn_psp_select;
        default:
            return FALSE;
    }
}