#ifndef _DEFS_H_
#define _DEFS_H_

// STD-C includes
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// PSPSDK includes
#ifdef PSP
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <oslib/oslib.h>

#define printf pspDebugScreenPrintf
#endif

// UNIX/PC includes
#ifdef DESKTOP
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#endif

// Local headers
#include "types.h"
#include "print.h"
#include "options.h"
#include "input.h"
#include "sound.h"
#include "ini.h"
#include "lua_includes.h"
#include "lua_virtual_machine.h"
#include "math_manager.h"
#include "time_manager.h"
#include "game.h"

// PC local headers
#ifdef DESKTOP
#include "pc/window.h"
#endif

#endif