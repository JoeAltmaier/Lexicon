 // RegKey.h - registry access class
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-12-02  JAltmaier  Created
 *
**/

#include "RegKey.h"


RegKey::RegKey(HKEY _hKeyRoot, TCHAR *_pPath, BOOL _bCreate)
: hKeyRoot(_hKeyRoot), pPath(_pPath), bCreate(_bCreate)
{
	Open();
}

void RegKey::Open() {
	if (bCreate) {
		DWORD disp;
		RegCreateKeyEx( 
			hKeyRoot,
			pPath,
			0, 
			TEXT(""), 
			REG_OPTION_NON_VOLATILE, 
			KEY_ALL_ACCESS, 
			NULL, 
			&hKey, 
			&disp 
			); 

	} else {
			RegOpenKeyEx( 
			hKeyRoot,
			pPath,
			0,
			KEY_QUERY_VALUE,
			&hKey
			);
	}

}

RegKey::~RegKey() {
	if (hKey != NULL)
		RegCloseKey(hKey);
	hKey=NULL;
}

void RegKey::Read(TCHAR **aName, U32 *aType, U8 *pDefault, BOOL bReinitialize, U8 *pRet) {
	U32 size=0;
	U32 sizeWrite=0;
	DWORD cb;

	if (bReinitialize) {
		Expunge(aName);

		Open();
	}

	for (U32 iValue=0; aName[iValue]; iValue++) {
		size=Size(aType[iValue]);
		sizeWrite=SizeWrite(pDefault, aType[iValue]);

		if (bReinitialize)
			// Write default value
			RegSetValueEx( 
				hKey, 
				aName[iValue], 
				0, 
				aType[iValue], 
				pDefault, 
				sizeWrite
				); 

		DWORD type;

		cb=size; // cb is both input and output parameter

		// Read into pRet
		if (
			RegQueryValueEx(
				hKey, 
				aName[iValue], 
				NULL, 
				&type, 
				pRet, 
				&cb
				) != OK) 
		{
			// Write default value
			RegSetValueEx( 
				hKey, 
				aName[iValue], 
				0, 
				aType[iValue], 
				pDefault, 
				sizeWrite
				); 

			memcpy(pRet, pDefault, sizeWrite);
		}

		pDefault += size;
		pRet += size;
	}

	bInitialized=true;
}


void RegKey::Write(TCHAR *pName, U32 type, U8 *pData) {
	RegSetValueEx( 
		hKey, 
		pName, 
		NULL, 
		type, 
		pData, 
		SizeWrite(pData, type)
		); 
}

LONG RegKey::Read(TCHAR *pName, TCHAR *pRet) {
	*pRet=NULL;

	DWORD type=REG_SZ;
	DWORD cb=Size(type);
	return RegQueryValueEx(
		hKey, 
		pName, 
		NULL, 
		&type, 
		(U8*)pRet, 
		&cb
		);
}

void RegKey::Read(TCHAR *pName, U32 *pRet) {
	*pRet=0;

	DWORD type=REG_DWORD;
	DWORD cb=Size(type);
	RegQueryValueEx(
		hKey, 
		pName, 
		NULL, 
		&type, 
		(U8*)pRet, 
		&cb
		);
}

U32 RegKey::SizeWrite(U8 *pValue, U32 type) {
	U32 size;

	switch (type) {
	case REG_DWORD:
		size=sizeof(DWORD);
		break;
	case REG_SZ:
		size=(lstrlen((TCHAR*)pValue)+1) * sizeof(TCHAR);
		break;
	default:
		size=0;
		break;
	}

	return size;
}

U32 RegKey::Size(U32 type) {
	U32 size;

	switch (type) {
	case REG_DWORD:
		size=sizeof(DWORD);
		break;
	case REG_SZ:
		size=MAXSIZE * sizeof(TCHAR);
		break;
	default:
		size=0;
		break;
	}

	return size;
}

void RegKey::Expunge(TCHAR **aName) {
	for (U32 iValue=0; aName[iValue]; iValue++)
		RegDeleteValue(hKey, aName[iValue]);

	RegCloseKey(hKey);
	RegDeleteKey(hKeyRoot, pPath);
	hKey=NULL;
}

void RegKey::Erase() {
	U32 iKey=0;
	TCHAR aName[256];
	DWORD cbName=256;
	while (RegEnumKeyEx(hKey, iKey, aName, &cbName, NULL,NULL,NULL,NULL) == 0) {
		RegDeleteValue(hKey, aName);
		iKey++;
	}

	RegCloseKey(hKey);
	RegDeleteKey(hKeyRoot, pPath);
	hKey=NULL;
}
