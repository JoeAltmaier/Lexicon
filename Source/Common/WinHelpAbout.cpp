// WinHelpAbout.cpp - Paints a GifAnimator, manages buttons
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "WinHelpAbout.h"
#include <mmsystem.h>
#include "Resource1.h"
#include "LexSkins.h"


BEGIN_MESSAGE_MAP(WinHelpAbout, SWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

WinHelpAbout::WinHelpAbout(MainWnd& _winBoard, const char *_text)
	: winBoard(_winBoard), text(_text)
{
}


WinHelpAbout::~WinHelpAbout() {
}

int  WinHelpAbout::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	helpText.Create(0, &font, TColor(0), svLex.svrectHelpBox.rect, this, 0);

	helpText.SetText(text);
	helpText.MoveOrderTop();

	bmBackground.Create(svLex.svimgHelp.rect.Size(), 0);
	bmBackground.LoadDataImage((const char*)svLex.svimgHelp.pBytes, svLex.svimgHelp.cb);

	if (!elmtBtOk.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgHelpOk.pBytes, (int)svLex.svimgHelpOk.cb, svLex.svrectHelpOk.rect, this, IDB_HELP_OK))
		return ERR;
	if (!elmtBtAbout.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgHelpAbout.pBytes, (int)svLex.svimgHelpAbout.cb, svLex.svrectHelpAbout.rect, this, IDB_HELP_ABOUT))
		return ERR;

	return 0;
}

BOOL WinHelpAbout::OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_HELP_OK:
		winBoard.ReturnToMainScreen();
		break;

	case IDB_HELP_ABOUT:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
//		winBoard.ReturnToMainScreen();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		return true;
	}

	return false;
}

void WinHelpAbout::Process() {

}

VOID WinHelpAbout::OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip)
{
	_bmCanvas.CopyFrom(bmBackground, svLex.svimgHelp.rect, svLex.svimgHelp.rect, svLex.svimgHelp.rect, 0, 0);

	SWnd::OnBlend(_bmCanvas, _rcElmt, _rcClip);
}
