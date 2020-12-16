/*

 Name:  MLREG.C

 Description:
 A single routine for registering all loaders in MikMod for the current
 platform.

 Portability:
 All systems - all compilers

*/

#include "mikmod.h"

void MikMod_RegisterAllLoaders(void)
{
	//Removed to save space
//   MikMod_RegisterLoader(load_uni);
   MikMod_RegisterLoader(load_it);
   MikMod_RegisterLoader(load_xm);
   MikMod_RegisterLoader(load_s3m);
   MikMod_RegisterLoader(load_mod);
/*   MikMod_RegisterLoader(load_mtm);
   MikMod_RegisterLoader(load_stm);
   MikMod_RegisterLoader(load_dsm);
   MikMod_RegisterLoader(load_med);
   MikMod_RegisterLoader(load_far);
   MikMod_RegisterLoader(load_ult);
   MikMod_RegisterLoader(load_669);*/
}
