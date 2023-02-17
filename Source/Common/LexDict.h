// LexDict.h - declaration of dictionary

#ifndef __LexDict_h
#define __LexDict_h

#include "LexTypes.h"
//#include <Windows.h>

class LexDict {
	U32 nEntry;
	U32 nMax;
	U32 sEntry;
	U8 **dictionary;
	U32 *maskMatch;
	U32 iEntryBonusFirst;

	enum {NMATCHMAX=10240, MAXLETTERS=32};
	U32 aMatch[2][NMATCHMAX+1];

	static const 
	U32 mask_ch[26];

public:
	LexDict(U8 *, U32, U8 *, U32);
	~LexDict();

	const U8 *Entry(U32);

	U32 *Match(U32 mask, U32 iMatch);
	U32 *MatchLetters(U8 *pLetters, U32 iMatch);
	BOOL MatchWord(U8 *pWord, U32 iMatch);
	BOOL MatchWord(U8 *pWord, U32 cCh, U32 *aMatch);
	U32 GetNBonus() { return nEntry - iEntryBonusFirst; }
	U32 GetNWord() { return nEntry; }
	const U8 * GetIBonus(U32 iWord) { if (GetNBonus()) return Entry(iWord + iEntryBonusFirst); else return Entry(iWord); }
	U32 WordLen(const U8* p) { U32 cb;  for (cb = 0; p[cb] != '\r'; cb++); return cb; }

private:
	U32 Mask_Word(U8 *);
	void MakeRoom();
};

#endif
