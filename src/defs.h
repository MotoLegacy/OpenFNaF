#ifndef _DEFS_H_
#define _DEFS_H_

// STD-C includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// PSPSDK includes
#ifdef PSP
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#define printf pspDebugScreenPrintf
#endif

// UNIX/PC includes
#ifdef DESKTOP
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

// Local headers
#include "types.h"
#include "options.h"
#include "ini.h"
#include "game.h"

#endif