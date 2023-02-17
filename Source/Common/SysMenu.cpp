/* SysMenu.cpp - Implementation of the SysMenu class
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

#include "AppAfx.h"

#include "SWnd.h"
#include "SysMenu.h"

// .Create
//
BOOL SysMenu::Create() 
{ 
	if (!CreatePopupMenu())
		return FALSE;

	AppendMenu(MF_ENABLED,IDC_SYSMENUABOUT,_T("About\tF1"));
	AppendMenu(MF_SEPARATOR,0,_T(""));

#if 0
	BOOL f1= options.CreatePopupMenu();
	AppendMenu(Registry::IsSnapTo() ? MF_CHECKED : MF_UNCHECKED,IDC_SYSMENUSNAPTO,_T("Snap-To Screen"));

	AppendMenu(MF_SEPARATOR,0,_T(""));
	AppendMenu(Registry::IsAlwaysOnTop() ? MF_CHECKED : MF_UNCHECKED,  IDC_SYSMENUALWAYSONTOP,_T("Always On Top"));
	AppendMenu(Registry::IsMultipleInstances() ? MF_CHECKED : MF_UNCHECKED,IDC_SYSMENUMULTIPLES,_T("Allow Multiple Instances"));

	//options.AppendMenu(MF_SEPARATOR,0,_T(""));
	//options.AppendMenu(!Registry::IsInSystemTray() ? MF_CHECKED : MF_UNCHECKED,  IDC_SYSMENUONTASKBAR,_T("Remote on Taskbar"));
	//options.AppendMenu(Registry::IsInSystemTray() ? MF_CHECKED : MF_UNCHECKED, IDC_SYSMENUINSYSTEMTRAY,_T("Remote in System Tray"));
	
	BOOL f2= options.AppendMenu(MF_POPUP,(UINT_PTR) GetSafeHmenu(),_T("Options"));

	options.AppendMenu(MF_SEPARATOR,0,_T(""));
	options.AppendMenu(MF_ENABLED,IDC_SYSMENUEXIT,_T("Exit"));
#endif
	return TRUE;
}

// .AppendToMenu
// 
// Append the this Menu to the end of the specified menu.
//
void SysMenu::AppendToMenu(CMenu *_pMenu)
{
	// IDM_ABOUTBOX must be in the system command range.
//	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
//	ASSERT(IDM_ABOUTBOX < 0xF000);
	return;
	if (_pMenu != NULL) {
		CString strAboutMenu;
		//strAboutMenu.LoadString(IDS_ABOUTBOX); //**
		//strAboutMenu = "&About";
		//if (!strAboutMenu.IsEmpty()) {
			_pMenu->InsertMenu(0,MF_BYPOSITION,MF_SEPARATOR);
			_pMenu->AppendMenu(MF_SEPARATOR);	//**
			_pMenu->AppendMenu(MF_POPUP,(UINT_PTR) GetSafeHmenu(),_T("Settings"));
//			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX,strAboutMenu);	//**
		//}
	}
}

// .OnMenuPopup
//
void SysMenu::OnMenuPopup(CWnd *_pWnd,CPoint _pt)
{
	//CheckMenuItem(IDC_SYSMENUSNAPTO,Registry::IsSnapTo() ? MF_CHECKED : MF_UNCHECKED);
	//CheckMenuItem(IDC_SYSMENUALWAYSONTOP,Registry::IsAlwaysOnTop() ? MF_CHECKED : MF_UNCHECKED);
	//CheckMenuItem(IDC_SYSMENUMULTIPLES,Registry::IsMultipleInstances() ? MF_CHECKED : MF_UNCHECKED);
	//CheckMenuItem(IDC_SYSMENUONTASKBAR,!Registry::IsInSystemTray() ? MF_CHECKED : MF_UNCHECKED);
	//CheckMenuItem(IDC_SYSMENUINSYSTEMTRAY,Registry::IsInSystemTray() ? MF_CHECKED : MF_UNCHECKED);

	::TrackPopupMenu(_pWnd->GetSystemMenu(false)->GetSafeHmenu(),TPM_LEFTALIGN,_pt.x,_pt.y,0,_pWnd->GetSafeHwnd(),nullptr);
	int err = GetLastError();
	//::TrackPopupMenuEx(menuSys.m_hMenu,TPM_LEFTALIGN | TPM_TOPALIGN,_pt.x,_pt.y,m_hWnd,NULL);

	_pWnd->PostMessage(WM_NULL, 0, 0);	// Fix for popup from Notification Icon so menu will cancel if clicking off the menu. 
}

// .OnSysMenuSnapTo
//
BOOL SysMenu::CheckSnapTo()
{ 
	BOOL fChecked = (GetMenuState(IDC_SYSMENUSNAPTO,MF_BYCOMMAND) & MF_CHECKED);

	CheckMenuItem(IDC_SYSMENUSNAPTO, ((fChecked) ? MF_UNCHECKED : MF_CHECKED) | MF_BYCOMMAND);
	Registry::SetSnapTo(!fChecked);

	return !fChecked;
}

// .OnSysMenuMultiples
//
BOOL SysMenu::CheckMultiples()
{ 
	UINT state = GetMenuState(IDC_SYSMENUMULTIPLES,MF_BYCOMMAND);
	if (state & MF_CHECKED) {
		Registry::SetMultipleInstances(FALSE);
		CheckMenuItem(IDC_SYSMENUMULTIPLES,MF_UNCHECKED | MF_BYCOMMAND);
	}
	else {
		Registry::SetMultipleInstances(TRUE);
		CheckMenuItem(IDC_SYSMENUMULTIPLES,MF_CHECKED | MF_BYCOMMAND);
	}
	return !(state & MF_CHECKED);
}

// .OnSysMenuAlwaysOnTop
//
BOOL SysMenu::CheckAlwaysOnTop()	
{ 
	UINT state = GetMenuState(IDC_SYSMENUALWAYSONTOP,MF_BYCOMMAND);
	if (state & MF_CHECKED) {
		Registry::SetAlwaysOnTop(FALSE);
		CheckMenuItem(IDC_SYSMENUALWAYSONTOP,MF_UNCHECKED | MF_BYCOMMAND);
	}
	else {
		Registry::SetAlwaysOnTop(TRUE);
		CheckMenuItem(IDC_SYSMENUALWAYSONTOP,MF_CHECKED | MF_BYCOMMAND);
	}
	return !(state & MF_CHECKED);
}

// .OnSysMenuOnTaskbar
//
BOOL SysMenu::CheckOnTaskbar()	
{ 
	UINT state = GetMenuState(IDC_SYSMENUONTASKBAR,MF_BYCOMMAND);
	if (!(state & MF_CHECKED)) {
		Registry::SetInSystemTray(FALSE);
		CheckMenuItem(IDC_SYSMENUONTASKBAR,MF_CHECKED | MF_BYCOMMAND);
		CheckMenuItem(IDC_SYSMENUINSYSTEMTRAY,MF_UNCHECKED | MF_BYCOMMAND);
	}
	return (state & MF_CHECKED) != 0;
}

// .OnSysMenuInSystemTray
//
BOOL SysMenu::CheckInSystemTray()	
{ 
	UINT state = GetMenuState(IDC_SYSMENUINSYSTEMTRAY,MF_BYCOMMAND);
	if (!(state & MF_CHECKED)) {
		Registry::SetInSystemTray(TRUE);
		CheckMenuItem(IDC_SYSMENUINSYSTEMTRAY,MF_CHECKED | MF_BYCOMMAND);
		CheckMenuItem(IDC_SYSMENUONTASKBAR,MF_UNCHECKED | MF_BYCOMMAND);
	}
	return !(state & MF_CHECKED);
}

