
#include <stdio.h>
#include <stdarg.h>

#include "options.h"

// Normal printf, but with a verbose check.
void Print_Normal(char* message,...) {
    if (!OPT_VERBOSE)
        return;

    va_list argptr;
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
}