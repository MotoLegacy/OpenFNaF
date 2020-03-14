#ifndef _INPUT_H_
#define _INPUT_H_

extern void Input_SpecialKeyHandler(int key, int x, int y);
extern void Input_GenericKeyHandler(unsigned char key, int x, int y);
extern void Input_MousePassiveHandler(int x, int y);
extern void Input_MouseClickHandler(int button, int state, int x, int y);

#endif