//PSP adhoc functions.
// based on pspZorba's sample
// www.pspZorba.com
#include <pspkernel.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_adhoc.h>
#include <pspnet_adhocctl.h>
#include <pspnet_adhocmatching.h>
#include <psputility_sysparam.h>
#include <string.h>

#include "pspadhoc.h"

static int state = ADHOC_UNINIT;
static u8 myMacAddress[8];
static struct productStruct product;
static int matchingHD = 0;
static int pdpHD = 0;
static int port = 0;

static struct remotePsp allRemotePSP[MAX_REMOTEPSP];
static int allRemotePSPCount;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal functions:
////////////////////////////////////////////////////////////////////////////////////////////////////////
int _getMacAddress()
{
    return sceWlanGetEtherAddr(myMacAddress);
}

int _loadModule(const char *prx, int mode)
{
	return pspSdkLoadStartModule(prx, mode);
}

int _loadModules()
{
    int ret;
    ret = _loadModule("flash0:/kd/ifhandle.prx", PSP_MEMORY_PARTITION_KERNEL);
    if(ret < 0) return -1;
    ret = _loadModule("flash0:/kd/memab.prx", PSP_MEMORY_PARTITION_KERNEL);
    if(ret < 0) return -1;
    ret = _loadModule("flash0:/kd/pspnet_adhoc_auth.prx", PSP_MEMORY_PARTITION_KERNEL);
    if(ret < 0) return -1;
    ret = _loadModule("flash0:/kd/pspnet.prx", PSP_MEMORY_PARTITION_USER);
    if(ret < 0) return -1;
    ret = _loadModule("flash0:/kd/pspnet_adhoc.prx", PSP_MEMORY_PARTITION_USER);
    if(ret < 0) return -1;
    ret = _loadModule("flash0:/kd/pspnet_adhocctl.prx", PSP_MEMORY_PARTITION_USER);
    if(ret < 0) return -1;
    ret = _loadModule("flash0:/kd/pspnet_adhoc_matching.prx", PSP_MEMORY_PARTITION_USER);
    if(ret < 0) return -1;
    return 0;
}

int _libNetInit()
{
    return sceNetInit(0x20000, 0x20 , 0x1000, 0x20, 0x1000);
    //return sceNetInit(128*1024, 42, 4*1024, 42, 4*1024);
}

int _libAdhocInit()
{
    return sceNetAdhocInit();
}

int _libAdhocctlInit()
{
    return sceNetAdhocctlInit(0x2000, 0x20, &product);
}

int _adhocctlConnect()
{
    return sceNetAdhocctlConnect(NULL);
}


void _adhocctlState()
{
    int encore = 1, s;
    while( encore)
    {
        sceNetAdhocctlGetState(&s);
        if (s == 1)
        {
            encore=0; /* connected */
        }
        else
        {
            sceKernelDelayThread(50 * 1000); // 50ms
        }
    }
}


int _pdpCreate()
{
    pdpHD = sceNetAdhocPdpCreate(myMacAddress, port, 0x400, 0);
	return pdpHD;
}


int _matchingInit()
{
    return sceNetAdhocMatchingInit( 0x20000);
}


void _matchingCB(int unk1, int event, unsigned char *macSource, int size, void *data);
int _createMatching()
{
    matchingHD = sceNetAdhocMatchingCreate( 3,0xa,0x22b,0x800, 0x2dc6c0, 0x5b8d80,3,0x7a120, _matchingCB);
	return matchingHD;
}


int _startMatching()
{
    char name[50] = "";
    sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME, name, 50);

    return sceNetAdhocMatchingStart(matchingHD, 0x10, 0x2000, 0x10, 0x2000, strlen(name), name);
}


struct remotePsp *_findByMacAddress(const u8 aMacAddress[6])
{
    int i = 0;
	for( i=0; i<allRemotePSPCount; i++)
	{
	    if(memcmp(aMacAddress, allRemotePSP[i].macAddress,6 *sizeof(u8)) == 0)
            return &allRemotePSP[i];
	}
    return NULL;
}

