#include <pspkernel.h>
#include "oslib.h"
#include "usb.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Globals:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceUID modules[8];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void oslStopUnloadModule(SceUID modID){
    int status = 0;
    sceKernelStopModule(modID, 0, NULL, &status, NULL);
    sceKernelUnloadModule(modID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int oslInitUsbStorage()		{
	u32 retVal = 0;

    //start necessary drivers
	modules[0] = pspSdkLoadStartModule("flash0:/kd/chkreg.prx", PSP_MEMORY_PARTITION_KERNEL);
    modules[1] = pspSdkLoadStartModule("flash0:/kd/npdrm.prx", PSP_MEMORY_PARTITION_KERNEL);
    modules[2] = pspSdkLoadStartModule("flash0:/kd/semawm.prx", PSP_MEMORY_PARTITION_KERNEL);
    modules[3] = pspSdkLoadStartModule("flash0:/kd/usbstor.prx", PSP_MEMORY_PARTITION_KERNEL);
    modules[4] = pspSdkLoadStartModule("flash0:/kd/usbstormgr.prx", PSP_MEMORY_PARTITION_KERNEL);
    modules[5] = pspSdkLoadStartModule("flash0:/kd/usbstorms.prx", PSP_MEMORY_PARTITION_KERNEL);
    modules[6] = pspSdkLoadStartModule("flash0:/kd/usbstorboot.prx", PSP_MEMORY_PARTITION_KERNEL);
	modules[7] = pspSdkLoadStartModule("flash0:/kd/usbdevice.prx", PSP_MEMORY_PARTITION_KERNEL); 

    //setup USB drivers
    retVal = sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
    if (retVal != 0)
		return -6;

    retVal = sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
    if (retVal != 0)
		return -7;

    retVal = sceUsbstorBootSetCapacity(0x800000);
    if (retVal != 0)
		return -8;

    return 0;
}

int oslStartUsbStorage()		{
    return sceUsbActivate(0x1c8);
}

int oslStopUsbStorage()		{
    int retVal = sceUsbDeactivate(0x1c8);
    sceIoDevctl("fatms0:", 0x0240D81E, NULL, 0, NULL, 0 ); //Avoid corrupted files
	return retVal;
}

int oslDeinitUsbStorage()			{
    int i;
	unsigned long state = oslGetUsbState();
    if (state & PSP_USB_ACTIVATED)
        oslStopUsbStorage();
    sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
    sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
    for (i=7; i>=0; i--)
		if (modules[i] >= 0)
			oslStopUnloadModule(modules[i]);
    return 0;
}
