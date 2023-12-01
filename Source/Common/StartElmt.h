// WinStart.h - Main menu when starting game
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     2-23-23  JAltmaier  Created
 *
**/

#ifndef _StartElmt_h
#define _StartElmt_h

#include "AppDefs.h"
#include "AppSkin.h"
#include "SElement.h"
#include "SFont.h"
#include "SButtonControl.h"
#include "STextboxControl.h"
#include "SListbox.h"
#include "Leaderboard.h"

#define MAINPLAYBUTTON      "IDB_MAINBACKGROUND"
#define MAINEXITBUTTON      "IDB_MAIN_EXIT"
#define MAINHELPBUTTON      "IDI_MAIN_HELP"
#define MAINCONFIGBUTTON      "IDI_MAIN_CONFIG"

class CLASS_ABASE2(StartElmt, public, SElement, public, LeaderboardCallback)
public:
	StartElmt();
	~StartElmt() { }

	void UpdateLeaderboards(int score) { leaderboard.UpdateLeaderboards(score); }
	void Start();

protected://SElement overrides
	virtual ERC  OnCreate(SCreateStruct& _cs) override;
	virtual void OnDestroy() override { }

private:
	void Done();
	void Process();

	virtual BOOL OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual ERC OnNotify(SElement* _pChild, WPARAM _wParam, LPARAM _lParam) override { return NotifyParent(_wParam, _lParam); }
	virtual BOOL OnListBox(SListBox* _pElmt, int _nNotification) override;
	virtual BOOL OnLButtonDown(UINT _nFlags, CPoint _point) override;
	virtual BOOL OnTextboxNotify(STextboxControl* _pElmt, int _nCode, CPoint _ptClick) override;
	void OnLeaderboardCallback(std::string&)  override;

private:
	SListBox selectGame;
//	SListBox selectBonusList;

	CRect rectScores;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;
	MskSkinDecal elmtBackground;

	MskButton elmtBtPlay;
	MskButton elmtBtExit;
	MskButton elmtBtConfig;
	MskButton elmtBtHelp;
	STextbox elmtScores;
	Leaderboard leaderboard;
};

#endif // StartElmt

