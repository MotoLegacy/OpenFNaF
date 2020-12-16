#include <pspkernel.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <psputility.h>

#include "oslib.h"
#include "saveload.h"

SceUtilitySavedataParam savedata;
PspUtilitySavedataListSaveNewData newData;
char key[] = "QTAK319JQKJ952HA";
int saveLoadType = OSL_DIALOG_NONE;

void oslInitSaveDialog(struct oslSaveLoad *saveData){
	if(saveData->dialogType == 0){
		oslInitMultiSaveDialog(saveData);
	}else if(saveData->dialogType == 1){
		oslInitSingleSaveDialog(saveData);
	}else if(saveData->dialogType == 2){
		oslInitAutoSaveDialog(saveData);
	} else {
		oslInitMultiSaveDialog(saveData);
	}
}

void oslInitMultiSaveDialog(struct oslSaveLoad *saveData){
	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	memset(&newData, 0, sizeof(PspUtilitySavedataListSaveNewData));

	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	savedata.mode = PSP_UTILITY_SAVEDATA_LISTSAVE;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, saveData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, saveData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	savedata.saveNameList = saveData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

	// Allocate buffers used to store various parts of the save data
	savedata.dataBuf = malloc(saveData->dataSize);
	savedata.dataBufSize = saveData->dataSize;
	savedata.dataSize = saveData->dataSize;

    memset(savedata.dataBuf, 0, saveData->dataSize);
    memcpy(savedata.dataBuf, saveData->data, saveData->dataSize);

	strcpy(savedata.sfoParam.title, saveData->gameTitle);
	strcpy(savedata.sfoParam.savedataTitle, saveData->savedataTitle);
	strcpy(savedata.sfoParam.detail, saveData->detail);
	savedata.sfoParam.parentalLevel = 1;

	// No icon1
	savedata.icon1FileData.buf = NULL;
	savedata.icon1FileData.bufSize = 0;
	savedata.icon1FileData.size = 0;

	savedata.pic1FileData.buf = saveData->pic1;
	savedata.pic1FileData.bufSize = saveData->size_pic1;
	savedata.pic1FileData.size = saveData->size_pic1;

	savedata.icon0FileData.buf = saveData->icon0;
	savedata.icon0FileData.bufSize = saveData->size_icon0;
	savedata.icon0FileData.size = saveData->size_icon0;

	// No snd0
	savedata.snd0FileData.buf = NULL;
	savedata.snd0FileData.bufSize = 0;
	savedata.snd0FileData.size = 0;

	// Set title
	newData.title = saveData->gameTitle;

	// Set new data
	savedata.newData = &newData;

	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY;
    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_SAVE;
}

void oslInitSingleSaveDialog(struct oslSaveLoad *saveData){
	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	memset(&newData, 0, sizeof(PspUtilitySavedataListSaveNewData));

	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	//savedata.mode = PSP_UTILITY_SAVEDATA_LISTSAVE;
	savedata.mode = PSP_UTILITY_SAVEDATA_SAVE;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, saveData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, saveData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	//savedata.saveNameList = saveData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

	// Allocate buffers used to store various parts of the save data
	savedata.dataBuf = malloc(saveData->dataSize);
	savedata.dataBufSize = saveData->dataSize;
	savedata.dataSize = saveData->dataSize;

    memset(savedata.dataBuf, 0, saveData->dataSize);
    memcpy(savedata.dataBuf, saveData->data, saveData->dataSize);

	strcpy(savedata.sfoParam.title, saveData->gameTitle);
	strcpy(savedata.sfoParam.savedataTitle, saveData->savedataTitle);
	strcpy(savedata.sfoParam.detail, saveData->detail);
	savedata.sfoParam.parentalLevel = 1;

	// No icon1
	savedata.icon1FileData.buf = NULL;
	savedata.icon1FileData.bufSize = 0;
	savedata.icon1FileData.size = 0;

	savedata.pic1FileData.buf = saveData->pic1;
	savedata.pic1FileData.bufSize = saveData->size_pic1;
	savedata.pic1FileData.size = saveData->size_pic1;

	savedata.icon0FileData.buf = saveData->icon0;
	savedata.icon0FileData.bufSize = saveData->size_icon0;
	savedata.icon0FileData.size = saveData->size_icon0;

	// No snd0
	savedata.snd0FileData.buf = NULL;
	savedata.snd0FileData.bufSize = 0;
	savedata.snd0FileData.size = 0;

	// Set title
	newData.title = saveData->gameTitle;

	// Set new data
	savedata.newData = &newData;

	//savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY;
    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_SAVE;
}

