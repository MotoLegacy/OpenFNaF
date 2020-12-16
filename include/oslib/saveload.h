#ifndef SAVELOAD_H
#define SAVELOAD_H

/** @defgroup saveload Save and Load

	Functions to use the save and load screen
	@{
*/

/**Save and Load cancel*/
#define OSL_SAVELOAD_CANCEL 1
/**Save and Load OK*/
#define OSL_SAVELOAD_OK 0

/**Save dialog*/
#define OSL_DIALOG_SAVE 1
/**Load dialog*/
#define OSL_DIALOG_LOAD 2
/**Delete dialog*/
#define OSL_DIALOG_DELETE 3
/**Struct containing data to save/load
	\param gameTitle[0x80]
        Title of the game
	\param gameID[13]
        Game ID of the game
	\param saveName[20]
        Save name
	\param savedataTitle[0x80]
        ParamSfo SaveData Title
	\param detail[0x400]
        ParamSfo SaveData Detail
	\param (*nameList)[20]
        List of names for saves data
	\param *pic1
        Pic1 data
	\param size_pic1
        Pic1 size
	\param *icon0
        Icon0 data
	\param size_icon0
        Icon0 size
	\param dialogType
		Type of load or save Dialog, example 0 for multi list, 1 single data, 2 automatic
	\param *data
        Pointer to data to be saved/loaded
	\param dataSize
        Size of data to be saved/loaded
*/
struct oslSaveLoad{
    char gameTitle[0x80];
    char gameID[13];
    char saveName[20];
	char savedataTitle[0x80];
	char detail[0x400];
    char (*nameList)[20];
    unsigned char *pic1;
    unsigned int size_pic1;
    unsigned char *icon0;
    unsigned int size_icon0;
	int dialogType;
    void *data;
    int dataSize;
};

/**Initializes the save dialog
	\param *saveData
        Pointer to a oslSaveLoad structure containing data to be saved
*/
void oslInitSaveDialog(struct oslSaveLoad *saveData);

/**Initializes the multilist save dialog
	\param *saveData
        Pointer to a oslSaveLoad structure containing data to be saved
*/
void oslInitMultiSaveDialog(struct oslSaveLoad *saveData);

/**Initializes the single save dialog
	\param *saveData
        Pointer to a oslSaveLoad structure containing data to be saved
*/
void oslInitSingleSaveDialog(struct oslSaveLoad *saveData);

/**Initializes the automatic save dialog
	\param *saveData
        Pointer to a oslSaveLoad structure containing data to be saved
*/
void oslInitAutoSaveDialog(struct oslSaveLoad *saveData);
/**Initializes the load dialog
	\param *loadData
        Pointer to a oslSaveLoad structure where data will be loaded
*/
void oslInitLoadDialog(struct oslSaveLoad *loadData);
/**Initializes the multilist load dialog
	\param *loadData
        Pointer to a oslSaveLoad structure where data will be loaded
*/
void oslInitMultiLoadDialog(struct oslSaveLoad *loadData);
/**Initializes the single load dialog
	\param *loadData
        Pointer to a oslSaveLoad structure where data will be loaded
*/
void oslInitSingleLoadDialog(struct oslSaveLoad *loadData);

/**Initializes the automatic load dialog
	\param *loadData
        Pointer to a oslSaveLoad structure where data will be loaded
*/
void oslInitAutoLoadDialog(struct oslSaveLoad *loadData);

void oslInitDeleteDialog(struct oslSaveLoad *deleteData);
/**Draws the save/load dialog
*/
void oslDrawSaveLoad();

/**Returns the current dialog status
*/
int oslGetLoadSaveStatus();

/**Returns the current dialog type
*/
int oslGetSaveLoadType();

/**Gets the dialog's result
*/
int oslSaveLoadGetResult();

/**Ends the current dialog
*/
void oslEndSaveLoadDialog();

/** @} */ // end of saveload
#endif
