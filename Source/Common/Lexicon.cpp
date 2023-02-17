// Lexicon.cpp - definition of rule object

/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     11-25-02  JCA  Created
 *
**/

#include "Lexicon.h"
#include "Random.h"

char Lexicon::aChDist[]="aaaaaaaaabbccddddeeeeeeeeeffggghhiiiiiiiiijkllllmmnnnnnnooooooooqqprrrrrrssssttttttuuuuvvwwxyyz";
U32 Lexicon::nDist;

Lexicon::Lexicon(U8 *pWords, U32 cb, U8 *_pBonusList, U32 _cbBonusList, BOOL _bRandomBonus, Coord _size, U32 _cbWordMin)
: size(_size), dictionary(pWords, cb, _pBonusList, _cbBonusList), cbWordMin(_cbWordMin), pBonusList(_pBonusList), cbBonusList(_cbBonusList), bRandomBonus(_bRandomBonus), iBonus(0), state(PLAY_IDLE)
{
	board=new U8[size.x * size.y+1];
	board[size.x * size.y]=0;
	bDirty=new BOOL[size.x * size.y];
	bBurn=new BOOL[size.x * size.y];
	nDist=strlen(aChDist);
}

void Lexicon::FillBoard() {
	// Collect enough random words to fill board
	char *aCh=new char[size.x * size.y];
	S32 cCh=0;

	while (cCh < size.x * size.y - max(size.x, size.y)) {
		const U8 *pWord=dictionary.Entry(Random::Value(dictionary.GetNWord()));
		U32 cb=dictionary.WordLen(pWord);
		memmove(&aCh[cCh], pWord, cb);
		cCh += cb;
	}
	// Fill out tail of aCh with random chars
	while (cCh < size.x * size.y)
		aCh[cCh++]=RandomChar();

	// Put aCh chars randomly into board
	Coord cd;
	for (cd.x=0; cd.x < size.x; cd.x++)
		for (cd.y=0; cd.y < size.y; cd.y++) {
			U32 iCh=Random::Value(cCh);
			Letter(cd)=aCh[iCh];
			Event(ELetterNew, cd);
			memmove(&aCh[iCh], &aCh[iCh+1], cCh-iCh-1);
			cCh--;
		}
}

void Lexicon::ResetScore() {
	state = PLAY_LEVEL;

	score=0;
	Event(EScoreUpdate, (void*)score);
	memset(aLastPossibleMatch, 0, sizeof(aLastPossibleMatch));
	aBestWord[0]=0;
	dScoreBestWord=0;
	Event(EBestWord, "");
}

void Lexicon::ClearMatch() {
	Coord cd;
	for (cd.x=0; cd.x < size.x; cd.x++)
		for (cd.y=0; cd.y < size.y; cd.y++) {
			Dirty(cd, false);
			Burn(cd, false);
		}
}

// Level is over, account for remaining score
void Lexicon::LevelEnd(bool _bQuit) {
	if (_bQuit)
		state = PLAY_IDLE;
	else
		state = PLAY_END;

	// Debit remaining tiles
	Coord cd;
	bool bAllBlank = true;
	for (cd.x = 0; cd.x < size.x; cd.x++)
		for (cd.y = 0; cd.y < size.y; cd.y++)
			if (!IsBlank(cd))
			{
				bAllBlank = false;
				Event(ELetterUsed, cd);
				ChScoreDebit(Letter(cd), cd);
				Blank(cd);
			}

	if (bAllBlank)
	{
		// Level bonus!
	}
}

void Lexicon::ChooseBonusWord() {
	bBonusUsed=false;

	U32 nBonus=dictionary.GetNBonus();
	if (nBonus) {
		if (bRandomBonus)
			iBonus=Random::Value(dictionary.GetNBonus());
		else
			iBonus = (iBonus+1) % dictionary.GetNBonus();

		Event(EBonusWord, (void*)dictionary.GetIBonus(iBonus));
	
	} else {
		BOOL bOk=false;
		const U8 *pWord;
		while (!bOk) {
			iBonus=Random::Value(dictionary.GetNWord());

			pWord=dictionary.Entry(iBonus);
			S32 cbWord=dictionary.WordLen(pWord);
			bOk=(cbWord >= cbWordMin && cbWord <= size.x);
		}

		Event(EBonusWord, (void*)pWord);
	}

}

