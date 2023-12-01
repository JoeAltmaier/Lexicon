// ConfigElmt.cpp - Paints a GifAnimator, manages buttons
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "ConfigElmt.h"
#include "resource1.h"
#include "LexSkins.h"



int  ConfigElmt::OnCreate(SCreateStruct &_cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	bmBackground.LoadDataImage((const char*)svLex.svimgSelect.pBytes, svLex.svimgSelect.cb);
	if (!elmtBackground.Create(esVISIBLE, bmBackground, 1, CPoint(0, 0), this))
		return ERR;	//*FIX* These old Msk classes in AppSkin.h need a lot of work.

	LOGFONT logfont;
	memset((char*)&logfont, 0, sizeof(logfont));
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
	logfont.lfHeight = 20;
	logfont.lfWeight = FW_BOLD;

	font.LoadLogFont(&logfont, 0);
	selectSkin.Create(esCAPTURECLICKS | MK_LBUTTON, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectSkinsBox.rect, this, 0);

	selectSkin.AddString(TEXT("Holiday"));
	selectSkin.AddString(TEXT("Summer Fun"));
	selectSkin.SetCurSel(0);
	selectSkin.SetFocus();

	selectDict.Create(esCAPTURECLICKS, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectDictsBox.rect, this, 0);
	selectDict.AddString(TEXT("English"));
	selectDict.SetCurSel(0);

	if (!elmtBtOk.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgSelectOk.pBytes, (int)svLex.svimgSelectOk.cb, svLex.svrectSelectOk.rect, this, IDB_SELECT_OK))
		return ERR;
	if (!elmtBtCancel.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgSelectCancel.pBytes, (int)svLex.svimgSelectCancel.cb, svLex.svrectSelectCancel.rect, this, IDB_SELECT_CANCEL))
		return ERR;

	return 0;
}

BOOL ConfigElmt::OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_SELECT_OK:
		// Start a game
		Process();

		// drop thru
	case IDB_SELECT_CANCEL:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		NotifyParent(IDB_SELECT_CANCEL, 0);
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		return true;
	}

	return false;
}

void ConfigElmt::Process() {

}

BOOL ConfigElmt::OnListBox(SListBox* _pElmt, int _nNotification)
{ 
	if (_pElmt == &selectSkin)
	{
		return TRUE;
	}

	return FALSE; 
}
