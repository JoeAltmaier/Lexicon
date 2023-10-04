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

#include "ATypes.h"
#include "string.h"

uint8_t prefix[32];
uint32_t sPrefix=0;
uint8_t word[32];
uint32_t sWord=0;

uint8_t *pPost=NULL;
uint32_t sPost=0;
uint32_t sPostMax=0;
extern void Post(uint8_t *, uint32_t);
extern void PostWord(uint8_t *);
void PostPrefix();


void Decompress(uint8_t *p, uint32_t cb, uint8_t **pPRet, uint32_t *pCbRet) {

	uint8_t *pEnd=p+cb;

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
			PostWord((uint8_t*)"");
			break;
			
		case '\001':
			PostWord((uint8_t*)"ing");
			break;

		case '\002':
			PostWord((uint8_t*)"al");
			break;

		case '\003':
			PostWord((uint8_t*)"ch");
			break;

		case '\004':
			PostWord((uint8_t*)"ed");
			break;

		case '\005':
			PostWord((uint8_t*)"le");
			break;

		case '\006':
			PostWord((uint8_t*)"ly");
			break;

		case '\007':
			PostWord((uint8_t*)"us");
			break;

		case '\010':
			PostWord((uint8_t*)"er");
			break;

		case '\011':
			PostWord((uint8_t*)"es");
			break;

		case '\013':
			PostWord((uint8_t*)"ie");
			break;

		case '\014':
			PostWord((uint8_t*)"ng");
			break;

		case '\016':
			PostWord((uint8_t*)"a");
			break;

		case '\017':
			PostWord((uint8_t*)"d");
			break;

		case '\020':
			PostWord((uint8_t*)"e");
			break;

		case '\021':
			PostWord((uint8_t*)"h");
			break;

		case '\022':
			PostWord((uint8_t*)"l");
			break;

		case '\023':
			PostWord((uint8_t*)"n");
			break;

		case '\024':
			PostWord((uint8_t*)"r");
			break;

		case '\025':
			PostWord((uint8_t*)"s");
			break;

		case '\026':
			PostWord((uint8_t*)"t");
			break;

		case '\027':
			PostWord((uint8_t*)"y");
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

void Post(uint8_t *p, uint32_t cb) {
	if (sPost + cb > sPostMax) {
		if (sPostMax == 0)
			sPostMax=512;
		uint8_t *pPostNew=new uint8_t[sPostMax*2];
		memcpy(pPostNew, pPost, sPost);
		delete pPost;
		pPost=pPostNew;
		sPostMax *= 2;
	}
	strncpy((char*)pPost+sPost, (char*)p, cb);
	sPost += cb;
}

void PostWord(uint8_t *p) {
	Post(prefix, sPrefix);
	Post(word, sWord);
	Post(p, strlen((char*)p));
	Post((uint8_t*)"\r", 1);
	sWord=0;
}

void PostPrefix() {
	uint8_t *pWord=word;
	while (sWord) {
		prefix[sPrefix++]=*pWord++;
		sWord--;
	}
}
