#ifndef _DEFS_H_
#define _DEFS_H_

#include "version.h"
#include "types.h"

#ifdef DESKTOP
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include "pc/graphics_wrapper.h"
#include "pc/general_wrapper.h"
#include "pc/sound_wrapper.h"
#include "keys.h"
#include "pc/window.h"
#endif

#include "sound.h"
#include "ai_handler.h"
#include "convert.h"
#include "time_manager.h"
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "math_manager.h"
#include "options.h"
#include "print.h"
#include "save.h"

#endif