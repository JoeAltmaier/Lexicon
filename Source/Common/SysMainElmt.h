/* SysMainElmt.h - Non-client area element
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


#pragma once

#ifndef __SysMainElmt_h
#define __SysMainElmt_h

#include "AppDefs.h"
#include "SElement.h"
#include "SysMenu.h"

#include "AppSkin.h"


// Main Window Non-Client area element
//
class CLASS_ABASE(SysMainElmt, public, SElement)
public:
    SysMainElmt() : fMenu(FALSE) { }

	BOOL Create(DWORD _dwStyle,const CRect &rect,SElementRef *_pParent);

protected:
    virtual ERC  OnCreate(SCreateStruct& _cs) override;
    virtual BOOL OnBnClicked(SButtonControl* _pElmt, CPoint _ptClick) override;
   // virtual VOID OnBlend(TBitmap& _bmCanvas, const CRect& _rcArea, const CRect& _rcClip) { }

public:
	MskResDecal ctrlBezel;
	MskMove  rgMove;

	//MskMenuButton btSysMenu;
	MskResButton btSysMinimize;
	MskResButton btSysMaximize;
	MskResButton btSysClose;

	BOOL fMenu;
	//SysMenu menuSys;
};


#endif	// __SysMain_h
