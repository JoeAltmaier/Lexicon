// Lexicon.h - declaration of rule object

/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     11-25-02  JCA  Created
 *
**/


#ifndef __Lexicon_h
#define __Lexicon_h

#include "LexTypes.h"
#include "LexDict.h"
#include "Configuration.h"

#define CLOCKSTEPS 20

class Lexicon {
	// Board state
	U8 *board;
	BOOL *bDirty;
	BOOL *bBurn;
	Coord size;

	enum {PLAY_IDLE, PLAY_LEVEL, PLAY_END} state;

	// Score state
	S32 score;

	// Clock state
	U32 clock;
	U32 clockMax;

	// Dictionary
	LexDict dictionary;

	S32 cbWordMin;

	U32 iBonus;
	BOOL bRandomBonus;
	BOOL bBonusUsed;

	char aLastPossibleMatch[32];
	char aBestWord[32];
	U32 dScoreBestWord;
	U32 cLevelsConsecutive;
	U32 cLevelsOneGame;
	U32 cBonusWords;

	static
	char aChDist[];

	static
	U32 nDist;

protected:
	Configuration& config;

public:
	Lexicon(Configuration &_config, U8 *_pWords, U32 _cb, Coord _size, U32 _cbWordMin);

	void FillBoard();

	void ResetScore();
	U32 DScore(const U8 *pCh, U32 cCh);
	U32 DValue(const U8 *pCh, U32 cCh);
	void ChScoreDebit(U8 ch, const Coord &cd);

	void ClearMatch();
	void LevelEnd(bool _bQuit = false);
	void SetBonusWord(U32 _iBonus) { iBonus = _iBonus; }
	void SetBonusWordList(const U8* pCh, U32 cCh);
	void SetClock() { Event(EClockCredit, (void*)clock); }

	virtual
	void Event(EventType, void *_id = NULL)=0;

	virtual
	void Event(EventType, Coord)=0;

	BOOL Swap(const Coord &c1, const Coord &c2, BOOL bNeighbor);
	void Move(const Coord &c1, const Coord &c2);
	U8 New(const Coord &cd);
	void Blank(const Coord &cd);

	void Match();
	void ReRack();

	Coord Size() { return size; }

	U32 GetScore() { return score; }
	U32 GetClock() { return clock; }

	U8 &Letter(const Coord &cd) { return board[size.x * cd.y + cd.x]; }

	BOOL IsDirty(const Coord &cd) { return bDirty[size.x * cd.y + cd.x]; }
	void Dirty(const Coord &cd, BOOL b=true) { bDirty[size.x * cd.y + cd.x]=b; }
	BOOL IsBurned(const Coord &cd) { return bBurn[size.x * cd.y + cd.x]; }
	void Burn(const Coord &cd, BOOL b=true) { bBurn[size.x * cd.y + cd.x]=b; }

	const char *LastPossibleMatch() { return aLastPossibleMatch; }

	NeighborType Neighbor(const Coord &c1, const Coord &c2) ;
	BOOL IsBlank(const Coord &cd);
	BOOL NoBlank();
	void AnimationIdle();
	U8* Letters() { return board; }

protected:
	void TestDiscard(Coord cd);

private:

	BOOL DirtyRow(Coord cd, U32 cCh);
	BOOL AllBurnedRow(Coord cd, U32 cCh);
	void BurnRow(Coord cd, U32 cCh);
	BOOL DirtyCol(Coord cd, U32 cCh);
	BOOL AllBurnedCol(Coord cd, U32 cCh);
	void BurnCol(Coord cd, U32 cCh);
	BOOL AnyBlankRow(Coord cd, U32 cCh);
	BOOL AnyBlankCol(Coord cd, U32 cCh);

	U8 RandomChar();

	void TestExhausted();
	void TestImpossible();
};

typedef struct Score {
	Coord cd;
	U32 dScore;
	U8 *pWord;
	BOOL bHoriz;
	Score(const Coord &_cd, const U8 *_pWord, U32 _cb, U32 _dScore, BOOL _bHoriz)
		: cd(_cd), dScore(_dScore), pWord(new U8[_cb+1]), bHoriz(_bHoriz)
	{
		strncpy_s((char*)pWord, _cb + 1, (const char*)_pWord, _cb);
		pWord[_cb]=0;
	};
	~Score() { delete pWord; }
} Score;

enum {BONUSLETTER=100, BONUSWORD=100, CHBLANK=('z'+1)};

#endif
