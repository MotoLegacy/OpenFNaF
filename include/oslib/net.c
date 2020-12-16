#include <pspkernel.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <psputility.h>
#include <psputility_netmodules.h>
#include <pspwlan.h>
#include <pspnet.h>
#include <pspnet_inet.h>
//#include <pspnet_apctl.h>		//<-- STAS: included in net.h
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspssl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "oslib.h"
#include "net.h"

static int networkIsActive = 0;			//<-- STAS: network initialization flag

int oslLoadNetModules()
{
	sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
	sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
	sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
	sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
    sceUtilityLoadNetModule(PSP_NET_MODULE_SSL);
	return 0;
}


int oslUnloadNetModules()
{
	sceUtilityUnloadNetModule(PSP_NET_MODULE_SSL);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
	sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//Public API
///////////////////////////////////////////////////////////////////////////////////////////////////
int oslIsWlanPowerOn(){
    return sceWlanDevIsPowerOn();
}


int oslIsWlanConnected(){
	union SceNetApctlInfo apctlInfo;

	if(sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &apctlInfo) == 0)
		return 1;
	else
		return 0;
}

int oslGetNetConfigs(struct oslNetConfig *result){
    int index = 0;
    netData name, ip;

    for (index=1; index<OSL_MAX_NET_CONFIGS; index++){
       if (sceUtilityCheckNetParam(index))
          break;

       sceUtilityGetNetParam(index, PSP_NETPARAM_NAME, &name);
       sceUtilityGetNetParam(index, PSP_NETPARAM_IP, &ip);

       strcpy(result[index - 1].name, name.asString);
       strcpy(result[index - 1].IP, ip.asString);
    }
    return index - 1;
}

//<-- STAS: intelligent NetTerm routine which takes care of exact oslNetInit step.
//			Note: PSP sometime reboots when we try to deinitialize things not initialized yet.
int oslNetTermEx(int step)
{
    switch(step) {
      case 0:
		sceHttpSaveSystemCookie();
      case 7:
		sceHttpsEnd();
      case 6:
		sceHttpEnd();
      case 5:
		sceSslEnd();
      case 4:
		sceNetApctlTerm();
      case 3:
		sceNetResolverTerm();
      case 2:
		sceNetInetTerm();
      case 1:
		sceNetTerm();
    }
	oslUnloadNetModules();
	networkIsActive = 0;			//<-- STAS: network is uninitialized marker
	return 0;
}


int oslIsNetActive()
{
	return networkIsActive;
}
//<-- STAS END -->

int oslNetInit()
{
	int res;

	oslLoadNetModules();
    res = sceNetInit(128*1024, 42, 4*1024, 42, 4*1024);
	if (res < 0)
		return OSL_NET_ERROR_NET;

	res = sceNetInetInit();
	if (res < 0) {
		oslNetTermEx(1);
		return OSL_NET_ERROR_INET;
	}

	res = sceNetResolverInit();
	if (res < 0)
	{
		oslNetTermEx(2);
		return OSL_NET_ERROR_RESOLVER;
	}

    res = sceNetApctlInit(0x10000, 48);
	if (res < 0)
	{
		oslNetTermEx(3);
		return OSL_NET_ERROR_APCTL;
	}

	res = sceSslInit(0x28000);
	if (res < 0)
	{
		oslNetTermEx(4);
		return OSL_NET_ERROR_SSL;
	}

	res = sceHttpInit(0x25800);
	if (res < 0)
	{
		oslNetTermEx(5);
		return OSL_NET_ERROR_HTTP;
	}

	res = sceHttpsInit(0, 0, 0, 0);
	if (res < 0)
	{
		oslNetTermEx(6);
		return OSL_NET_ERROR_HTTPS;
	}

	res = sceHttpsLoadDefaultCert(0, 0);
	if (res < 0)
	{
		oslNetTermEx(7);
		return OSL_NET_ERROR_CERT;
	}

	res = sceHttpLoadSystemCookie();
	if (res < 0)
	{
		oslNetTermEx(7);
		return OSL_NET_ERROR_COOKIE;
	}
	networkIsActive = 1;			//<-- STAS: network is initialized marker
	return 0;
}


int oslNetTerm()
{
	return oslNetTermEx(0);			//<-- STAS: full deinitialization
}

