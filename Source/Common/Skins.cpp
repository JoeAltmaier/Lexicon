// Skins.cpp - Skin loader
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-04-02  JCA  Created
 *
**/

#include "Skins.h"
#include "Resource.h"

SkinLoader::SkinLoader(const TCHAR* pFileName, const SkinResources* pSr, void* pDataRet)
	: head(NULL)
{
	// Open file

	HANDLE hSkin = CreateFile(
		pFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_SEQUENTIAL_SCAN, 
		NULL
	);

	LoadResourcesFromSkinfile(hSkin);

	BuildResourceStructure(pSr, pDataRet);

	CloseHandle(hSkin);
}

SkinLoader::~SkinLoader() {
	head.DeleteElements();
}

bool SkinLoader::LoadResourcesFromSkinfile(HANDLE hSkin) {

	SkinResourceDescriptor srd;
	DWORD cb;

	BOOL bReadOk = ReadFile(
		hSkin,
		&srd,
		sizeof(srd),
		&cb,
		NULL
	);

	while (bReadOk && cb) {
		if (srd.cb > 0x100000)
			return false;

		U8* pResource = new U8[srd.cb];

		bReadOk = ReadFile(
			hSkin,
			pResource,
			srd.cb,
			&cb,
			NULL
		);

		if (bReadOk && cb)
			head.LinkTail((new Resource(srd, pResource))->node);
		else
			return false;

		// Next resource header
		bReadOk = ReadFile(
			hSkin,
			&srd,
			sizeof(srd),
			&cb,
			NULL
		);
	}

	return true;
}


void SkinLoader::BuildResourceStructure(const SkinResources* pSr, void* pDataRet) {
	for (U32 i = 0; pSr[i].tySv != SKIN_EOF; i++) {
		const Resource& resource = Find((const U8*)pSr[i].pNameResource, pSr[i].tySv);

		switch (pSr[i].tySv) {
		case SKIN_IMAGE: {
			SkinValueImage& svImg = *(SkinValueImage*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueImage));

			if (&resource) {
				svImg.pBytes = resource.pData;
				svImg.cb = resource.srd.cb - sizeof(RECT);
				// May not be aligned - use byte copy
				memcpy(&svImg.rect, resource.pData + resource.srd.cb - sizeof(RECT), sizeof(svImg.rect));
			}
		}
					   break;

		case SKIN_ANIMATION: {
			SkinValueAnimation& svAnim = *(SkinValueAnimation*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueAnimation));

			if (&resource)
				svAnim.pBytes = resource.pData;
		}
						   break;

		case SKIN_WAVE: {
			SkinValueWave& svWave = *(SkinValueWave*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueWave));

			if (&resource) {
				svWave.pBytes = resource.pData;
				svWave.cb = resource.srd.cb;
			}
		}
					  break;

		case SKIN_DICTIONARY: {
			SkinValueDict& svDict = *(SkinValueDict*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueDict));

			if (&resource) {
				svDict.pBytes = resource.pData;
				svDict.cb = resource.srd.cb;
			}
		}
							break;

		case SKIN_TEXT: {
			SkinValueText& svText = *(SkinValueText*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueText));

			if (&resource) {
				svText.pString = (char*)resource.pData;
				svText.cb = resource.srd.cb;
			}
		}
					  break;

		case SKIN_TCHAR: {
			SkinValueTchar& svTchar = *(SkinValueTchar*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueTchar));

			if (&resource) {
				svTchar.pString = (TCHAR*)resource.pData;
				svTchar.cb = resource.srd.cb;
			}
		}
					   break;

		case SKIN_RECT: {
			SkinValueRect& svRect = *(SkinValueRect*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueRect));

			if (&resource)
				// May be unaligned - use byte copy
				memcpy(&svRect.rect, resource.pData, sizeof(svRect.rect));
		}
					  break;

		case SKIN_BOOLEAN: {
			SkinValueBoolean& svBoolean = *(SkinValueBoolean*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueBoolean));

			if (&resource)
				svBoolean.b = (BOOL)resource.pData[0];
		}
						 break;

		case SKIN_SCALAR: {
			SkinValueScalar& svScalar = *(SkinValueScalar*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueScalar));

			if (&resource)
				svScalar.u32 = *(U32*)resource.pData;
		}
						break;

		case SKIN_RGB: {
			SkinValueRgb& svRgb = *(SkinValueRgb*)pDataRet;
			pDataRet = (void*)(((U8*)pDataRet) + sizeof(SkinValueRgb));

			if (&resource)
				svRgb.color = *(COLORREF*)resource.pData;
		}
					 break;

		case SKIN_EOF:
			break;
		}
	}
}

const Resource& SkinLoader::Find(const U8* const pName, TypeSkinValue tySv) {
	Resource* pResource = head.NextElement();
	while (pResource) {
		if (strcmp(pResource->srd.aNameResource, (const char*)pName) == 0
			&& pResource->srd.tySv == tySv)
			return *pResource;

		pResource = pResource->node.NextElement();
	}

	return *(Resource*)NULL;
}
