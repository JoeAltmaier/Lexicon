// Skins.h - Lexicon skin definition
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-04-02  JCA  Created
 *
**/

#ifndef __Skins_h
#define __Skins_h

#include "LexTypes.h"
#include <Windows.h>
#include "CircularList_T.h"

// Skins are defined by an array of SkinResource resource descriptors

typedef enum { SKIN_IMAGE = 1, SKIN_ANIMATION = 2, SKIN_WAVE = 3, SKIN_DICTIONARY = 4, SKIN_TEXT = 5, SKIN_TCHAR = 6, SKIN_RECT = 7, SKIN_BOOLEAN = 8, SKIN_SCALAR = 9, SKIN_RGB = 10, SKIN_EOF = 0 } TypeSkinValue;

typedef struct SkinResourceDescriptor {
	char aNameResource[32];
	TypeSkinValue tySv;
	U32 cb;
} SkinResourceDescriptor;

typedef struct SkinResources {
	char* pNameResource;
	TypeSkinValue tySv;
	char* pDescription;
} SkinResources;


// Values are returned in a structured buffer.  Fields are one of the following.

// Types are related to value returned by each SKIN_ type

typedef struct SkinValueImage {
	void* pBytes;
	UINT32 cb;
	CRect rect;	// Really image size in right and bottom
} SkinValueImage;

typedef struct SkinValueAnimation {
	void* pBytes;
} SkinValueAnimation;

typedef struct SkinValueWave {
	void* pBytes;
	UINT32 cb;
} SkinValueWave;

typedef struct SkinValueDict {
	void* pBytes;
	UINT32 cb;
} SkinValueDict;

typedef struct SkinValueText {
	char* pString;
	UINT32 cb;
} SkinValueText;

typedef struct SkinValueRect {
	CRect rect;
} SkinValueRect;

typedef struct SkinValueBoolean {
	BOOL b;
} SkinValueBoolean;

typedef struct SkinValueScalar {
	UINT32 u32;
} SkinValueScalar;

typedef struct SkinValueTchar {
	TCHAR* pString;
	UINT32 cb;
} SkinValueTchar;

typedef struct SkinValueRgb {
	COLORREF color;
} SkinValueRgb;




class Resource;

typedef CircularList_T<Resource> ClRes;

class Resource {
public:
	SkinResourceDescriptor srd;
	U8* pData;
	ClRes node;

public:
	Resource(const SkinResourceDescriptor& _srd, U8* _pData)
		: node(this), srd(_srd), pData(_pData) {}
	~Resource() { if (pData) delete pData; }
};


class SkinLoader {
	ClRes head;
public:
	SkinLoader() : head(NULL) {}
	SkinLoader(const TCHAR* pFileName, const SkinResources*, void* pDataRet);
	~SkinLoader();

	bool LoadResourcesFromSkinfile(HANDLE);
	void BuildResourceStructure(const SkinResources* pSr, void* pDataRet);
	const Resource& Find(const U8* const, TypeSkinValue);

};

#endif
