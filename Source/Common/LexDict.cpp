// LexDict.cpp - definition of dictionary

#include "LexDict.h"
#include <stdlib.h>

const U32 LexDict::mask_ch[26]={
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
	0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
	0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
	0x100000, 0x200000
};

LexDict::LexDict(U8 *p, U32 cb, U8 *p2, U32 cb2) {
	// Load text blobs, parse into words, create maskMatch.
	nMax=1024;
	dictionary=new U8*[nMax];
	maskMatch=new U32[nMax];
	sEntry=0;

	nEntry=1; // 0 is illegal entry index
	U8 *pEnd=p+cb;
	while (p < pEnd) {
		MakeRoom();

		dictionary[nEntry]=p;
		maskMatch[nEntry]=0;
		U32 cCh=0;
		while (*p != '\r' && *p != 0) {
			maskMatch[nEntry] |= mask_ch[tolower(*p) - 'a'];
			p++;
			cCh++;
		}
		nEntry++;
		if (cCh > sEntry)
			sEntry = cCh;
		p++;
	}

	iEntryBonusFirst=nEntry;

	pEnd=p2+cb2;
	while (p2 < pEnd) {
		MakeRoom();

		dictionary[nEntry]=p2;
		maskMatch[nEntry]=0;
		U32 cCh=0;
		while (*p2 != '\r' && *p2 != 0) {
			maskMatch[nEntry] |= mask_ch[tolower(*p2) - 'a'];
			p2++;
			cCh++;
		}
		nEntry++;
		if (cCh > sEntry)
			sEntry = cCh;
		p2++;
		if (*p2 == '\n')
			p2++;
	}

}

LexDict::~LexDict() {
	delete dictionary;
	delete maskMatch;
}

void LexDict::MakeRoom() {
	if (nEntry >= nMax) {
		U8 **dictNew=new U8*[nMax*2];
		U32 *maskNew=new U32[nMax*2];
		for (U32 i=0; i < nMax; i++) {
			dictNew[i]=dictionary[i];
			maskNew[i]=maskMatch[i];
		}
		delete dictionary;
		delete maskMatch;
		dictionary=dictNew;
		maskMatch=maskNew;
		nMax *= 2;
	}
}

const U8 *LexDict::Entry(U32 iEntry) {
	return dictionary[iEntry];
}

U32 *LexDict::Match(U32 mask, U32 iMatch) {
	// Pre-screen the dictionary for words that have only the letters in mask
	U32 i=0;
	for (U32 iEntry=1; iEntry < nEntry && i < NMATCHMAX; iEntry++)
		if ((maskMatch[iEntry] & mask) == maskMatch[iEntry])
			aMatch[iMatch][i++]=iEntry;

	aMatch[iMatch][i]=0;

	return aMatch[iMatch];
}

U32 *LexDict::MatchLetters(U8 *pLetters, U32 iMatch) {
	// Find a list of words that have all the given letters

	// Make a U8 letter list
	U32 nLetters=0;
	while (pLetters[nLetters])
		nLetters++;

	// Find words that have only the given letters
	U32 mask=Mask_Word(pLetters);
	U32 *aMatch=Match(mask, iMatch);

	U8 *aMask=new U8[nLetters]; // 1 flag for each letter in pLetters

	// Test all the words found to see if they have all the letters exactly (including duplicates)
	U32 iPut=0;
	for (int i=0; aMatch[i]; i++) {
		const U8 *pWord=Entry(aMatch[i]);

		// Test each letter in this word against pLetters
		for (U32 iLetter=0; iLetter < nLetters; iLetter++) 
			aMask[iLetter]=0;

		while (*pWord && *pWord != '\r') {
			// Scan letters left in pLetters, see if *pWord is there
			for (U32 iLetter=0; pLetters[iLetter]; iLetter++)
				if (!aMask[iLetter] && pLetters[iLetter] == *pWord) {
					aMask[iLetter]=1;
					goto NextLetterInWord;
				}
			// Can't find *pWord in pLetters (that hasn't been used already)
			// Must be a duplicate letter.  Oh well, this word doesn't work.
			goto NextWordInMatch;

NextLetterInWord:
			pWord++;
		}

		// For every letter in pWord, found a unique instance in pLetters.
		aMatch[iPut++]=aMatch[i];

NextWordInMatch:
		;
	}

	// Zero-terminate the list
	aMatch[iPut]=0;

	delete aMask;

	return aMatch;
}

BOOL LexDict::MatchWord(U8 *pWord, U32 iMatch) {
	// Find if the word exists

	// Find words that have only the given letters
	U32 mask=Mask_Word(pWord);
	U32 *aMatch=Match(mask, iMatch);
	U32 cCh=strlen((char*)pWord);

	// Test all the words found to see if any is our word
	for (int i=0; aMatch[i]; i++) {
		const U8 *pEntry=Entry(aMatch[i]);
		if (WordLen(pEntry) == cCh)
			if (strncmp((char*)pWord, (char*)pEntry, cCh) == 0)
				return true;
	}

	return false;
}

BOOL LexDict::MatchWord(U8 *pWord, U32 cCh, U32 *aMatch) {
	// Find the word 

	// Test all the words found to see if any is our word
	for (int i=0; aMatch[i]; i++) {
		const U8 *pWordMatch=Entry(aMatch[i]);
		if (WordLen(pWordMatch) == cCh)
			if (strncmp((char*)pWord, (char*)pWordMatch, cCh) == 0)
				return true;
	}

	return false;
}

U32 LexDict::Mask_Word(U8 *pWord) {
	U32 mask=0;
	while (*pWord) {
		if (isalpha(*pWord))
			mask |= mask_ch[tolower(*pWord) - 'a'];
		pWord++;
	}

	return mask;
}
