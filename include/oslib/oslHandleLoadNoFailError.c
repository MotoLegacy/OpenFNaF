#include "oslib.h"

void osl_noFailDefaultCallback(const char *filename, u32 reserved);
void (*osl_noFailCallback)(const char *filename, u32 reserved) = osl_noFailDefaultCallback;

void osl_noFailDefaultCallback(const char *filename, u32 reserved)		{
	oslFatalError("Unable to locate the following file: %s\nPlease reinstall the application and read the instructions carefully.", filename);
}

void oslHandleLoadNoFailError(const char *filename)		{
	//Add 1 to enable error trapping
	if (osl_noFail & 1)		{
		osl_noFailCallback(filename, 0);
		//Add 2 to avoid quitting after the error message
		if (!(osl_noFail & 2))
			oslQuit();
	}
}