BOOL Lexicon::Swap(const Coord &c1, const Coord &c2, BOOL bNeighbor) {
	if (bNeighbor)
		if (Neighbor(c1,c2) == NNot)
			return false;

	if (!bNeighbor)
		if (IsBlank(c1) || IsBlank(c2))
			return false;

	U8 &ch1=Letter(c1);
	U8 &ch2=Letter(c2);
	U8 chTemp=ch1;
	ch1=ch2;
	ch2=chTemp;

	// Mark which cells have been changed for later word search
	Dirty(c1);
	Dirty(c2);

	return true;
}

void Lexicon::Move(const Coord &c1, const Coord &c2) {
	Letter(c2) = Letter(c1);
	if (!(c1 == c2))
		Letter(c1)=CHBLANK;
}

U8 Lexicon::New(const Coord &cd) {
	return (Letter(cd) = RandomChar());
}

void Lexicon::Blank(const Coord &cd) {
	Letter(cd) = CHBLANK;
}

BOOL Lexicon::IsBlank(const Coord &cd)  {
	if (cd.x >= size.x || cd.y >= size.y || cd.x < 0 || cd.y < 0)
		return true;

	if (Letter(cd) == CHBLANK)
		return true;

	return false;
}

NeighborType Lexicon::Neighbor(const Coord &c1, const Coord &c2)  {
	if (c1.x >= size.x || c1.y >= size.y || c2.x >= size.x || c2.y >= size.y
		|| c1.x < 0 || c1.y < 0 || c2.x < 0 || c2.y < 0)
		return NNot;

	if (Letter(c1)==CHBLANK)
		return NNot;

	if (Letter(c2)==CHBLANK)
		return NNot;

	if (c1.x == c2.x-1 && c1.y == c2.y) return NLeft;
	if (c1.x == c2.x+1 && c1.y == c2.y) return NRight;
	if (c1.x == c2.x && c1.y == c2.y-1) return NTop;
	if (c1.x == c2.x && c1.y == c2.y+1) return NBottom;

	return NNot;
}

U8 Lexicon::RandomChar() {
	return aChDist[Random::Value(nDist)];
}

	U32 aBonus[26]={
//		a	b	c	d	e	f	g	h	i	j	k	l	m	n	o	p	q	r	s	t	u	v	w	x	y	z
		0,	0,	0,	0,	0,	1,	0,	1,	0,	2,	1,	0,	0,	0,	0,	0,	3,	0,	0,	0,	0,	1,	1,	2,	1,	3,	
	};

void Lexicon::Match() {
	// Search dictionary from largest to smallest possible words for words in every row and column
	// Post event for each letter removed
	// Post score event

	U32 bonus=0; // Bonus for each word beyond the first
	S32 cValueTotal=0;// Total of all letter values

	// Loop over rows, large to small word length
	U8 *pRow=new U8[size.x+1];
	pRow[size.x]=0;
	Coord cd;
	for (cd.y=0; cd.y < size.y; cd.y++) {

		// Collect letters in row
		for (cd.x=0; cd.x < size.x; cd.x++)
			pRow[cd.x]=Letter(cd);

		// Find all words using only letters in row
		U32 *aMatch=dictionary.MatchLetters(pRow, 0);

		// Search for words in aMatch, largest to smallest
		if (aMatch[0]) // at least one word possible
		for (S32 cCh=size.x; cCh >= cbWordMin; cCh--)
			for (cd.x = 0; cd.x <= size.x - cCh; cd.x++)
				// Only Burn if some letter of the word is Dirty, and not all burned yet (not sub-word in larger word)
				if (DirtyRow(cd, cCh) && !AllBurnedRow(cd, cCh))
					// If word is in aMatch, Burn() all its letters
					if (dictionary.MatchWord(&pRow[cd.x], cCh, aMatch)) {
						BurnRow(cd, cCh);
						U32 dScore= DScore(&pRow[cd.x], cCh);
						Event(EScore, new Score(cd, &pRow[cd.x], cCh, dScore + bonus, true));
						bonus += BONUSWORD; // increase for next word if any
						cValueTotal += DValue(&pRow[cd.x], cCh);
					}

	}

	// Loop over cols, large to small word length
	U8 *pCol=new U8[size.y+1];
	pCol[size.y]=0;
	for (cd.x=0; cd.x < size.x; cd.x++) {

		// Collect letters in col
		for (cd.y=0; cd.y < size.y; cd.y++)
			pCol[cd.y]=Letter(cd);

		// Find all words using only letters in col
		U32 *aMatch=dictionary.MatchLetters(pCol, 0);

		// Search for words in aMatch, largest to smallest
		for (S32 cCh=size.y; cCh >= cbWordMin; cCh--)
			for (cd.y = 0; cd.y <= size.y - cCh; cd.y++)
				// Only Burn if some letter of the word is Dirty, and not all burned yet (not sub-word in larger word)
				if (DirtyCol(cd, cCh) && !AllBurnedCol(cd, cCh))
					// If word is in aMatch, Burn() all its letters
					if (dictionary.MatchWord(&pCol[cd.y], cCh, aMatch)) {
						BurnCol(cd, cCh);
						U32 dScore= DScore(&pCol[cd.y], cCh);
						Event(EScore, new Score(cd, &pCol[cd.y], cCh, dScore + bonus, false));
						bonus += BONUSWORD; // increase for next word if any
						cValueTotal += DValue(&pCol[cd.y], cCh);
					}
	}

	// Post event for every burned letter
	for (cd.x=0; cd.x < size.x; cd.x++)
		for (cd.y=size.y-1; cd.y >= 0; cd.y--)
			if (IsBurned(cd))
			{
				Event(ELetterUsed, cd);
				Blank(cd);
			}

	ClearMatch();

	// Rack up letters
	for (cd.x = 0; cd.x < size.x; cd.x++)
	{
		int yBlank = -1;
		for (cd.y = size.y - 1; cd.y >= 0; cd.y--)
			if (IsBlank(cd))
			{
				if (yBlank == -1)
					yBlank = cd.y;
			}
			else 
				if (yBlank != -1)
				{
					// Move letter to yBlank
					Letter(Coord(cd.x, yBlank)) = Letter(cd);
					Blank(cd);
					yBlank--;
				}
	}

	// Credit clock for matching
	if (cValueTotal)
		Event(ELettersUsed, (void*)cValueTotal);

	// Choose new bonus word
	if (bBonusUsed)
		Event(EBonusUsed, NULL);
}

	// Multiply by bonus letter multiplier
	// If more than one bonus letter present, take total multiplier - (nBonusLetter-1)
	// E.g. Q is 4, X is 3,  QuaX gets (4+3) - (2-1) == 6
	// In other words, each bonus beyond the first gets added but one less
	// Implement this by starting with multiplier of 1, adding in all bonusus-1

