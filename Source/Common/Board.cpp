// Board.cpp - definition of board animation of Lexicon rules
/*
 * Copyright (c) 2002 by
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

Board::Board(Configuration &_config, MainWnd &_winBoard)
	: Lexicon((U8*)svLex.svdictWordList.pBytes, svLex.svdictWordList.cb, (U8*)svLex.svtextBonusList.pString, svLex.svtextBonusList.cb, svLex.svboolBonusRandom.b, Coord(svLex.svrectTilesInBoard.rect.right, svLex.svrectTilesInBoard.rect.bottom), svLex.svscalarWordLengthMin.u32), 
	config(_config), winBoard(_winBoard)
{
	//Event(ENew, NULL); - wait until window initialization done
}


// Events from game actions

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
	case ENextLevel:
		ClearMatch();
		FillBoard(); // Fill out the letter grid from valid words
		ChooseBonusWord();
		winBoard.PopulateTiles(Letters()); // Give the letter grid to the window to display
		break;

	case EPause:
	case EMute:
		break;

	case ESwap:
		{
		SwapTiles* swap = (SwapTiles*)id;
		Swap(swap->tileA, swap->tileB, config.IsShuffle());

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

	case ELettersUsed:
		// Clocked games get an extra tick back per letter
		winBoard.Time(-(S32)id);
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
		winBoard.SetBonusWord((const U8*)id);
		break;

	case ELevelBonus:
		winBoard.LevelBonus((U32)id);
		break;

	case EBonusUsed:
		ChooseBonusWord();
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
	}
}

