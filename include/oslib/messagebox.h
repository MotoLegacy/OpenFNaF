#ifndef _OSL_MESSAGEBOX_H_
#define _OSL_MESSAGEBOX_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup msgbox Message boxes

	Message box functions in OSLib.
	@{
*/


/** Message box button actions. */
enum OSL_MB_ACTIONS			{
	OSL_MB_OK=1,								//!< Ok button
	OSL_MB_CANCEL,								//!< Cancel button
	OSL_MB_YES,									//!< Yes button
	OSL_MB_NO,									//!< No button
	OSL_MB_QUIT									//!< Quit button (exits immediately with an oslQuit call!)
};

//*** Interface utilisateur ***
#ifdef PSP
	/** Displays a debug message, with the same format as printf. The title displays the file, the line number and the function from where it has been called.
	The following example displays 'var = 2' in a message box, where you can either continue execution (close the message box) either quit your application directly (press triangle):
\code
int var = 2;
oslDebug("var = %i", var);
\endcode */
	#define oslDebug(format...)		({ char __str2[1000], __str[1000]; sprintf(__str2, "Debug (%s:%i,%s)",__FUNCTION__,__LINE__,__FILE__); sprintf(__str , ##format); oslMessageBox(__str, __str2, oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_OK,OSL_KEY_TRIANGLE,OSL_MB_QUIT,0,0)); })

	/** Creates a 32-bit integer to be used for the 'flags' parameter when calling oslMessageBox. There are 6 arguments, 2 per button. The first one of each pair represents the button assigned (one of the
	#OSL_KEY_BITS values) and the second one represents the action (one of #OSL_MB_ACTIONS values).
\code
//We only have 2 buttons, one which is "ok", assigned to the cross button, and one "cancel" assigned to the circle button. A double zero means no button.
oslMessageBox("Message text", "Message title", oslMake3Buttons(OSL_KEY_CROSS, OSL_MB_OK, OSL_KEY_CIRCLE, OSL_MB_CANCEL, 0, 0));
\endcode */
	#define oslMake3Buttons(b1,a1,b2,a2,b3,a3)		((b1)|((a1)<<5)|((b2)<<9)|((a2)<<14)|((b3)<<18)|((a3)<<23))
	/** Same as #oslDebug, except that it displays a warning message instead. */
	#define oslWarning(format...)			({ char __str[1000]; sprintf(__str , ##format); oslMessageBox(__str, "Warning", oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_OK,OSL_KEY_TRIANGLE,OSL_MB_QUIT,0,0)); })
	/** Same as #oslDebug, except that it displays a fatal error message instead. You cannot continue the execution from here, the only available choice is 'Quit', so you are sure this function is a
	dead end and no one will ever return from there. */
	#define oslFatalError(format...)		({ char __str[1000]; sprintf(__str , ##format); oslMessageBox(__str, "Fatal error", oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_QUIT,0,0,0,0)); })
#else
	#define oslDebug(...)		{ char __str2[1000], __str[1000]; sprintf(__str2, "Debug (%s:%i,%s)",__FUNCTION__,__LINE__,__FILE__); sprintf(__str , __VA_ARGS__); oslMessageBox(__str, __str2, oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_OK,OSL_KEY_TRIANGLE,OSL_MB_QUIT,0,0)); }
	#define oslMake3Buttons(b1,a1,b2,a2,b3,a3)		((b1)|((a1)<<5)|((b2)<<9)|((a2)<<14)|((b3)<<18)|((a3)<<23))
	#define oslWarning(...)			{ char __str[1000]; sprintf(__str , __VA_ARGS__); oslMessageBox(__str, "Warning", oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_OK,OSL_KEY_TRIANGLE,OSL_MB_QUIT,0,0)); }
	#define oslFatalError(...)		{ char __str[1000]; sprintf(__str , __VA_ARGS__); oslMessageBox(__str, "Fatal error", oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_QUIT,0,0,0,0)); }
#endif

/** Displays a fatal error message if the condition is not satisfied.

\code
//Try to load an image
OSL_IMAGE *img = oslLoadImageFilePNG("test.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_5551);
//Make sure the image pointer is not NULL (meaning the loading failed).
oslAssert(img != NULL);
//Note that the 'img != NULL' condition can be replaced with 'img' (this is a feature of the C language)
//So the following code is equivalent but shorter to write:
oslAssert(img);
//Also you could do something like this:
if (!img)		{
	//Displays an error message box
	oslMessageBox("The image test.png could not be loaded...", "Error", oslMake3Buttons(OSL_KEY_CROSS, OSL_MB_OK, 0, 0, 0, 0));
	//Exit as some important resource is missing
	oslQuit();
}
//We will only get there if the image was loaded successfully.
...
\endcode
*/
#define oslAssert(cond)		({ if (!(cond)) { char __str[1000]; sprintf(__str , "This program encountered a fatal error and must be terminated.\n\nFile : %s:%i\nError: %s",__FILE__,__LINE__,""#cond); oslMessageBox(__str, "Fatal error", oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_QUIT,0,0,0,0)); } })

/** Displays a message box.
	\param text
		Text displayed inside of the message box.
	\param title
		Title of the message box.
	\param flags
		See #oslMake3Buttons.

There is a code sample with the #oslMake3Buttons documentation. */
extern unsigned int oslMessageBox(const char *text, const char *title, unsigned int flags);

//Not available anymore as it crashes with the newer firmwares
//extern void oslSystemMessage(const char *message);

/*
	Code à copier dans les projets où PSPSDK et OSLib ne sont pas supportés pour afficher un oslDebug.

	enum {OSL_MB_OK=1, OSL_MB_CANCEL, OSL_MB_YES, OSL_MB_NO, OSL_MB_QUIT};
	enum {OSL_KEY_SELECT=1, OSL_KEY_START=4, OSL_KEY_UP=5, OSL_KEY_RIGHT=6, OSL_KEY_DOWN=7, OSL_KEY_LEFT=8, OSL_KEY_L=9, OSL_KEY_R=10,
			OSL_KEY_TRIANGLE=13, OSL_KEY_CIRCLE=14, OSL_KEY_CROSS=15, OSL_KEY_SQUARE=16, OSL_KEY_HOME=17, OSL_KEY_HOLD=18, OSL_KEY_NOTE=24};
	extern unsigned int oslMessageBox(const char *text, const char *title, unsigned int flags);
	#define oslMake3Buttons(b1,a1,b2,a2,b3,a3)		((b1)|((a1)<<5)|((b2)<<9)|((a2)<<14)|((b3)<<18)|((a3)<<23))
	#define oslDebug(format...)		({ char __str2[1000], __str[1000]; sprintf(__str2, "Debug (%s:%i,%s)",__FUNCTION__,__LINE__,__FILE__); sprintf(__str , ##format); oslMessageBox(__str, __str2, oslMake3Buttons(OSL_KEY_CROSS,OSL_MB_OK,OSL_KEY_TRIANGLE,OSL_MB_QUIT,0,0)); })
*/

/** @} */ // end of msgbox

#ifdef __cplusplus
}
#endif

#endif
