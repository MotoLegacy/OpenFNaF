#ifndef _DEFS_H_
#define _DEFS_H_

// PSPSDK includes
#ifdef PSP
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#endif

// UNIX/PC includes
#ifdef DESKTOP
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

// STD-C includes
#include <string.h>
#include <stdlib.h>

// Local headers
#include "ini.h"

#endif