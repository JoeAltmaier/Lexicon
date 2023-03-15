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

void* LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32* size);


BEGIN_MESSAGE_MAP(WinHelpAbout, SWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

WinHelpAbout::WinHelpAbout(MainWnd& _winBoard, const char *_text)
	: winBoard(_winBoard), text(_text), about("---")
{
	UINT32 cb;
	about = (char*)LocateResource(IDT_ABOUT, TEXT("TEXT"), &cb);

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

	font.LoadLogFont(&logfont, 0);
	
	helpText.Create(SElement::esVISIBLE | SEditboxControl::esAUTOVSCROLL | SEditboxControl::esMULTILINE, svLex.svrectHelpBox.rect, this);
	helpText.SetText(text);
	helpText.SetFont(&font);
	helpText.MoveOrderTop();
	helpText.SetReadOnly();
	
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
		helpText.SetText(text);
		winBoard.ReturnToMainScreen();
		break;

	case IDB_HELP_ABOUT:
		helpText.SetText(about);
		return true;
	}

	return false;
}

VOID WinHelpAbout::OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip)
{
	_bmCanvas.CopyFrom(bmBackground, svLex.svimgHelp.rect, svLex.svimgHelp.rect, svLex.svimgHelp.rect, 0, 0);
}



void* LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32* size)
{
	HRSRC hr = FindResource(NULL, MAKEINTRESOURCE(id), lpType);
	*size = SizeofResource(NULL, hr);
	return LoadResource(NULL, hr);
}
