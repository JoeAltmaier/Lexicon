/* PlayAreaElmt.h -- Play Area element
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Engineering, 2008,2009
 *
 * This material is a confidential trade secret and proprietary information of
 * Iowa Software Engineering, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of Iowa Software Engineering.
 * This material is also copyrighted as an unpublished work under sections 104 and
 * 408 of Title 17 of the United States Code. Law prohibits unauthorized use, 
 * copying or reproduction.
 *
 * Tom Nelson, PO Box 649, North Liberty,IA 52317, 319-936-6856
 * Iowa Software Engineering, 2 Hawkeye Dr. Ste 103, North Liberty,IA 52317, 319-626-3077
 *
 * Revision History:
 *      4-24-05  TRN  Created
 *      1-23-23  JCA  Added Mute, Pause
 *
**/


#pragma once

#ifndef __PlayAreaElmt_h
#define __PlayAreaElmt_h

#include "AppDefs.h"
#include "SysMenu.h"
#include "AppSkin.h"
#include "SFontBox.h"
#include "ACircularList_T.h"

#include "TileGridElmt.h"


#define PLAYBACKGROUND		"IDB_PLAYBOARD"
#define PLAYQUITBUTTON      "IDB_QUITBUTTON"
#define PLAYNEWBUTTON       "IDB_NEWBUTTON"
#define PLAYMUTEBUTTON       "IDB_MUTEBUTTON"
#define PLAYPAUSEBUTTON       "IDB_PAUSEBUTTON"
#define PLAYPAUSELEGENDBUTTON       "IDB_PAUSELEGENDBUTTON"
#define PLAYLEVELBONUSBUTTON       "IDB_BONUSLEVEL"

// Main Window Non-Client area element
//
class CLASS_ABASE(PlayAreaElmt,public,SElement)
public:
	PlayAreaElmt() { }
	~PlayAreaElmt() { }
		
protected://SFC overrides
	virtual ERC  OnCreate(SCreateStruct &_cs) override;
	virtual void OnDestroy() override;

    virtual BOOL OnButtonNotify(SButtonControl *_pElmt,int _tEvent,CPoint _ptClick) override;
	virtual ERC OnNotify(SElement* _pChild, WPARAM _wParam, LPARAM _lParam) override { return NotifyParent(_wParam, _lParam); }

public:
	// from MainWnd, destined for the appropriate elmt
	void PopulateTiles(U8* board) 
	{ 
		elmtTileGrid.DestroyTiles();
		elmtTileGrid.PauseGame(false);
		elmtPauseLegend.SetItem(!elmtTileGrid.IsPauseGame());
		elmtTileGrid.DropPlayTiles(board);
	}
	void CreateFloater(const Coord& cd, S32 score) { elmtTileGrid.FloatScore(cd, score); }
	void SetBonusWord(const char* _pBonusWord) { elmtBonusWord.SetText(_pBonusWord); }
	void SetBestWord(const char* _pBestWord) { elmtBestWord.SetText(_pBestWord); }
	void SetScore(U32 score);
	void StartAnimation(const Coord& cd) { elmtTileGrid.StartAnimation(cd); }
	void StartAnimation(const Coord& cdAt, const Coord& cdTo) { elmtTileGrid.StartAnimation(cdAt, cdTo); }
	void AnimationIdle() { elmtTileGrid.AnimationIdle(); }
	void LevelBonus(U32 bOn) { elmtLevelBonus.SetItem(!bOn); }

	bool IsNew(SElement* pElmt) { return (pElmt == &elmtBtNew); }
	void Quit()
	{
		elmtTileGrid.DestroyTiles();
		elmtTileGrid.DropGameOver();
		elmtPauseLegend.SetItem(!elmtTileGrid.IsPauseGame());
	}
private:
	TBitmap bmBackground;
	MskSkinDecal elmtBackground;
	TileGridElmt elmtTileGrid;
    MskResButton elmtBtQuit;
	MskResButton elmtBtNew;
	MskResButton elmtBtMute;
	MskResButton elmtBtPause;
	MskResDecal elmtPauseLegend;
	MskResDecal elmtLevelBonus;
	SFont font;
	TBitmap bmFontStrip;
	SFontBox elmtBonusWord;
	SFontBox elmtBestWord;
	SFontBox elmtScore;
	Coord cdCenter;
};

#endif	// __PlayAreaElmt_h
