// WinStart.cpp - Main menu on start
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "WinStart.h"
#include <mmsystem.h>
#include "Resource1.h"
#include "LexSkins.h"


BEGIN_MESSAGE_MAP(WinStart, SWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


char* leaderboards[] = { "Best_Board", NULL };


WinStart::WinStart(MainWnd& _winBoard)
	: winBoard(_winBoard), leaderboard(leaderboards, this, &LeaderboardCallback::OnLeaderboardCallback)
{
}


WinStart::~WinStart() {
}

int  WinStart::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ERC erc;

	if ((erc = SWnd::OnCreate(lpCreateStruct)) == -1)
		return erc;

	LOGFONT logfont;
	memset((char*)&logfont, 0, sizeof(logfont));
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
	logfont.lfHeight = 20;
	logfont.lfWeight = FW_BOLD;

	font.LoadLogFont(&logfont, svLex.svrgbTransparent.color);
	selectGame.Create(0, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectGameModeBox.rect, this, 0);

	selectGame.AddString(TEXT("Crash"));
	selectGame.SetSel(winBoard.config.IsShuffle() ? (winBoard.config.IsClocked() ? 1 : 0) : 2, true);

	elmtScores.Create(SElement::esVISIBLE, &font, TColor(0), svLex.svrectHighScoresBox.rect, this);

	bmBackground.Create(svLex.svimgBackground.rect.Size(), 0);
	bmBackground.LoadDataImage((const char*)svLex.svimgMain.pBytes, svLex.svimgMain.cb);

	if (!elmtBtPlay.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgMainPlay.pBytes, (int)svLex.svimgMainPlay.cb, svLex.svrectMainPlay.rect, this, IDB_MAIN_PLAY))
		return ERR;
	if (!elmtBtExit.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgMainExit.pBytes, (int)svLex.svimgMainExit.cb, svLex.svrectMainExit.rect, this, IDB_MAIN_EXIT))
		return ERR;
	if (!elmtBtConfig.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgMainConfig.pBytes, (int)svLex.svimgMainConfig.cb, svLex.svrectMainConfig.rect, this, IDB_MAIN_CONFIG))
		return ERR;
	if (!elmtBtHelp.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgMainHelp.pBytes, (int)svLex.svimgMainHelp.cb, svLex.svrectMainHelp.rect, this, IDB_MAIN_HELP))
		return ERR;

	return 0;
}

BOOL WinStart::OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_MAIN_PLAY:
		// Start a game
		Process();

		SetCursor(LoadCursor(NULL, IDC_WAIT));
		winBoard.StartGame();
		ShowWindow(SW_HIDE);
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		return true;
	case IDB_MAIN_EXIT:
		Process();
		winBoard.Exit();
		return true;
	case IDB_MAIN_CONFIG:
		Process();
		winBoard.StartSelect();
		return true;
	case IDB_MAIN_HELP:
		Process();
		winBoard.StartHelp();
		return true;
	}

	return false;
}

BOOL WinStart::OnElmtLButtonDown(UINT _nFlags, CPoint _point)
{
	if (elmtScores.GetRect().PtInRect(_point))
		leaderboard.Rotate();

	return SWnd::OnElmtLButtonDown(_nFlags, _point);
}

void WinStart::Process() {
	winBoard.config.SetClocked(false);

	U32 iGame = selectGame.GetCurSel();
	if (iGame == 0)
		winBoard.config.SetGame(Configuration::GAME_SHUFFLE);

	else if (iGame == 1) {
		winBoard.config.SetGame(Configuration::GAME_SHUFFLE);
		winBoard.config.SetClocked(true);
	}

	else
		winBoard.config.SetGame(Configuration::GAME_CRASHANDBURN);
}

BOOL WinStart::OnElmtListBox(SListBox* _pElmt, int _nNotification)
{
	if (_pElmt == &selectGame)
	{
		PlaySound((TCHAR*)svLex.svwaveSelect.pBytes, NULL, SND_ASYNC | SND_MEMORY | SND_NOSTOP);
		Process();
	}

	return FALSE;
}

BOOL WinStart::OnTextboxNotify(STextboxControl* _pElmt, int _nCode, CPoint _ptClick) 
{
	return FALSE;
}

void WinStart::OnLeaderboardCallback(std::string& text)
{
	elmtScores.SetText(text.c_str());
}

VOID WinStart::OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip)
{
	_bmCanvas.CopyFrom(bmBackground, svLex.svimgBackground.rect, svLex.svimgBackground.rect, svLex.svimgBackground.rect, 0, 0);
}

void WinStart::Start() { leaderboard.Start(); }