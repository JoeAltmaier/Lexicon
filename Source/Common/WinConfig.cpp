// WinConfig.cpp - Paints a GifAnimator, manages buttons
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "WinConfig.h"
#include <mmsystem.h>
#include "Resource1.h"
#include "LexSkins.h"


BEGIN_MESSAGE_MAP(WinConfig, SWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

WinConfig::WinConfig(MainWnd& _winBoard)
: winBoard(_winBoard)
{
}


WinConfig::~WinConfig() {
}

int  WinConfig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ERC erc;

	if ((erc = SWnd::OnCreate(lpCreateStruct)) == -1)
		return erc;

	LOGFONT logfont;
	memset((char*)&logfont, 0, sizeof(logfont));
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
	logfont.lfHeight = 20;
	logfont.lfWeight = FW_BOLD;

	font.LoadLogFont(&logfont, 0);
	selectSkin.Create(0, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectSkinsBox.rect, this, 0);

	selectSkin.AddString(TEXT("Rosewood"));
	selectSkin.SetFocus();

	selectDict.Create(0, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectDictsBox.rect, this, 0);
	selectDict.AddString(TEXT("English"));

	bmBackground.Create(svLex.svimgSelect.rect.Size(), 0);
	bmBackground.LoadDataImage((const char *)svLex.svimgSelect.pBytes, svLex.svimgSelect.cb);

	if (!elmtBtOk.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgSelectOk.pBytes, (int)svLex.svimgSelectOk.cb, svLex.svrectSelectOk.rect, this, IDB_SELECT_OK))
		return ERR;
	if (!elmtBtCancel.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgSelectCancel.pBytes, (int)svLex.svimgSelectCancel.cb, svLex.svrectSelectCancel.rect, this, IDB_SELECT_CANCEL))
		return ERR;

	selectSkin.MoveOrderTop();

	return 0;
}

BOOL WinConfig::OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_SELECT_OK:
		// Start a game
		Process();

		// drop thru
	case IDB_SELECT_CANCEL:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		winBoard.ReturnToMainScreen();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		return true;
	}

	return false;
}

void WinConfig::Process() {

}

BOOL WinConfig::OnElmtListBox(SListBox* _pElmt, int _nNotification)
{ 
	if (_pElmt == &selectSkin)
	{
		return TRUE;
	}

	return FALSE; 
}


VOID WinConfig::OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip) 
{
	_bmCanvas.CopyFrom(bmBackground, svLex.svimgBackground.rect, svLex.svimgBackground.rect, svLex.svimgBackground.rect, 0, 0);
}
