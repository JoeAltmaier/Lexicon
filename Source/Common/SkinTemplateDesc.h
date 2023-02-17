
#ifndef __SkinTemplateDesc_h
#define __SkinTemplateDesc_h

#include "LexTypes.h"
#include "CircularList_T.h"

class SkinTemplateDesc;

typedef CircularList_T<SkinTemplateDesc> ClSt;

class SkinTemplateDesc {
public:
	ClSt node;
	char *pId;
	U32 id;
	char *pFormat;
	SkinTemplateDesc(char *_pId, U32 _id, char *_pFormat)
		:node(this), pId(_pId), id(_id), pFormat(_pFormat)
	{}
};

#endif
