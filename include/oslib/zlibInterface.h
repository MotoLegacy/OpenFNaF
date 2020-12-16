#ifndef ___ZLIBINTERFACE
#define ___ZLIBINTERFACE

// enum - Unzip Execute eXtract Return
enum {
	UZEXR_OK = 1,
	UZEXR_CANCEL = 0,
	// All errors shall be a negative value
	UZEXR_INVALIDCALLBACK = -1,
	UZEXR_INVALIDFILE = -2,
	UZEXR_FATALERROR = -3
};

// enum - UnZip CallBack Return
enum {
	UZCBR_OK,
	UZCBR_PASS,
	UZCBR_CANCEL
};

// enum UnZip CallBack id
enum {
	UZCB_FIND_FILE,
	UZCB_EXTRACT_PROGRESS
};



// Specify a pointer to the callback function to receive notification from the Unzip
void Unzip_setCallback(int (*pfuncCallback)(int nCallbackId, unsigned long ulExtractSize,
		      unsigned long ulCurrentPosition, const void *pData,
                      unsigned long ulDataSize, unsigned long ulUserData));

// Extract the file specified in the Unzip.
int Unzip_execExtract(const char *pszTargetFile, unsigned long ulUserData);


#endif //_ZLIBINTERFACE
