#ifndef _INPUT_H_
#define _INPUT_H_

#include "keys.h"
#include "window.h"
#include "types.h"

// 16 should be enough.. right?
#define MAX_KEYFUNCTIONS       16

typedef struct {
  sfKeyCode key;
  void (*func)();
  bool pressed;
} keydata_t;

extern void Input_CheckButtons();
extern void Input_RegisterKey(sfKeyCode key, void (*func));

#endif