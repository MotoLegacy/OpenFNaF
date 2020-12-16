#ifndef _ADHOC_H_
#define _ADHOC_H_

#define ADHOC_ERROR_WLAN				-1
#define ADHOC_ERROR_MAC					-2
#define ADHOC_ERROR_MODULES				-3
#define ADHOC_ERROR_NET_INIT			-4
#define ADHOC_ERROR_INIT				-5
#define ADHOC_ERROR_CTL_INIT			-6
#define ADHOC_ERROR_CTL_CONNECT			-7
#define ADHOC_ERROR_PDP_CREATE			-8
#define ADHOC_ERROR_MATCHING_INIT		-9
#define ADHOC_ERROR_MATCHING_CREATE		-10
#define ADHOC_ERROR_MATCHING_START		-11

#define MATCHING_JOINED 0x1 // Another PSP has joined
#define MATCHING_SELECTED 0x2 // Another PSP selected to match
#define MATCHING_REJECTED 0x4 // The request has been rejected
#define MATCHING_CANCELED 0x5 // The request has been cancelled
#define MATCHING_ACCEPTED 0x6
#define MATCHING_ESTABLISHED 0x7 // Both PSP's have agreed to connect, at this point Lumines
// closes the connection and creates a new one with just the
// two PSP's in it.
#define MATCHING_DISCONNECT 0xa // A PSP has quit, this does not include when the PSP crashes

#define ADHOC_UNINIT -1
#define ADHOC_INIT 0

#define MAX_REMOTEPSP 100

/** @defgroup AdHoc AdHoc

	Functions for adhoc.
	@{
*/

/**
Connection states
*/
enum remotePspState {
	/** PSP Disconnected */
	OSL_ADHOC_DISCONNECTED = 0,
	/** PSP Joined */
	OSL_ADHOC_JOINED,
	/** A PSP selected me */
	OSL_ADHOC_SELECTED,
	/** Connection rejected */
	OSL_ADHOC_REJECTED,
	/** Connection canceled */
	OSL_ADHOC_CANCELED,
	/** Connection accepted */
	OSL_ADHOC_ACCEPTED,
	/** Connection established */
	OSL_ADHOC_ESTABLISHED
};

/**
Structure for information on a psp
*/
struct remotePsp
{
	/** The mac address of the PSP */
    u8     macAddress[8];
	/** Nickname of the PSP */
	char   name[257];
	/** The connection state of the PSP ( remotePspState ) */
    int    connectionState;;
};

/**
Initialize the adhoc, returns 0 on succes ( < 0 on error)
*/
int oslAdhocInit(char *productID);

/**
Returns the current connection state ( remotePspState )
*/
int oslAdhocGetState();

/**
Returns the current mac address
*/
u8* oslAdhocGetMacAddress();

/**
Returns the number of remotes psp
*/
int oslAdhocGetRemotePspCount();

/**
Returns the remote psp with the given mac address
*/
struct remotePsp *oslAdhocGetPspByMacAddress(const u8 aMacAddress[6]);

/**
Returns the remote psp with the given index
*/
struct remotePsp *oslAdhocGetPspByIndex(int index);

/**
Request a connction to a remote psp
*/
int oslAdhocRequestConnection(struct remotePsp *aPsp, int timeOut, int (*requestConnectionCB)(int aPspState));

/**
Sends data to a remote psp
*/
int oslAdhocSendData( struct remotePsp *pPsp, void *data, int lenData);

/**
Receives data from a remote psp.
Returns the length of the data received (if == 0 no data received)
*/
int oslAdhocReceiveData( struct remotePsp *pPsp, void *data, int maxLen);

/**
Retuns one psp that is requesting a connection
*/
struct remotePsp *oslAdhocGetConnectionRequest();

/**
Rejects a connection request from a psp
*/
void oslAdhocRejectConnection(struct remotePsp *aPsp);

/**
Acepts a connection request from a psp
*/
void oslAdhocAcceptConnection(struct remotePsp *aPsp);

/**
Terminates the adhoc
*/
void oslAdhocTerm();

/** @} */ // end of adhoc
#endif

