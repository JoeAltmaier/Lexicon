// WinStart.h - Main menu when starting game
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     2-23-23  JAltmaier  Created
 *
**/

#ifndef _WinStart_h
#define _WinStart_h

#include "MainWnd.h"
#include "SListBox.h"
#include "Leaderboard.h"

#define MAINPLAYBUTTON      "IDB_MAINBACKGROUND"
#define MAINEXITBUTTON      "IDB_MAIN_EXIT"
#define MAINHELPBUTTON      "IDI_MAIN_HELP"
#define MAINCONFIGBUTTON      "IDI_MAIN_CONFIG"

class WinStart : public SWnd, public LeaderboardCallback {
public:
	WinStart(MainWnd&);
	~WinStart();

	int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateLeaderboards(int score) { leaderboard.UpdateLeaderboards(score); }
	void Start();

private:
	void Done();
	void Process();

	virtual BOOL OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual BOOL OnElmtListBox(SListBox* _pElmt, int _nNotification) override;
	virtual BOOL OnElmtLButtonDown(UINT _nFlags, CPoint _point) override;
	virtual VOID OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip) override;
	virtual BOOL OnTextboxNotify(STextboxControl* _pElmt, int _nCode, CPoint _ptClick) ;
	void OnLeaderboardCallback(std::string&)  override;

private:
	MainWnd& winBoard;

	SListBox selectGame;

	CRect rectScores;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;

	MskButton elmtBtPlay;
	MskButton elmtBtExit;
	MskButton elmtBtConfig;
	MskButton elmtBtHelp;
	STextbox elmtScores;
	Leaderboard leaderboard;

protected:
	DECLARE_MESSAGE_MAP()
};

#endif

