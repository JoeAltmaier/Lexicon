// Decompress.cpp - expand my special compressed dictionary format to plaintext
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-02-02  JCA  Created
 *
**/

/*
	This handles three kinds of sequences:
	abc.		append abc to prefix
	-			pop 1 char from prefix
	xyz\r		output prefix and xyz and \r

	Special codes for common sequences:
	--			=
	---			_
	ing\r		\001
	al\r		\002
	ch\r		\003
	ed\r		\004
	le\r		\005
	ly\r		\006
	us\r		\007
	er\r		\010
	es\r		\011
	ie\r		\013
	ng\r		\014
	a\r			\016
	d\r			\017
	e\r			\020
	h\r			\021
	l\r			\022
	n\r			\023
	r\r			\024
	s\r			\025
	t\r			\026
	y\r			\027

	and any character with 0x80 set means the next character is '.'
*/

#include "LexTypes.h"
#include "string.h"

U8 prefix[32];
U32 sPrefix=0;
U8 word[32];
U32 sWord=0;

U8 *pPost=NULL;
U32 sPost=0;
U32 sPostMax=0;
extern void Post(U8 *, U32);
extern void PostWord(U8 *);
void PostPrefix();


void Decompress(U8 *p, U32 cb, U8 **pPRet, U32 *pCbRet) {
	pPost=NULL;
	sPost=0;
	sPostMax=0;
	sPrefix=0;
	sWord=0;

	U8 *pEnd=p+cb;

	while (p < pEnd) {
		switch (*p) {
		case '.':
			// append word to prefix
			PostPrefix();
			break;

		case '-':
			sPrefix--;
			break;

		case '=':
			sPrefix--;
			sPrefix--;
			break;

		case '_':
			sPrefix--;
			sPrefix--;
			sPrefix--;
			break;

		case '\r':
			PostWord((U8*)"");
			break;
			
		case '\001':
			PostWord((U8*)"ing");
			break;

		case '\002':
			PostWord((U8*)"al");
			break;

		case '\003':
			PostWord((U8*)"ch");
			break;

		case '\004':
			PostWord((U8*)"ed");
			break;

		case '\005':
			PostWord((U8*)"le");
			break;

		case '\006':
			PostWord((U8*)"ly");
			break;

		case '\007':
			PostWord((U8*)"us");
			break;

		case '\010':
			PostWord((U8*)"er");
			break;

		case '\011':
			PostWord((U8*)"es");
			break;

		case '\013':
			PostWord((U8*)"ie");
			break;

		case '\014':
			PostWord((U8*)"ng");
			break;

		case '\016':
			PostWord((U8*)"a");
			break;

		case '\017':
			PostWord((U8*)"d");
			break;

		case '\020':
			PostWord((U8*)"e");
			break;

		case '\021':
			PostWord((U8*)"h");
			break;

		case '\022':
			PostWord((U8*)"l");
			break;

		case '\023':
			PostWord((U8*)"n");
			break;

		case '\024':
			PostWord((U8*)"r");
			break;

		case '\025':
			PostWord((U8*)"s");
			break;

		case '\026':
			PostWord((U8*)"t");
			break;

		case '\027':
			PostWord((U8*)"y");
			break;

		default:
			if (*p & 0x80) {
				word[sWord++]=(*p) & 0x7F;
				PostPrefix();
			}
			else
				word[sWord++]=*p;
			break;
		}

		p++;
	}

	*pPRet=pPost;
	*pCbRet=sPost;
}

void Post(U8 *p, U32 cb) {
	if (sPost + cb + 1 > sPostMax) {
		if (sPostMax == 0)
			sPostMax=512;
		U8 *pPostNew=new U8[sPostMax*2];
		memcpy(pPostNew, pPost, sPost);
		delete pPost;
		pPost=pPostNew;
		sPostMax *= 2;
	}
	strncpy_s((char*)pPost+sPost, sPostMax - sPost, (char*)p, cb);
	sPost += cb;
}

void PostWord(U8 *p) {
	Post(prefix, sPrefix);
	Post(word, sWord);
	Post(p, strlen((char*)p));
	Post((U8*)"\r", 1);
	sWord=0;
}

void PostPrefix() {
	U8 *pWord=word;
	while (sWord) {
		prefix[sPrefix++]=*pWord++;
		sWord--;
	}
}
