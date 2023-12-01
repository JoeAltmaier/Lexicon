// HelpElmt.cpp - Paints a GifAnimator, manages buttons
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "HelpElmt.h"
#include <mmsystem.h>
#include "Resource1.h"
#include "LexSkins.h"
#include "MainWnd.h"

void* LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32* size);


HelpElmt::HelpElmt() {}


HelpElmt::~HelpElmt() {}

int  HelpElmt::OnCreate(SCreateStruct& _cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	UINT32 cb;
	about = (char*)LocateResource(IDT_ABOUT, TEXT("TEXT"), &cb);
	text = svLex.svtextHelp.pString;

	bmBackground.LoadDataImage((const char*)svLex.svimgHelp.pBytes, svLex.svimgHelp.cb);
	if (!elmtBackground.Create(esVISIBLE, bmBackground, 1, CPoint(0, 0), this))
		return ERR;	//*FIX* These old Msk classes in AppSkin.h need a lot of work.
	
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
	//helpText.ShowElement(0);

	if (!elmtBtOk.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgHelpOk.pBytes, (int)svLex.svimgHelpOk.cb, svLex.svrectHelpOk.rect, this, IDB_HELP_OK))
		return ERR;
	if (!elmtBtAbout.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgHelpAbout.pBytes, (int)svLex.svimgHelpAbout.cb, svLex.svrectHelpAbout.rect, this, IDB_HELP_ABOUT))
		return ERR;

	return 0;
}

BOOL HelpElmt::OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_HELP_OK:
		helpText.SetText(text);
		((MainWnd*)GetParentWnd())->ReturnToMainScreen();
		break;

	case IDB_HELP_ABOUT:
		helpText.SetText(about);
		return true;
	}

	return false;
}

void HelpElmt::OnElementPosChanged(SElementPos& _pos)
{
	if (GetParent()->GetFirstChild() != this)
		helpText.ShowElement(1);
	else
		helpText.ShowElement(0);
}

void* LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32* size)
{
	HRSRC hr = FindResource(NULL, MAKEINTRESOURCE(id), lpType);
	*size = SizeofResource(NULL, hr);
	return LoadResource(NULL, hr);
}
