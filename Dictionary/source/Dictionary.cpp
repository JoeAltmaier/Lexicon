// Dictionary.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ATypes.h"

#include <Windows.h>
#include <StdLib.h>
#include <string.h>

#include <io.h>
#include <tchar.h>
#include <sys/types.h>
#include <sys/stat.h>

extern void Output(char *, int);
extern void Flush() ;
extern void WritePrev();
extern void Decompress(uint8_t *p, uint32_t, uint8_t **, uint32_t*);
extern int Usage();

BOOL fSuffixOnly=false;
BOOL fPrefixOnly=false;


int main(int argc, char* argv[])
{
	// Get bits from file
	char *pFn=argv[1];

	if (pFn == nullptr)
		return Usage();

	if (*pFn == '"') {
		pFn++;
		for (int iCh=0; pFn[iCh]; iCh++)
			if (pFn[iCh] == '"')
				pFn[iCh]=0;
	}

	HANDLE fh=CreateFile(pFn, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (fh == (void*)-1)
		return Usage();

	int size;
	size=GetFileSize(fh, nullptr);
	char *buf=new char[(int)size];
	
	unsigned long cb;
	ReadFile(fh, buf, size, &cb, nullptr);

	if (argc > 2)
		if (argv[2][0] == '-') {
			if (argv[2][1] == 'd') {
				uint8_t *pData;
				uint32_t cbRet;
				Decompress((uint8_t*)buf, cb, &pData, &cbRet);
				while (cbRet--)
					printf("%c", *pData++);
				return 0;
			}
			else if (argv[2][1] == 's')
				fSuffixOnly=true;

			else if (argv[2][1] == 'p')
				fPrefixOnly=true;

			else
				return Usage();

		}
	else {
		// Compare two dictionaries
		HANDLE fh2=CreateFile(argv[2], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		int size2;
		size2=GetFileSize(fh2, nullptr);
		char *buf2=new char[(int)size2];
	
		unsigned long cb2;
		ReadFile(fh2, buf2, size2, &cb2, nullptr);

		char *pEnd=buf + cb;
		char *pEnd2=buf2+cb2;
		char *p=buf;
		char *p2=buf2;

		while(p < pEnd && p2 < pEnd2) {
		int i=0;
		while (p < pEnd && p[i] != '\r') i++;

		int i2=0;
		while (p2 < pEnd2 && p2[i2] != '\r') i2++;

		p[i]=0;
		p2[i2]=0;

		if (strcmp(p, p2) == 0)
			printf("%s\n", p);
		else
			printf("%s\t%s\n", p, p2);

		p += i+1;
		p2 += i2+1;
		}
		return 0;
	}

	// Read words, output only words in size range
	char *pEnd=buf + cb;
	while (buf < pEnd) {
		char *pWord=buf;
		BOOL bSpecial=false;
		while (*buf != '\r') (bSpecial|= !islower(*buf)),  buf++;
		*buf=0;


		int sWord=buf - pWord;
		if (sWord >= 3 && sWord <= 8 && !bSpecial)
			Output(pWord, sWord);

		buf++;
		if (*buf == '\n')
			buf++;
	}

	Flush();

	return 0;
}


char prefix[32];
int sPrefix=0;
char prev[32];
int sPrev=0;

void Output(char *pWord, int sWord) {
	if (sPrev == 0) {
		strncpy(prev, pWord, sWord);
		prev[sWord]=0;
		sPrev=sWord;
		return;
	}

	// We have a previous word.  Find the common prefix.
	int iCh;
	for (iCh=0; iCh < sWord; iCh++)
		if (prev[iCh] != pWord[iCh])
			break;

	// iCh is where the previous word and this word differ.
	// Make the similar part the prefix.
	// But we already have a prefix.  See if we need to push or pop.
	int iChPrefix;
	for (iChPrefix=0; iChPrefix < sPrefix && iChPrefix  < iCh; iChPrefix++)
		if (prefix[iChPrefix] != prev[iChPrefix])
			break;

	// iChPrefix is where the existing prefix and the new prefix differ

	BOOL fOutput=false;

	// Output previous word using longest prefix: old or new
	if (sPrefix > iCh) {
		WritePrev();
		fOutput=true;
	}

	// Make the prefix what we want: shorten or lengthen it


	// Pop from prefix to make them the same
	// Use different pop characters for 3,2, and 1 pop
	while (sPrefix >= iChPrefix+3) {
		if (!fSuffixOnly && !fPrefixOnly)
			printf("_");
		sPrefix -= 3;
	}
	while (sPrefix >= iChPrefix+2) {
		if (!fSuffixOnly && !fPrefixOnly)
			printf("=");
		sPrefix -= 2;
	}
	while (sPrefix > iChPrefix) {
		if (!fSuffixOnly && !fPrefixOnly)
			printf("-");
		sPrefix--;
	}

	// Push new prefix chars
// d e i k l m n o r s t 
	if (sPrefix < iCh) {
		while (sPrefix < iCh) {
			prefix[sPrefix] = prev[sPrefix];
			if (!fSuffixOnly)
				if (!fPrefixOnly)
					printf("%c", prev[sPrefix] | (sPrefix+1 == iCh? 0x80 :0));
				else
					printf("%c", prev[sPrefix]);
			sPrefix++;
		}
		if (fPrefixOnly)
			printf("\n");
	}

	if (!fOutput) {
		// Write previous word
		WritePrev();
	}

	// Save new word as "prev"
	strncpy(prev, pWord, sWord+1);
	sPrev=sWord;
}

void Flush() { Output(prev, sPrev); printf(""); }

void WritePrev() {
	// Output from sPrefix to sPrev
/*
	Special codes for common suffixes plus \r:
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
*/

	int i=sPrefix;
	
	if (!fSuffixOnly && !fPrefixOnly) {

	for (; i < sPrev-3; i++)
		printf("%c", prev[i]);

	if (strcmp(prev+i, "ing") == 0) {
		printf("\001");
		return;
	}

	for (; i < sPrev-2; i++)
		printf("%c", prev[i]);

	if (strcmp(prev+i, "al") == 0) {
		printf("\002");
		return;
	}

	if (strcmp(prev+i, "ch") == 0) {
		printf("\003");
		return;
	}

	if (strcmp(prev+i, "ed") == 0) {
		printf("\004");
		return;
	}
	
	if (strcmp(prev+i, "le") == 0) {
		printf("\005");
		return;
	}
	
	if (strcmp(prev+i, "ly") == 0) {
		printf("\006");
		return;
	}
	
	if (strcmp(prev+i, "us") == 0) {
		printf("\007");
		return;
	}
	
	if (strcmp(prev+i, "er") == 0) {
		printf("\010");
		return;
	}
	
	if (strcmp(prev+i, "es") == 0) {
		printf("\011");
		return;
	}
	
	if (strcmp(prev+i, "ie") == 0) {
		printf("\013");
		return;
	}
	
	if (strcmp(prev+i, "ng") == 0) {
		printf("\014");
		return;
	}
	
	for (; i < sPrev-1; i++)
		printf("%c", prev[i]);

	if (strcmp(prev+i, "a") == 0) {
		printf("\016");
		return;
	}

	if (strcmp(prev+i, "d") == 0) {
		printf("\017");
		return;
	}

	if (strcmp(prev+i, "e") == 0) {
		printf("\020");
		return;
	}

	if (strcmp(prev+i, "h") == 0) {
		printf("\021");
		return;
	}

	if (strcmp(prev+i, "l") == 0) {
		printf("\022");
		return;
	}

	if (strcmp(prev+i, "n") == 0) {
		printf("\023");
		return;
	}

	if (strcmp(prev+i, "r") == 0) {
		printf("\024");
		return;
	}

	if (strcmp(prev+i, "s") == 0) {
		printf("\025");
		return;
	}

	if (strcmp(prev+i, "t") == 0) {
		printf("\026");
		return;
	}

	if (strcmp(prev+i, "y") == 0) {
		printf("\027");
		return;
	}
	}	// !fSuffixOnly


	if (!fPrefixOnly)
		for (; i < sPrev; i++)
			printf("%c", prev[i]);

	if (!fPrefixOnly)
		printf("\r");
}

int Usage() {
				printf(
				"usage: dictionary wordlistfile		 (to compress the file)\n"
				"or     dictionary compressedfile -d (to decompress the file)\n"
				"or     dictionary wordlistfile -s   (to print suffixes only)\n"
				"or     dictionary wordlistfile wordlistfile2 (to compare two files)\n");
				return 0;
}
