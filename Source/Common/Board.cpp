// Board.cpp - definition of board regulation of Lexicon rules
/*
 * Copyright (c) 2002, 2023 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     11-25-02  JCA  Created 
 *
**/

#include "Board.h"
#include "LexSkins.h"
#include "Resource.h"
#include "MainWnd.h"

#define FRAMESGLOW 0x30

Board::Board(Configuration &_config, MainWnd &_winBoard)
	: Lexicon(_config, (U8*)svLex.svdictWordList.pBytes, svLex.svdictWordList.cb, Coord(svLex.svrectTilesInBoard.rect.right, svLex.svrectTilesInBoard.rect.bottom), svLex.svscalarWordLengthMin.u32), 
	winBoard(_winBoard)
{
}

// Events from game actions (Lexicon, MainWnd)

void Board::Event(EventType evt, void *id) {
	switch (evt) {
	case EQuit:
		LevelEnd(true);
		break;

	case EMainWindow:
		winBoard.ReturnToMainScreen();
		break;

	case ENew:
		ResetScore();
		[[fallthrough]];
	case ENextLevel:
		ClearMatch();
		winBoard.NextBonusWord();
		FillBoard(); // Fill out the letter grid from valid words
		SetClock();
		winBoard.PopulateTiles(Letters()); // Give the letter grid to the window to display
		break;

	case EPause:
	case EMute:
		break;

	case ESwap:
		{
		SwapTiles* swap = (SwapTiles*)id;
		Swap(swap->tileA, swap->tileB, config.IsShuffle());
		winBoard.StartAnimation(swap->tileA, FRAMESGLOW);
		winBoard.StartAnimation(swap->tileB, FRAMESGLOW);

		delete (SwapTiles*)id;
		}
		break;

	case EMoveTile:
		{
		MoveTile* move = (MoveTile*)id;
		winBoard.StartAnimation(move->cdAt, move->cdTo);

		delete (MoveTile*)id;
		}
		break;

	case EStill: // All tiles still (post-word-score)
		AnimationIdle();
		break;

	case ETileDropped:
		// Calculate words formed, burn letters
		Match();
		break;

	case ETilesBurned:
		ReRack();
		break;

	case EClockCredit:
		// Set total clock credits
		winBoard.Time((U32)id);
		break;

	case EScore:
		{
		Score &score=*(Score*)id;

		// Post score balloon animation to winBoard
		winBoard.ScoreAnimation(score.cd, score.pWord, score.dScore, score.bHoriz);

		delete (Score*)id;
		}
		break;

	case EScoreUpdate:
		winBoard.SetScore((U32)id);
		break;

	case EBonusWord:
		SetBonusWord((U32)id);
		break;

	case ELevelBonus:
		winBoard.LevelBonus((U32)id);
		break;

	case EBonusUsed:
		winBoard.NextBonusWord(true);
		break;

	case EBestWord:
		winBoard.SetBestWord((U8*)id);
		break;

	case EBoardExhausted:
		// No more room for valid words
		LevelEnd();
		break;

	case EBoardImpossible:
		// Room for words, but letters present can't make any words
		// Crash only
		LevelEnd();
		break;

	// Event back from winBoard, ready for next level
	case EWinComplete:
		ClearMatch();
		FillBoard();
		break;

	case EGameOver:
		winBoard.GameOver(GetScore());
		break;

	case EAchieve:
		winBoard.Achieve((const char *)id);
		break;
	}
}

void Board::Event(EventType evt, Coord cd) {
	switch (evt) {
	case ELetterNew:
		winBoard.AddAnimation(cd);
		break;

	case ELetterUsed:
		// Letter burned.  Animate that.
		winBoard.StartAnimation(cd);
		break;

	case EDiscard:
		TestDiscard(cd);
		break;
	}
}

