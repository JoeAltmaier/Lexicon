/* SysMenu.h - Implementation of the SysMenu class
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
 *      1-15-04  TRN  Created
 *
**/

#pragma once

#ifndef __SysMenu_h
#define __SysMenu_h

#include "AppDefs.h"
#include "Registry.h"

// IDs must be mod 10 since they may appear on the System Menu
//
#define IDC_SYSMENU						0x2710
#define IDC_SYSMINIMIZE					0x2720
#define IDC_SYSMAXIMIZE					0x2730
#define IDC_SYSCLOSE					0x2740

#define IDC_SYSMENUABOUT				0x3010
#define IDC_SYSMENUSNAPTO				0x3140
#define IDC_SYSMENUMULTIPLES			0x3150
#define IDC_SYSMENUALWAYSONTOP			0x3160
#define IDC_SYSMENUONTASKBAR			0x3170
#define IDC_SYSMENUINSYSTEMTRAY			0x3180
#define IDC_SYSMENUEXIT					0x3300


// Note: Must use .CreatePopupMenu() on PocketPC.
// Using .CreateMenu() for sub-menus does not work on PocketPC.
//
class SysMenu : public CMenu {
public:
	BOOL Create();

	void AppendToMenu(CMenu *_pMenu);
	void OnMenuPopup(CWnd *_pWnd,CPoint _pt);

	BOOL CheckSnapTo();
	BOOL CheckMultiples();
	BOOL CheckAlwaysOnTop();	
	BOOL CheckOnTaskbar();
	BOOL CheckInSystemTray();

//private:
	CMenu options;
};

#endif	// __SysMenu_h