void oslInitAutoSaveDialog(struct oslSaveLoad *saveData){
	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	memset(&newData, 0, sizeof(PspUtilitySavedataListSaveNewData));

	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	//savedata.mode = PSP_UTILITY_SAVEDATA_LISTSAVE;
	savedata.mode = PSP_UTILITY_SAVEDATA_AUTOSAVE;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, saveData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, saveData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	//savedata.saveNameList = saveData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

	// Allocate buffers used to store various parts of the save data
	savedata.dataBuf = malloc(saveData->dataSize);
	savedata.dataBufSize = saveData->dataSize;
	savedata.dataSize = saveData->dataSize;

    memset(savedata.dataBuf, 0, saveData->dataSize);
    memcpy(savedata.dataBuf, saveData->data, saveData->dataSize);

	strcpy(savedata.sfoParam.title, saveData->gameTitle);
	strcpy(savedata.sfoParam.savedataTitle, saveData->savedataTitle);
	strcpy(savedata.sfoParam.detail, saveData->detail);
	savedata.sfoParam.parentalLevel = 1;

	// No icon1
	savedata.icon1FileData.buf = NULL;
	savedata.icon1FileData.bufSize = 0;
	savedata.icon1FileData.size = 0;

	savedata.pic1FileData.buf = saveData->pic1;
	savedata.pic1FileData.bufSize = saveData->size_pic1;
	savedata.pic1FileData.size = saveData->size_pic1;

	savedata.icon0FileData.buf = saveData->icon0;
	savedata.icon0FileData.bufSize = saveData->size_icon0;
	savedata.icon0FileData.size = saveData->size_icon0;

	// No snd0
	savedata.snd0FileData.buf = NULL;
	savedata.snd0FileData.bufSize = 0;
	savedata.snd0FileData.size = 0;

	// Set title
	newData.title = saveData->gameTitle;

	// Set new data
	savedata.newData = &newData;

	//savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY;
    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_SAVE;
}

void oslInitLoadDialog(struct oslSaveLoad *loadData){
	if(loadData->dialogType == 0){
		oslInitMultiLoadDialog(loadData);
	}else if(loadData->dialogType == 1){
		oslInitSingleLoadDialog(loadData);
	}else if(loadData->dialogType == 2){
		oslInitAutoLoadDialog(loadData);
	} else {
		oslInitMultiLoadDialog(loadData);
	}
}

void oslInitMultiLoadDialog(struct oslSaveLoad *loadData){
	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	savedata.mode = PSP_UTILITY_SAVEDATA_LISTLOAD;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, loadData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, loadData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	savedata.saveNameList = loadData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

    savedata.dataBuf = loadData->data;
	savedata.dataBufSize = loadData->dataSize;
	savedata.dataSize = loadData->dataSize;

    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_LOAD;
}

void oslInitSingleLoadDialog(struct oslSaveLoad *loadData){
	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	//savedata.mode = PSP_UTILITY_SAVEDATA_LISTLOAD;
	savedata.mode = PSP_UTILITY_SAVEDATA_LOAD;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, loadData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, loadData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	//savedata.saveNameList = loadData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

    savedata.dataBuf = loadData->data;
	savedata.dataBufSize = loadData->dataSize;
	savedata.dataSize = loadData->dataSize;

    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_LOAD;
}

void oslInitAutoLoadDialog(struct oslSaveLoad *loadData){
	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	//savedata.mode = PSP_UTILITY_SAVEDATA_LISTLOAD;
	savedata.mode = PSP_UTILITY_SAVEDATA_AUTOLOAD;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, loadData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, loadData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	//savedata.saveNameList = loadData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

    savedata.dataBuf = loadData->data;
	savedata.dataBufSize = loadData->dataSize;
	savedata.dataSize = loadData->dataSize;

    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_LOAD;
}

void oslInitDeleteDialog(struct oslSaveLoad *deleteData){

	memset(&savedata, 0, sizeof(SceUtilitySavedataParam));
	savedata.base.size = sizeof(SceUtilitySavedataParam);

    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &savedata.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &savedata.base.buttonSwap);
	savedata.base.graphicsThread = 0x11;
	savedata.base.accessThread = 0x13;
	savedata.base.fontThread = 0x12;
	savedata.base.soundThread = 0x10;

	//savedata.mode = PSP_UTILITY_SAVEDATA_LISTLOAD;
	savedata.mode = PSP_UTILITY_SAVEDATA_LISTDELETE;
	savedata.overwrite = 1;
	savedata.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if _PSP_FW_VERSION >= 200
	strncpy(savedata.key, key, 16);
#endif
	strcpy(savedata.gameName, deleteData->gameID);	// First part of the save name, game identifier name
	strcpy(savedata.saveName, deleteData->saveName);	// Second part of the save name, save identifier name

	// List of multiple names
	//char nameList[][20] = {"",""};
	//strcpy(nameList[0][0],deleteData->saveName);
	savedata.saveNameList = deleteData->nameList;

	strcpy(savedata.fileName, "DATA.BIN");	// name of the data file

    savedata.dataBuf = deleteData->data;
	savedata.dataBufSize = deleteData->dataSize;
	savedata.dataSize = deleteData->dataSize;

    sceUtilitySavedataInitStart(&savedata);
    saveLoadType = OSL_DIALOG_DELETE;
}

void oslDrawSaveLoad(){
	switch(sceUtilitySavedataGetStatus()) {
		case PSP_UTILITY_DIALOG_INIT:
		case PSP_UTILITY_DIALOG_VISIBLE:
			sceGuFinish();
			sceGuSync(0,0);
			sceUtilitySavedataUpdate(1);
			sceGuStart(GU_DIRECT, osl_list);
			oslSetAlpha(OSL_FX_RGBA, 0xff);
            break;
        case PSP_UTILITY_DIALOG_QUIT:
            sceUtilitySavedataShutdownStart();
            break;
		case PSP_UTILITY_DIALOG_FINISHED:
        case PSP_UTILITY_DIALOG_NONE:
			break;
	}
}


int oslGetLoadSaveStatus(){
    return sceUtilitySavedataGetStatus();
}


int oslGetSaveLoadType(){
    return saveLoadType;
}


int oslSaveLoadGetResult(){
    return savedata.base.result;
}


void oslEndSaveLoadDialog(){
    saveLoadType = OSL_DIALOG_NONE;
}
