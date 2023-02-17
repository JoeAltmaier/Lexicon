/* SysMainElmt.cpp - Non-client area element
 *
 * Copyright (c) 2004 by
 *      Tom Nelson D.B.A. OpenVista Software
 *
 * This material is a confidential trade secret and proprietary information of
 * OpenVista Software, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of OpenVista Software.
 * This material is also copyrighted as an unpublished work under sections 104
 * and 408 of Title 17 of the United States Code. Law prohibits unauthorized
 * use, copying or reproduction.
 *
 * Revision History:
 *      4-24-05  TRN  Created
 *
**/

#include "AppAfx.h"

#include "SysMainElmt.h"

/*static*/ char MskBase::szPath[];// = "";

BOOL SysMainElmt::Create(DWORD _dwStyle, const CRect& _rect, SElementRef* _pParent)
{
    return BaseClass::Create(_dwStyle, _rect, _pParent) == OK;
}

ERC SysMainElmt::OnCreate(SCreateStruct &_cs)
{
    ERC erc;

    if ((erc = BaseClass::OnCreate(_cs)) != OK)
        return erc;

	ctrlBezel.Create(esVISIBLE | esNOMOUSE,IDB_BEZEL,1, CPoint(0, 0),this);

	CRect rcClient;
    rcClient = GetParent()->GetRect();

	rgMove.Create(SElement::esVISIBLE,CRect(15,0,rcClient.Width()-44,19), this);

    CSize sizeBt(12, 12);

	//btSysMenu.Create(SButton::bsNOFOCUSCELL | SButton::bsNODEFAULTCELL | SButton::bsNOSTEALTHCELL,IDB_BTSYSMENU, CRect(CPoint(5,4),sizeBt), this,IDC_SYSMENU);
	btSysMinimize.Create(SButton::bsNOFOCUSCELL | SButton::bsNODEFAULTCELL | SButton::bsNOSTEALTHCELL,IDB_BTMINIMIZE, CRect(CPoint(rcClient.Width()-44,4), sizeBt), this,IDC_SYSMINIMIZE);
	btSysMaximize.Create(SButton::esDISABLED | SButton::bsNOFOCUSCELL | SButton::bsNODEFAULTCELL | SButton::bsNOSTEALTHCELL,IDB_BTMAXIMIZE, CRect(CPoint(rcClient.Width()-31,4), sizeBt), this,IDC_SYSMAXIMIZE);
	btSysClose.Create(SButton::bsNOFOCUSCELL | SButton::bsNODEFAULTCELL | SButton::bsNOSTEALTHCELL,IDB_BTCLOSE, CRect(CPoint(rcClient.Width()-17,4), sizeBt), this,IDC_SYSCLOSE);

	//fMenu = menuSys.Create();

	return OK;
}


// .OnBnClicked
//
BOOL SysMainElmt::OnBnClicked(SButtonControl* _pElmt, CPoint _ptClick)
{
    return NotifyButtonParent(_pElmt, SButtonControl::tbeCLICK, _ptClick);
}
