/*

 Name:  MDREG.C

 Description:
 A single routine for registering all drivers in MikMod for the current
 platform.

 Portability:
 DOS, WIN95, OS2, SunOS, Solaris,
 Linux, HPUX, AIX, SGI, Alpha

 Anything not listed above is assumed to not be supported by this procedure!

 All Others: n

 - all compilers!

*/

#include "mikmod.h"

void MikMod_RegisterAllDrivers(void)
{

#ifdef SUN
    MikMod_RegisterDriver(drv_sun);
#elif defined(PSP)
	MikMod_RegisterDriver(drv_psp);
#elif defined(SOLARIS)
    MikMod_RegisterDriver(drv_sun);
#elif defined(__alpha)
    MikMod_RegisterDriver(drv_AF);
#elif defined(OSS)
    MikMod_RegisterDriver(drv_oss);
    MikMod_RegisterDriver(drv_stdout);
    #ifdef ULTRA
       MikMod_RegisterDriver(drv_ultra);
    #endif /* ULTRA */
#elif defined(__hpux)
    MikMod_RegisterDriver(drv_hp);
#elif defined(AIX)
    MikMod_RegisterDriver(drv_aix);
#elif defined(SGI)
    MikMod_RegisterDriver(drv_sgi);
#elif defined(__OS2__)
    MikMod_RegisterDriver(drv_os2);
#elif defined(__WIN32__)
    MikMod_RegisterDriver(drv_w95);
#else
    MikMod_RegisterDriver(drv_awe);
    MikMod_RegisterDriver(drv_gus);
/*    MikMod_RegisterDriver(drv_gus2);     // use for hardware mixing only (smaller / faster) */
    MikMod_RegisterDriver(drv_pas);
    MikMod_RegisterDriver(drv_wss);
    MikMod_RegisterDriver(drv_ss);
    MikMod_RegisterDriver(drv_sb16);
    MikMod_RegisterDriver(drv_sbpro);
    MikMod_RegisterDriver(drv_sb);
#endif

}