int oslGetIPaddress(char *IPaddress){
	union SceNetApctlInfo apctlInfo;
    strcpy(IPaddress, "");
    if (sceNetApctlGetInfo(PSP_NET_APCTL_INFO_IP, &apctlInfo))
        return OSL_ERR_APCTL_GETINFO;
	strcpy(IPaddress, apctlInfo.ip);
    return 0;
}


int oslConnectToAP(int config, int timeout,
                   int (*apctlCallback)(int state)){
	int err = 0;
	int stateLast = -1;

    if (!oslIsWlanPowerOn())
        return OSL_ERR_WLAN_OFF;

	err = sceNetApctlConnect(config);
	if (err)
		return OSL_ERR_APCTL_CONNECT;

    time_t startTime;
    time_t currTime;
    time(&startTime);
	while (!osl_quit){
        //Check timeout:
        time(&currTime);
        if (currTime - startTime >= timeout){
            if (apctlCallback != NULL)
                (*apctlCallback)(OSL_ERR_APCTL_TIMEOUT);
            oslDisconnectFromAP();
            err = OSL_ERR_APCTL_TIMEOUT;
            break;
        }
		int state;
		err = sceNetApctlGetState(&state);
		if (err){
            if (apctlCallback != NULL)
                (*apctlCallback)(OSL_ERR_APCTL_GETSTATE);
            oslDisconnectFromAP();
            err = OSL_ERR_APCTL_GETSTATE;
            break;
        }
		if (state > stateLast){
			stateLast = state;
            if (apctlCallback != NULL){
                if ((*apctlCallback)(state)){
                    err = OSL_USER_ABORTED;
                    break;
                }
            }
        }
		if (state == PSP_NET_APCTL_STATE_GOT_IP)
			break;  // connected with static IP
		sceKernelDelayThread(50*1000);
	}

	return err;
}

int oslDisconnectFromAP(){
    sceNetApctlDisconnect();
    return 0;
}

int oslGetAPState(){
    int state;
    int err = sceNetApctlGetState(&state);
    if (err)
        return OSL_ERR_APCTL_GETSTATE;
    return state;
}


//The following functions are copied from PGE LUA source code:
//http://svn.luaplayer.org/pge/pgeNet.c
int oslResolveAddress(char *address, char *resolvedIP){
	struct hostent *host;

	if(!(host = gethostbyname(address)))
		return OSL_ERR_RESOLVER_RESOLVING;

	sprintf(resolvedIP, inet_ntoa(*((struct in_addr *)host->h_addr)));
    return 0;
}


int oslNetGetFile(const char *url, const char *filepath)
{
	int template, connection, request, ret, status, dataend, fd, byteswritten;
	//SceULong64 contentsize;
	unsigned char readbuffer[8192];
/*										//<-- STAS: HTTP library was already initialized (see oslNetInit()) !
	ret = sceHttpInit(20000);

	if(ret < 0)
		return OSL_ERR_HTTP_INIT;
*/										//<-- STAS END -->
	template = sceHttpCreateTemplate("OSL-agent/0.0.1 libhttp/1.0.0", 1, 1);
	if(template < 0)
		return OSL_ERR_HTTP_TEMPLATE;

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	if(ret < 0)
		return OSL_ERR_HTTP_TIMEOUT;

	ret = sceHttpSetRecvTimeOut(template, 60000000);
	if(ret < 0)
		return OSL_ERR_HTTP_TIMEOUT;

	ret = sceHttpSetSendTimeOut(template, 60000000);
	if(ret < 0)
		return OSL_ERR_HTTP_TIMEOUT;

	connection = sceHttpCreateConnectionWithURL(template, url, 0);
	if(connection < 0)
		return OSL_ERR_HTTP_CONNECT;

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);
	if(request < 0)
		return OSL_ERR_HTTP_REQUEST;

	ret = sceHttpSendRequest(request, 0, 0);
	if(ret < 0)
		return OSL_ERR_HTTP_REQUEST;

	ret = sceHttpGetStatusCode(request, &status);
	if(ret < 0)
		return OSL_ERR_HTTP_GENERIC;

	if(status != 200)
		return 0;
	
	/* Strangelove fix
	ret = sceHttpGetContentLength(request, &contentsize);
	if(ret < 0)
		return OSL_ERR_HTTP_GENERIC;
	*/

	dataend = 0;
	byteswritten = 0;

	// Strangelove fix - Added PSP_O_TRUNC
	fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_TRUNC | PSP_O_CREAT, 0777);

	while(dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);
		if(ret < 0)
		{
			sceIoWrite(fd, filepath, 4);
			sceIoClose(fd);
			return OSL_ERR_HTTP_GENERIC;
		}

		if(ret == 0)
			dataend = 1;

		if(ret > 0)
		{
			byteswritten += ret;
			sceIoWrite(fd, readbuffer, ret);
		}
	}

	sceIoClose(fd);
	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);
	sceHttpDeleteTemplate(template);
