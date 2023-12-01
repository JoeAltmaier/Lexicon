// StartElmt.cpp - Main menu on start
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "StartElmt.h"
#include <mmsystem.h>
#include "Resource1.h"
#include "LexSkins.h"
#include "MainWnd.h"

char* leaderboards[] = { "Best Board", NULL };


StartElmt::StartElmt()
	: leaderboard(leaderboards, this, &LeaderboardCallback::OnLeaderboardCallback)
{
}


int  StartElmt::OnCreate(SCreateStruct& _cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	bmBackground.LoadDataImage((const char*)svLex.svimgMain.pBytes, svLex.svimgMain.cb);
	if (!elmtBackground.Create(esVISIBLE, bmBackground, 1, CPoint(0, 0), this))
		return ERR;	//*FIX* These old Msk classes in AppSkin.h need a lot of work.


	LOGFONT logfont;
	memset((char*)&logfont, 0, sizeof(logfont));
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
	logfont.lfHeight = 20;
	logfont.lfWeight = FW_BOLD;

	font.LoadLogFont(&logfont, svLex.svrgbTransparent.color);
	selectGame.Create(0, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectGameModeBox.rect, this, 0);

	selectGame.AddString(TEXT("Crash"));
	selectGame.SetSel(((MainWnd*)GetParentWnd())->config.IsShuffle() ? (((MainWnd*)GetParentWnd())->config.IsClocked() ? 1 : 0) : 2, true);

	elmtScores.Create(SElement::esVISIBLE, &font, TColor(0), svLex.svrectHighScoresBox.rect, this);

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

BOOL StartElmt::OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_MAIN_PLAY:
		// Start a game
		Process();
		MoveOrderBottom();

		SetCursor(LoadCursor(NULL, IDC_WAIT));
		((MainWnd*)GetParentWnd())->StartGame();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		return true;
	case IDB_MAIN_EXIT:
		Process();
		((MainWnd*)GetParentWnd())->Exit();
		return true;
	case IDB_MAIN_CONFIG:
		Process();
		((MainWnd*)GetParentWnd())->StartConfig();
		return true;
	case IDB_MAIN_HELP:
		Process();
		((MainWnd*)GetParentWnd())->StartHelp();
		return true;
	}

	return false;
}

BOOL StartElmt::OnLButtonDown(UINT _nFlags, CPoint _point)
{
	if (elmtScores.GetRect().PtInRect(_point))
		leaderboard.Rotate();

	return TRUE;
}

void StartElmt::Process() {
	((MainWnd*)GetParentWnd())->config.SetClocked(false);

	U32 iGame = selectGame.GetCurSel();
	if (iGame == 0)
		((MainWnd*)GetParentWnd())->config.SetGame(Configuration::GAME_SHUFFLE);

	else if (iGame == 1) {
		((MainWnd*)GetParentWnd())->config.SetGame(Configuration::GAME_SHUFFLE);
		((MainWnd*)GetParentWnd())->config.SetClocked(true);
	}

	else
		((MainWnd*)GetParentWnd())->config.SetGame(Configuration::GAME_CRASHANDBURN);
}

BOOL StartElmt::OnListBox(SListBox* _pElmt, int _nNotification)
{
	if (_pElmt == &selectGame)
	{
		PlaySound((TCHAR*)svLex.svwaveSelect.pBytes, NULL, SND_ASYNC | SND_MEMORY | SND_NOSTOP);
		Process();
	}

	return FALSE;
}

BOOL StartElmt::OnTextboxNotify(STextboxControl* _pElmt, int _nCode, CPoint _ptClick) 
{
	return FALSE;
}

void StartElmt::OnLeaderboardCallback(std::string& text)
{
	elmtScores.SetText(text.c_str());
}

void StartElmt::Start() { leaderboard.Start(); }