U32 Lexicon::DScore(const U8 *pCh, U32 cCh) {
	U32 dScore=(cCh - (cbWordMin - 1)) * BONUSLETTER; // 3 letter:100, 4 letter:200 etc

	U32 bonus=1;
	for (U32 iCh=0; iCh < cCh; iCh++)
		bonus += aBonus[pCh[iCh] - 'a'];

	// Give 10X bonus for matching "Bonus Word"
	const U8 *pBonusWord=dictionary.GetIBonus(iBonus);
	U32 cb=dictionary.WordLen(pBonusWord);

	// Look for it anywhere within the matching characters e.g. match COWBOY, get bonus for BOY
	if (cCh >= cb)
		for (U32 i=0; i <= cCh - cb; i++)
			if (strncmp((const char *)pCh+i, (const char *)pBonusWord, cb) == 0) {
				bonus *= 10;
				bBonusUsed=true;
				break;
			}

	// Record "Best Word"
	if (bonus * dScore > dScoreBestWord) {
		dScoreBestWord = bonus * dScore;
		strncpy_s(aBestWord, sizeof(aBestWord), (char*)pCh, cCh);
		aBestWord[cCh]=0;
		Event(EBestWord, aBestWord);
	}

	score += (dScore * bonus);
	Event(EScoreUpdate, (void*)score);

	return dScore * bonus;
}

U32 Lexicon::DValue(const U8 *pCh, U32 cCh) {
	U32 value=0;
	for (U32 iCh=0; iCh < cCh; iCh++)
		value += aBonus[pCh[iCh] - 'a'];

	return value + cCh;
}

void Lexicon::ChScoreDebit(U8 ch, const Coord &cd) {
	S32 dScore= aBonus[ch - 'a'] + 1;
	dScore *= 100;
	score -= dScore;
	if (score < 0)
		score = 0;

	Event(EScoreUpdate, (void*)score);
	Event(EScore, new Score(cd, &ch, 1, -dScore, true));
}

void Lexicon::Bonus(S32 dScore) { 
	score += dScore; 
	Event(EScoreUpdate, (void*)score);
}

BOOL Lexicon::DirtyRow(Coord cd, U32 cCh) {
	while (cCh--)
		if (IsDirty(cd))
			return true;
		else
			cd.x++;

	return false;
}

BOOL Lexicon::AllBurnedRow(Coord cd, U32 cCh) {
	while (cCh--)
		if (!IsBurned(cd))
			return false;
		else
			cd.x++;

	return true;
}

void Lexicon::BurnRow(Coord cd, U32 cCh) {
	while (cCh--) {
		Burn(cd);
		cd.x++;
	}
}

