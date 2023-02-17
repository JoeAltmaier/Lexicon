/* SysMain.h - Non-client area element
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Engineering, 2008,2009
 *
 * This material is a confidential trade secret and proprietary information of
 * Iowa Software Engineering, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of Iowa Software Engineering.
 * This material is also copyrighted as an unpublished work under sections 104 and
 * 408 of Title 17 of the United States Code. Law prohibits unauthorized use, 
 * copying or reproduction.
 *
 * Tom Nelson, PO Box 649, North Liberty,IA 52317, 319-936-6856
 * Iowa Software Engineering, 2 Hawkeye Dr. Ste 103, North Liberty,IA 52317, 319-626-3077
 *
 * Revision History:
 *      4-24-05  TRN  Created
 *
**/


#pragma once

#ifndef __SysMain_h
#define __SysMain_h

#include "AppDefs.h"
#include "SElement.h"
#include "SysMenu.h"

#include "AppSkin.h"


// Main Window Non-Client area element
//
class SysMain : public SElement {
public:
	SysMain() : fMenu(FALSE) { }

	BOOL Create(SElementRef *_pParent,const CRect &rect);
	virtual VOID OnBlend(TBitmap &_bmCanvas,const CRect &_rcArea,const CRect &_rcClip) { }

public:
	MskDecal ctrlBezel;
	MskMove  rgMove;

	MskMenuButton btSysMenu;
	MskButton btSysMinimize;
	MskButton btSysMaximize;
	MskButton btSysClose;

	BOOL fMenu;
	SysMenu menuSys;
};


#endif	// __SysMain_h
