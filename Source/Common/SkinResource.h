// SkinResource.h - shared header file defining .skn file record format

// Skins are defined by an array of SkinResource resource descriptors

#ifndef __SkinResource_h
#define __SkinResource_h

#include "CircularList_T.h"
#include "LexTypes.h"

class SkinDefinition;
class SkinDesc;

typedef CircularList_T<SkinDesc> ClSd;

class SkinDesc {
protected:
	const char *pName;
	TypeSkinValue tysd;
	U32 cb;

public:
	ClSd node;
	SkinDesc(const char *_pName, TypeSkinValue _tysd, U32 _cb): node(this), pName(_pName), tysd(_tysd), cb(_cb) 
	{
	}

	virtual
	void SetSize(U32 _cb) { cb=_cb; }

	virtual
	BOOL Parse(SkinDefinition &sd, char *&p) { return false; }

	virtual
	BOOL Emit(HANDLE fhOut) {
		DWORD cbRet;
		SkinResourceDescriptor srd;

		strcpy_s(srd.aNameResource, pName);
		srd.tySv=tysd;
		srd.cb=cb;

		WriteFile(
			fhOut,
			&srd,
			sizeof(srd),
			&cbRet,
			NULL
		);

		return true;
	}

	virtual
	const char *Description() { return ""; }
};

#endif
