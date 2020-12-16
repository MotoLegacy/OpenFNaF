#ifndef OSK_H
#define OSK_H

/** @defgroup OSK On-Screen Keyboard

	Functions to display Sony's On Screen Keyboard
	@{
*/
//<-- STAS: define the OSL_KEYBOARD structure and its methods for multithreaded environment
typedef struct {
    SceUtilityOskParams oskParams;
//    SceUtilityOskData  *oskData;
} OSL_KEYBOARD;

extern OSL_KEYBOARD* osl_osk;

extern OSL_KEYBOARD* oslInitOskEx(int nData, int language);

extern int oslInitOskDataEx(OSL_KEYBOARD *kbd, int idx, unsigned short *desc,
                            unsigned short *intext, int textLimit, int linesNumber);

extern int oslActivateOskEx(OSL_KEYBOARD *kbd, int waitcycle);

extern int oslOskIsActiveEx(OSL_KEYBOARD *kbd);

extern void oslDeActivateOskEx(OSL_KEYBOARD *kbd);

extern int oslOskGetResultEx(OSL_KEYBOARD *kbd, int idx);

extern unsigned short* oslOskOutTextEx(OSL_KEYBOARD *kbd, int idx);

extern void oslEndOskEx(OSL_KEYBOARD *kbd);


#define OSL_OSK_RESULT_UNCHANGED	PSP_UTILITY_OSK_RESULT_UNCHANGED
#define OSL_OSK_RESULT_CANCELLED	PSP_UTILITY_OSK_RESULT_CANCELLED
#define OSL_OSK_RESULT_CHANGED		PSP_UTILITY_OSK_RESULT_CHANGED
//<-- STAS END -->

/**OSK cancel*/
#define OSL_OSK_CANCEL    PSP_UTILITY_OSK_RESULT_CANCELLED		/**<-- STAS: for backward compatibility */
/**OSK changed*/
#define OSL_OSK_CHANGED    PSP_UTILITY_OSK_RESULT_CHANGED
/**OSK unchanged*/
#define OSL_OSK_UNCHANGED    PSP_UTILITY_OSK_RESULT_UNCHANGED

/** Initializes the OSK
	\param *descStr
        Text shown as a description (bottom right corner)
    \param initialStr
        Initial text in the OSK
    \param textLimit
        Maximum number of chars
    \param linesNumber
        Number of lines
    \param language
        Language
			JAPANESE			0
			ENGLISH				1
			FRENCH				2
			SPANISH				3
			GERMAN				4
			ITALIAN				5
			DUTCH				6
			PORTUGUESE			7
			RUSSIAN				8
			KOREAN				9
			CHINESE_TRADITIONAL	10
			CHINESE_SIMPLIFIED	11
		If you pass to the function -1 then the language set in the firmware is used
*/
void oslInitOsk(char *descStr, char *initialStr, int textLimit, int linesNumber, int language);

/** Draws the OSK
After drawing it you should check if the user closed it. Remember to call oslEndOsk.
\code
if (oslOskIsActive()){
    oslDrawOsk();
    if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE){
        //The user closed the OSK
        oslEndOsk();
    }
}
\endcode
*/
void oslDrawOsk();

/** Checks if the OSK is active.
*/
int oslOskIsActive();

/**Returns the current OSK status
*/
int oslGetOskStatus();

/**Returns the OSK result (OSL_OSK_CHANGED, OSL_OSK_UNCHANGED or OSL_OSK_CANCEL)
*/
int oslOskGetResult();

/**Get the text inserted in the OSK
*/
void oslOskGetText(char *text);
void oslOskGetTextUCS2(unsigned short *text);

/**Ends the OSK
*/
void oslEndOsk();


/** @} */ // end of osk
#endif
