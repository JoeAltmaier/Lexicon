/* SysMain.cpp - Non-client area element
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

#include "SysMain.h"


BOOL SysMain::Create(SElementRef *_pParent,const CRect &_rect)
{
	SElement::Create(_rect,_pParent);

	//erc = bmBezel.LoadPngFile("Bezel1024.png","skin\\");
	//erc = ctrlBezel.Create(&bmBezel,CPoint(0,0),this);

	ctrlBezel.Create(this,CPoint(0,0),"Bezel1024.png");

	CRect rcClient;
	GetParent()->GetElementRect(rcClient);

	rgMove.Create(this,CRect(15,0,rcClient.Width()-44,19));

	btSysMenu.Create(this,"BmSysMenuStrip.png", CPoint(5,4),IDC_SYSMENU);
	btSysMinimize.Create(this,"BmSysMinimizeStrip.png", CPoint(rcClient.Width()-44,4),IDC_SYSMINIMIZE);
	btSysMaximize.Create(this,"BmSysMaximizeStrip.png", CPoint(rcClient.Width()-31,4),IDC_SYSMAXIMIZE);
	btSysClose.Create(this,"BmSysCloseStrip.png", CPoint(rcClient.Width()-17,4),IDC_SYSCLOSE);

	fMenu = menuSys.Create();

	return TRUE;
}



