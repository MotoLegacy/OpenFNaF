#ifndef USB_H
#define USB_H

#include <pspusb.h>
#include <pspusbstor.h>
#include <pspsdk.h>

#define oslGetUsbState sceUsbGetState

/** @defgroup Usb USB

    USB functions to start/stop usb storage
	@{
*/

enum {OSL_USB_ACTIVATED=PSP_USB_ACTIVATED,
      OSL_USB_CABLE_CONNECTED=PSP_USB_CABLE_CONNECTED,
	  OSL_USB_CONNECTION_ESTABLISHED=PSP_USB_CONNECTION_ESTABLISHED};

/** Initializes USB modules, must be called before oslStartUsbStorage.
Returns 0 on success.*/
extern int oslInitUsbStorage();

/** Starts USB storage
Returns 0 on success.*/
extern int oslStartUsbStorage();

/** Stops USB storage
Returns 0 on success.*/
extern int oslStopUsbStorage();

/** Deinitializes USB modules. */
extern int oslDeinitUsbStorage();

/** @} */ // end of USB

#endif
