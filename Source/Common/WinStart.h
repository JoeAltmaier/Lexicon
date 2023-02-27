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

#define MAINPLAYBUTTON      "IDB_MAINBACKGROUND"
#define MAINEXITBUTTON      "IDB_MAIN_EXIT"
#define MAINHELPBUTTON      "IDI_MAIN_HELP"
#define MAINCONFIGBUTTON      "IDI_MAIN_CONFIG"

class WinStart : public SWnd {
public:
	WinStart(MainWnd&);
	~WinStart();

	int  OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void Done();
	void Process();

	virtual BOOL OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual BOOL OnElmtListBox(SListBox* _pElmt, int _nNotification) override;
	virtual VOID OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip) override;

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

protected:
	DECLARE_MESSAGE_MAP()
};

#endif

