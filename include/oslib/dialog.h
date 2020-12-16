#ifndef DIALOG_H
#define DIALOG_H

/** @defgroup Dialogs Dialogs

	Functions to display Sony's dialogs
	@{
*/

#define OSL_DIALOG_CANCEL 1
#define OSL_DIALOG_OK 0

/**No dialog*/
#define OSL_DIALOG_NONE 0
/**Message dialog*/
#define OSL_DIALOG_MESSAGE 1
/**Error dialog*/
#define OSL_DIALOG_ERROR 2
/**Net conf dialog*/
#define OSL_DIALOG_NETCONF 3
//<-- STAS: another dialog types -->
#define OSL_DIALOG_OSK 4
#define OSL_DIALOG_SAVELOAD 5
#define OSL_DIALOG_BROWSER 6

/**Universal routine which draws the current dialog (of the given dialog type)
   and waits for the user finish work with it. Returns 0 or ErrorCode (<0).
*/
int oslDialogDrawAndWait(int dialogType);

/**Tests whether any of ERROR,MESSAGE or NETCONF dialogs is currently active
*/
int oslDialogIsActive();
//<-- STAS END -->
#define OSL_DIALOG_STATUS_NONE 		PSP_UTILITY_DIALOG_NONE		/**< No dialog is currently active */
#define OSL_DIALOG_STATUS_INIT 		PSP_UTILITY_DIALOG_INIT		/**< The dialog is currently being initialized */
#define OSL_DIALOG_STATUS_VISIBLE 	PSP_UTILITY_DIALOG_VISIBLE	/**< The dialog is visible and ready for use */
#define OSL_DIALOG_STATUS_QUIT 		PSP_UTILITY_DIALOG_QUIT		/**< The dialog has been canceled and should be shut down */
#define OSL_DIALOG_STATUS_FINISHED 	PSP_UTILITY_DIALOG_FINISHED	/**< The dialog has successfully shut down */

/**Initializes a Message Dialog
	\param *message
        Text shown in the message dialog
    \param enableYesno
        if 1 the dialog will have Yes/No option
*/
int oslInitMessageDialog(const char *message, int enableYesno);

/**Initializes an Error Dialog
	\param error
        Error code
*/
int oslInitErrorDialog(const unsigned int error);

/**Draws the current dialog to screen.
After drawing it you should check if the user closed it. Remember to call oslEndDialog.
\code
oslDrawDialog();
if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE){
    //The user closed the dialog
    oslEndDialog();
}
\endcode
*/
void oslDrawDialog();

/**Returns the current dialog type
*/
int oslGetDialogType();

/**Returns the current dialog status
*/
int oslGetDialogStatus();

/**Returns the button pressed in the dialog (only for message dialog with enableYesno == 1)
*/
int oslGetDialogButtonPressed();

/**Initializes the net conf dialog
*/
int oslInitNetDialog();

/**Gets the dialog result (OSL_DIALOG_CANCEL or OSL_DIALOG_OK)
*/
int oslDialogGetResult();

/**Ends the current dialog
*/
void oslEndDialog();

/** @} */ // end of dialog
#endif