//	sceHttpEnd();						//<-- STAS: This should be done in oslNetTerm() only !

	return 0;
}

int oslNetPostForm(const char *url, char *data, char *response, unsigned int responsesize)
{
	int template, connection, request, ret, status;
/*										//<-- STAS: HTTP library was already initialized (see oslNetInit()) !
	ret = sceHttpInit(20000);
	if(ret < 0)
		return OSL_ERR_HTTP_INIT;
*/										//<-- STAS END -->
	template = sceHttpCreateTemplate("OSL-agent/0.0.1 libhttp/1.0.0", 1, 1);
	if(template < 0)
	{
		sceHttpEnd();
		return OSL_ERR_HTTP_TEMPLATE;
	}

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	if(ret < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetRecvTimeOut(template, 60000000);

	if(ret < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetSendTimeOut(template, 60000000);

	if(ret < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	connection = sceHttpCreateConnectionWithURL(template, url, 0);

	if(connection < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	sceHttpEnableKeepAlive(connection);
	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_POST, (char*)url, strlen(data));
	if(request < 0)
	{
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_REQUEST;
	}

	sceHttpAddExtraHeader(request, "Content-Type", "application/x-www-form-urlencoded", 0);

	ret = sceHttpSendRequest(request, data, strlen(data));
	if(ret < 0)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_REQUEST;
	}

	ret = sceHttpGetStatusCode(request, &status);
	if(ret < 0)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_GENERIC;
	}

	if(status != 200)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_GENERIC;
	}

	if(response != NULL && responsesize > 0)
	{
		ret = sceHttpReadData(request, response, responsesize);
		if(ret < 0)
		{
			sceHttpDeleteRequest(request);
			sceHttpDeleteConnection(connection);
			sceHttpDeleteTemplate(template);
			sceHttpEnd();
			return OSL_ERR_HTTP_GENERIC;
		}
	}

	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);

	sceHttpDeleteTemplate(template);
//	sceHttpEnd();						//<-- STAS: This should be done in oslNetTerm() only !

	return 1;
}

int oslNetSocketCreate(void)
{
	int sock;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if(sock < 0)
		return -1;

	return sock;
}

int oslNetSocketAccept(int socket)
{
	int sock;

	struct sockaddr_in client;

	socklen_t size;

	sock = accept(socket, (struct sockaddr *) &client, &size);

	if(sock < 0)
		return -1;

	return sock;
}

int oslNetSocketBind(int socket, unsigned short port)
{
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int error;

	error = bind(socket, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	if(error < 0)
		return 0;

	return 1;
}

int oslNetSocketListen(int socket, unsigned int maxconn)
{
	int error;

	error = listen(socket, maxconn);

	if(error < 0)
		return 0;

	return 1;
}

int oslNetSocketConnect(int socket, char *ip, unsigned short port)
{
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(ip, &(addr.sin_addr));
	memset(&(addr.sin_zero), '\0', 8);

	if(connect(socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return 0;

	return 1;
}

int oslNetSocketSend(int socket, const void *data, int length)
{
	int bytessent = send(socket, data, length, 0);

	return bytessent;
}

int oslNetSocketReceive(int socket, void *data, int length)
{
	int bytesrecv = recv(socket, data, length, 0);

	return bytesrecv;
}

void oslNetSocketClose(int socket)
{
	close(socket);
}

void oslNetSocketSetClear(fd_set *set)
{
	FD_ZERO(set);
}

void oslNetSocketSetAdd(int socket, fd_set *set)
{
	FD_SET(socket, set);
}

void oslNetSocketSetRemove(int socket, fd_set *set)
{
	FD_CLR(socket, set);
}

int oslNetSocketSetIsMember(int socket, fd_set *set)
{
	if(FD_ISSET(socket, set))
		return 1;

	return 0 ;
}

int oslNetSocketSetSelect(unsigned int maxsockets, fd_set *set)
{
	// TODO: Add argument manipulation of timeout etc.
	int numsockets;
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	numsockets = select(maxsockets, set, NULL, NULL, &tv);

	if(numsockets < 0)
		return -1;

	return numsockets;
}