BOOL Lexicon::DirtyCol(Coord cd, U32 cCh) {
	while (cCh--)
		if (IsDirty(cd))
			return true;
		else
			cd.y++;

	return false;
}


BOOL Lexicon::AllBurnedCol(Coord cd, U32 cCh) {
	while (cCh--)
		if (!IsBurned(cd))
			return false;
		else
			cd.y++;

	return true;
}

void Lexicon::BurnCol(Coord cd, U32 cCh) {
	while (cCh--) {
		Burn(cd);
		cd.y++;
	}
}

BOOL Lexicon::AnyBlankRow(Coord cd, U32 cCh) {
	while (cCh--)
		if (IsBlank(cd))
			return true;
		else
			cd.x++;

	return false;
}

BOOL Lexicon::AnyBlankCol(Coord cd, U32 cCh) {
	while (cCh--)
		if (IsBlank(cd))
			return true;
		else
			cd.y++;

	return false;
}

// Check for state transitions after display updates complete
void Lexicon::AnimationIdle() {
	switch (state)
	{
	case PLAY_IDLE:
		break;
	case PLAY_LEVEL:
		TestExhausted();
		TestImpossible();
		break;
	case PLAY_END:
		Event(ENextLevel, 0);
		break;
	default: break;
	}
}

void Lexicon::TestExhausted() {
	// Test for non-empty board, return if any possible word locations remain
	Coord cd;
	// Rows
	for (cd.y=0; cd.y < size.y; cd.y++)
		for (cd.x = 0; cd.x <= size.x - cbWordMin; cd.x++) {
			BOOL bFoundStreak=true;

			for (S32 iCh=0; iCh < cbWordMin; iCh++)
				if (IsBlank(Coord(cd.x + iCh, cd.y)))
					bFoundStreak=false;

			if (bFoundStreak)
				return; // Any streak will do - not exhausted
		}

	// Columns
	for (cd.x=0; cd.x < size.x; cd.x++)
		for (cd.y = 0; cd.y <= size.y - cbWordMin; cd.y++) {
			BOOL bFoundStreak=true;

			for (S32 iCh=0; iCh < cbWordMin; iCh++)
				if (IsBlank(Coord(cd.x, cd.y + iCh)))
					bFoundStreak=false;

			if (bFoundStreak)
				return; // Any streak will do - not exhausted
		}

	// No runs of non-blank tiles exist large enough to hold the minimum word size.  Level over.
	aLastPossibleMatch[0]=0;
	Event(EBoardExhausted, NULL);
}


void Lexicon::TestImpossible() {
	// Test if board contains letters and contiguous non-blank tiles that could make words
	U32 cChMax=0;
	S32 cCh;
	Coord cd;
	for (cd.y=0; cd.y < size.y; cd.y++)
		for (cCh=size.x; cCh >= cbWordMin; cCh--)
			for (cd.x = 0; cd.x <= size.x - cCh; cd.x++)
				// If no letter blank, a possible word location
				if (!AnyBlankRow(cd, cCh)) {
					cChMax=cCh;
					goto TryCols;
				}
	
TryCols:
	for (cd.x=0; cd.x < size.x; cd.x++)
		for (cCh=size.y; cCh >= cbWordMin; cCh--)
			for (cd.y = 0; cd.y <= size.y - cCh; cd.y++)
				if (!AnyBlankCol(cd, cCh)) {
					cChMax=cCh;
					goto FindWords;
				}

FindWords:
	if (cChMax < (U32)cbWordMin) {
		aLastPossibleMatch[0]=0;
		return; // No match possible: board is exhausted
	}

	U32 *aMatch=dictionary.MatchLetters(board, 0);
	for (U32 iMatch=0; aMatch[iMatch]; iMatch++) {
		const U8 *pWordMatch=dictionary.Entry(aMatch[iMatch]);
		U32 sWordMatch=dictionary.WordLen(pWordMatch);
		if (sWordMatch <= cChMax && sWordMatch >= (U32)cbWordMin) {
			memcpy(aLastPossibleMatch, pWordMatch, sWordMatch);
			aLastPossibleMatch[sWordMatch]=0;
			return; // Match is possible
		}
	}

	aLastPossibleMatch[0]=0;
	Event(EBoardImpossible, NULL);
}


// Helper Coord global functions
Coord operator+(const Coord &cd1, const Coord &cd2) { return Coord(cd1.x+cd2.x, cd1.y+cd2.y); }
Coord operator-(const Coord &cd1, const Coord &cd2) { return Coord(cd1.x-cd2.x, cd1.y-cd2.y); }