struct remotePsp *_findByState(int remotePspState)
{
    int i = 0;
	for( i=0; i<allRemotePSPCount; i++)
	{
		if(allRemotePSP[i].connectionState == remotePspState)
            return &allRemotePSP[i];
	}
    return NULL;
}


int _removeByMacAddress(const u8 aMacAddress[6])
{
    int i = 0;
	int found = 0;
	for( i=0; i<allRemotePSPCount; i++)
	{
	    if(memcmp(aMacAddress, allRemotePSP[i].macAddress,6 *sizeof(u8)) == 0)
		{
			found = 1;
			allRemotePSPCount--;
		}
		if (found)
			allRemotePSP[i] = allRemotePSP[i + 1];
	}
	return found;
}


void _matchingCB(int unk1, int event, unsigned char *macSource, int size, void *data)
{
    struct remotePsp *pPsp = _findByMacAddress( (u8*) macSource);

    switch(event)
    {
        case MATCHING_JOINED: //A psp has joined
            if(pPsp == NULL)
            { //it's a new PSP, we add it to the list
                char buffer[42];
                memcpy(buffer,data,42);
                buffer[41]=0;
                struct remotePsp newPsp;
                memcpy(newPsp.macAddress, macSource, 8*sizeof(u8));
                strcpy(newPsp.name, buffer);
                newPsp.connectionState = OSL_ADHOC_JOINED;
                allRemotePSP[allRemotePSPCount++] = newPsp;
            }
            break;
        case MATCHING_DISCONNECT: //A psp has given up
			if(pPsp != NULL)
				_removeByMacAddress(pPsp->macAddress);
            break;
        case MATCHING_SELECTED: // A PSP has selected me
            if(pPsp != NULL)
            {
                pPsp->connectionState = OSL_ADHOC_SELECTED;
            }
            break;
        case MATCHING_REJECTED:
            if(pPsp != NULL)
            {
                pPsp->connectionState = OSL_ADHOC_REJECTED;
            }
			break;
        case MATCHING_CANCELED:
            if(pPsp != NULL)
            {
                pPsp->connectionState = OSL_ADHOC_CANCELED;
            }
            break;
        case MATCHING_ESTABLISHED:
            if(pPsp != NULL)
            {
                pPsp->connectionState = OSL_ADHOC_ESTABLISHED;
            }
            break;
        case MATCHING_ACCEPTED:  //connection accepted
            if(pPsp != NULL)
            {
                pPsp->connectionState = OSL_ADHOC_ACCEPTED;
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions:
////////////////////////////////////////////////////////////////////////////////////////////////////////
int oslAdhocInit(char *productID)
{
    if( state != ADHOC_UNINIT) return 0;

    memset(myMacAddress, 0, 8*sizeof(u8) );
    strncpy(product.product, productID, 9);
    product.unknown = 0;
    matchingHD=-1;
    pdpHD=-1;
    port = 0x309;
    allRemotePSPCount = 0;

    if (sceWlanDevIsPowerOn() != 1)
        return ADHOC_ERROR_WLAN;

    if (_getMacAddress() < 0)
        return ADHOC_ERROR_MAC;

	_loadModules();
    /*if (_loadModules())
        return ADHOC_ERROR_MODULES;*/

    if (_libNetInit() < 0)
        return ADHOC_ERROR_NET_INIT;

    if (_libAdhocInit() < 0)
        return ADHOC_ERROR_INIT;

    if (_libAdhocctlInit() < 0)
        return ADHOC_ERROR_CTL_INIT;

    if (_adhocctlConnect())
        return ADHOC_ERROR_CTL_CONNECT;

    _adhocctlState();

    if (_pdpCreate() < 0)
        return ADHOC_ERROR_PDP_CREATE;

    if (_matchingInit() < 0)
        return ADHOC_ERROR_MATCHING_INIT;

    if (_createMatching() < 0)
        return ADHOC_ERROR_MATCHING_CREATE;

    if (_startMatching() < 0)
        return ADHOC_ERROR_MATCHING_START;

    state = ADHOC_INIT;
    return 0;
}

u8* oslAdhocGetMacAddress()
{
    return myMacAddress;
}

int oslAdhocGetRemotePspCount()
{
	return allRemotePSPCount;
}

struct remotePsp *oslAdhocGetPspByMacAddress(const u8 aMacAddress[6])
{
	return _findByMacAddress(aMacAddress);
}

struct remotePsp *oslAdhocFindRemotePspByMacAddress(const u8 aMacAddress[6])
{
	return _findByMacAddress(aMacAddress);
}

struct remotePsp *oslAdhocGetPspByIndex(int index)
{
	if (index < allRemotePSPCount)
		return &allRemotePSP[index];
	else
		return NULL;
}

int oslAdhocRequestConnection(struct remotePsp *aPsp, int timeOut, int (*requestConnectionCB)(int aPspState))
{
	int ret = sceNetAdhocMatchingSelectTarget(matchingHD, (unsigned char *) aPsp->macAddress, 0, 0);
    if( ret <0) return ret;

	int quit = 0;
	time_t startTime;
	time_t currentTime;
	sceKernelLibcTime(&startTime);

    while(!quit)
    {
		if (aPsp == NULL)
			return -1;

		if (requestConnectionCB != NULL)
			quit = requestConnectionCB(aPsp->connectionState);

        //connection accepted
		if((aPsp->connectionState == OSL_ADHOC_ACCEPTED)||(aPsp->connectionState == OSL_ADHOC_ESTABLISHED))  return aPsp->connectionState;

        //connection not accepted
		if(aPsp->connectionState == OSL_ADHOC_REJECTED)
        {
			aPsp->connectionState = OSL_ADHOC_JOINED; //reset the state flag
			return aPsp->connectionState;
        }
		sceKernelLibcTime(&currentTime);
		if (timeOut > 0 && currentTime - startTime >= timeOut)
			break;
		sceKernelDelayThread(1000 * 1000); // 1 sec
    }
    return -1;
}

int oslAdhocSendData( struct remotePsp *pPsp, void *data, int lenData)
{
	return sceNetAdhocPdpSend( pdpHD, pPsp->macAddress,  port, data, lenData, 0, 0);
}


int oslAdhocReceiveData( struct remotePsp *pPsp, void *data, int maxLen)
{
    pdpStatStruct aStat;
    aStat.next = NULL;
    int sizeStat = sizeof(pdpStatStruct);
    unsigned int sizeData = maxLen;

    int ret = sceNetAdhocGetPdpStat(&sizeStat, &aStat);
    if(ret<0) return ret;

	if(aStat.rcvdData > 0)
	{
		//there are data to be read
		//sizeData = ( maxLen<aStat.rcvdData)?maxLen:aStat.rcvdData; //MIN
		//if the size isn't big enough there is an error ....
		ret = sceNetAdhocPdpRecv(pdpHD, pPsp->macAddress, &(aStat.port), data, &sizeData, 0, 0);
        if (ret < 0)
            return ret;
        return aStat.rcvdData;
	}
	else
	{
		return 0;
	}
}


struct remotePsp *oslAdhocGetConnectionRequest()
{
    return _findByState(OSL_ADHOC_SELECTED);
}

void oslAdhocRejectConnection(struct remotePsp *aPsp)
{
	sceNetAdhocMatchingCancelTarget(matchingHD, (unsigned char *) aPsp->macAddress);
	aPsp->connectionState = OSL_ADHOC_JOINED;
}

void oslAdhocAcceptConnection(struct remotePsp *aPsp)
{
	sceNetAdhocMatchingSelectTarget(matchingHD, (unsigned char *) aPsp->macAddress, 0, 0);
}

void oslAdhocTerm()
{
    if(state != ADHOC_INIT) return;

	sceNetAdhocctlDisconnect();
    sceNetAdhocPdpDelete(pdpHD, 0);
    sceNetAdhocMatchingStop(matchingHD);
    sceNetAdhocMatchingDelete( matchingHD);
    sceNetAdhocMatchingTerm();
    sceNetAdhocctlTerm();
    sceNetAdhocTerm();
    sceNetTerm();

    memset(myMacAddress, 0, 8*sizeof(u8));
	matchingHD = 0;
	pdpHD = 0;
	port = 0;
    state = ADHOC_UNINIT;
}


int oslAdhocGetState()
{
	return state;
}
