#include "oslib.h"

int (*readButtonsFunctions)(SceCtrlData *pad_data, int count);

OSL_CONTROLLER osl_pad;
OSL_REMOTECONTROLLER osl_remote;

OSL_CONTROLLER *oslReadKeys()
{
	SceCtrlData ctl;
	SceCtrlData ctlHome;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(1);

    if (readButtonsFunctions != NULL)
        readButtonsFunctions(&ctl, 1);
    else
    	sceCtrlPeekBufferPositive( &ctl, 1 );

    sceCtrlPeekBufferPositive( &ctlHome, 1 ); //Always in user mode, else sceCtrlPeekBufferPositive really reads the HOME button
    if (ctlHome.Buttons & PSP_CTRL_HOME){
        memset(&osl_keys->held, 0, sizeof(OSL_KEYLIST));
        memset(&osl_keys->pressed, 0, sizeof(OSL_KEYLIST));
        memset(&osl_keys->released, 0, sizeof(OSL_KEYLIST));
        memset(&osl_keys->lastHeld, 0, sizeof(OSL_KEYLIST));
		return osl_keys;
	}

	//Récupère les valeurs du stick
    if (osl_keys->holdAffectsAnalog && (ctl.Buttons & OSL_KEYMASK_HOLD)){
        osl_keys->analogX = 0;
        osl_keys->analogY = 0;
    }else{
        osl_keys->analogX = (signed)ctl.Lx-128;
        osl_keys->analogY = (signed)ctl.Ly-128;
        //if (osl_keys->analogX || osl_keys->analogY)
        //    scePowerTick(0); //Keep display on also with analog
    }

	if (osl_keys->analogToDPadSensivity > 0)		{
		if (osl_keys->analogY >= osl_keys->analogToDPadSensivity){
			ctl.Buttons |= OSL_KEYMASK_DOWN;
			scePowerTick(0); 
		}
		if (osl_keys->analogY <= -osl_keys->analogToDPadSensivity){
			ctl.Buttons |= OSL_KEYMASK_UP;
			scePowerTick(0); 
		}
		if (osl_keys->analogX >= osl_keys->analogToDPadSensivity){
			ctl.Buttons |= OSL_KEYMASK_RIGHT;
			scePowerTick(0); 
		}		
		if (osl_keys->analogX <= -osl_keys->analogToDPadSensivity){
			ctl.Buttons |= OSL_KEYMASK_LEFT;
			scePowerTick(0); 
		}
	}

	/*
		CODE AUTO-REPEAT
	*/
	if (osl_keys->autoRepeatInterval > 0)			{					//Auto repeat activé?
		//Si ça change -> compteur à zéro
		if (osl_keys->lastHeld.value != ctl.Buttons)
			osl_keys->autoRepeatCounter=0;
		else			{
			osl_keys->autoRepeatCounter++;
			if (osl_keys->autoRepeatCounter >= osl_keys->autoRepeatInit)			{
				//AutoRepeat déclenché -> déclenchement toutes les autoRepeatInterval coups
				if ((osl_keys->autoRepeatCounter - osl_keys->autoRepeatInit) % osl_keys->autoRepeatInterval == 0)
					osl_keys->lastHeld.value &= ~osl_keys->autoRepeatMask;
			}
		}
	}

	osl_keys->pressed.value = ~osl_keys->lastHeld.value & ctl.Buttons;
	osl_keys->released.value = osl_keys->lastHeld.value & ~ctl.Buttons;

	osl_keys->held.value = ctl.Buttons;
	osl_keys->lastHeld.value = ctl.Buttons;

	return osl_keys;
}

OSL_REMOTECONTROLLER *oslReadRemoteKeys()
{
    u32 remote;
	if (!sceHprmIsRemoteExist()){
        memset(&osl_remotekeys->held, 0, sizeof(OSL_REMOTEKEYLIST));
        memset(&osl_remotekeys->pressed, 0, sizeof(OSL_REMOTEKEYLIST));
        memset(&osl_remotekeys->released, 0, sizeof(OSL_REMOTEKEYLIST));
        memset(&osl_remotekeys->lastHeld, 0, sizeof(OSL_REMOTEKEYLIST));
		return osl_remotekeys;
	}

	sceHprmPeekCurrentKey( &remote );

	/*
		CODE AUTO-REPEAT
	*/
	if (osl_remotekeys->autoRepeatInterval > 0)			{					//Auto repeat activé?
		//Si ça change -> compteur à zéro
		if (osl_remotekeys->lastHeld.value != remote)
			osl_remotekeys->autoRepeatCounter=0;
		else			{
			osl_remotekeys->autoRepeatCounter++;
			if (osl_remotekeys->autoRepeatCounter >= osl_remotekeys->autoRepeatInit)			{
				//AutoRepeat déclenché -> déclenchement toutes les autoRepeatInterval coups
				if ((osl_remotekeys->autoRepeatCounter - osl_remotekeys->autoRepeatInit) % osl_remotekeys->autoRepeatInterval == 0)
					osl_remotekeys->lastHeld.value &= ~osl_remotekeys->autoRepeatMask;
			}
		}
	}

	osl_remotekeys->pressed.value = ~osl_remotekeys->lastHeld.value & remote;
	osl_remotekeys->released.value = osl_remotekeys->lastHeld.value & ~remote;

	osl_remotekeys->held.value = remote;
	osl_remotekeys->lastHeld.value = remote;

	return osl_remotekeys;
}

void oslFlushRemoteKey()
{
	oslReadRemoteKeys();
}


int oslIsRemoteExist(){
	return sceHprmIsRemoteExist();
}

void oslFlushKey()
{
	oslReadKeys();
}

int oslKbhit()
{
	OSL_CONTROLLER *keys;
	int i, j, k;
	keys = oslReadKeys();
	k = 0;
	j = 0;
	for (i=0;i<24;i++)		{
		if (keys->pressed.value & (1<<i))			{
			k |= (i+1)<<j;
			j+=8;
		}
	}
	return k;
}

int oslWaitKey()
{
	int i;
	while(!(i=oslKbhit()) && !osl_quit)
		oslWaitVSync();
	return i;
}


int oslSetReadKeysFunction(int (*sceCtrlReadBufferPositive)(SceCtrlData *pad_data, int count))
{
    readButtonsFunctions = sceCtrlReadBufferPositive;
    return 0;
}


int oslUnsetReadKeysFunction(){
    readButtonsFunctions = NULL;
    return 0;
}


int oslSetHoldForAnalog(int holdForAnalog){
    osl_keys->holdAffectsAnalog = holdForAnalog;
    return 0;
}
