// RegKey.h - registry access class
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-12-02  JAltmaier  Created
 *
**/
#ifndef __RegKey_h
#define __RegKey_h

#include "LexTypes.h"
//#include "WDefs.h"
#include "AppDefs.h"
#include <WinReg.h>

class RegKey {
	HKEY hKeyRoot;
	HKEY hKey;
	TCHAR *pPath;
	BOOL bInitialized;
	BOOL bCreate;

public:
	enum {MAXSIZE=512};
public:
	RegKey(HKEY, TCHAR*, BOOL bCreate=false);
	~RegKey();

	BOOL BInitialized() { return bInitialized; }

	void Open();
	void Read(TCHAR **aName, U32 *aType, U8 *pDefault, BOOL bReinitialize, U8 *pRet);
	void Write(TCHAR *pName, U32 aType, U8 *pRet);

	LONG Read(TCHAR *pName, TCHAR *pRet);
	void Read(TCHAR *pName, U32 *pRet);

	void Expunge(TCHAR **aName);

	void Erase();

private:
	U32 Size(U32 type);
	U32 SizeWrite(U8 *pValue, U32 type);
};

#endif
