/* MainApp.cpp - Main Application
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
 *      11-26-04  TRN  Created
 *
**/

#include "AppAfx.h"
#include "steam/steam_api.h"
#define atoll _atoi64

#include "ATimerService.h"

#include "AppDefs.h"
#include "MainApp.h"
#include "MainWnd.h"
#include "Registry.h"
#include "AboutDlg.h"

#include "AfxSock.h"
#include <winerror.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(MainApp, TWinApp)
//	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()



#if 0
Steamworks_SelfCheck();

// optional Steam input-device API
if (!SteamInput()->Init(false))
{
	... exit ...
}

#endif

extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText)
{
	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
	::OutputDebugString(pchDebugText);

	if (nSeverity >= 1)
	{
		// place to set a breakpoint for catching API errors
		int x = 3;
		(void)x;
	}
}

void MainApp::OnTimerSteam(Timer*)
{
	SteamGameServer_RunCallbacks();
}


// The one and only MainApp object

MainApp theApp;


// MainApp initialization

BOOL MainApp::InitInstance()
{
	ATimerService::StartService();

	Registry::LoadSettings();

	if (!Registry::IsMultipleInstances() && !IsFirstInstance(_T(APPWNDCLASS)))
		return FALSE;
  
	WNDCLASS wndcls;

    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

    wndcls.style = CS_HREDRAW | CS_VREDRAW;
#ifdef DESKTOP
    wndcls.style |= CS_BYTEALIGNCLIENT | CS_DBLCLKS;
#endif
    wndcls.lpfnWndProc = ::DefWindowProc;
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon = LoadIcon(_T("IDI_DEMO"));
    wndcls.hbrBackground =NULL; //(HBRUSH) GetStockObject(GRAY_BRUSH);
    wndcls.lpszMenuName = NULL; //MAKEINTRESOURCE(IDR_MAINFRAME);
#ifdef DESKTOP
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
#else
	wndcls.hCursor = NULL;
#endif
    // Specify your own class name for using FindWindow later
    wndcls.lpszClassName = _T(APPWNDCLASS);

    // Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls)) {
		AfxMessageBox(_T("Error: Class Registration Failed!"));
		return FALSE;
    }

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	INITCOMMONCONTROLSEX iccex; 

    /* INITIALIZE COMMON CONTROLS */
    iccex.dwICC = ICC_WIN95_CLASSES;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&iccex);


	TWinApp::InitInstance();

#ifdef DESKTOP
	if (!AfxSocketInit()) {
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
#endif

	// Steam initialization
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
	{
		return FALSE;
	}

	if (!SteamAPI_Init())
	{
		return FALSE;
	}

	SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);

	if (!SteamUser()->BLoggedOn())
	{
		return FALSE;
	}


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("Iowa Software Design"));

	pChildWnd = new MainWnd();
	if (!pChildWnd)
		return FALSE;

	m_pMainWnd = pChildWnd;


	DWORD dwExStyle = Registry::IsAlwaysOnTop() ? WS_EX_TOPMOST : 0;
	dwExStyle |= Registry::IsInSystemTray() ? WS_EX_TOOLWINDOW : 0;

	DWORD dwStyle = (WS_VISIBLE | WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX ); //(WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CAPTION | WS_THICKFRAME);

	CPoint ptWnd;
	Registry::GetWindowPos(ptWnd);

	CRect rcWnd = CRect(ptWnd,CSize(368,494));

	BOOL fCreated = pChildWnd->CreateEx(dwExStyle,_T(APPWNDCLASS), _T(APPNAME),dwStyle,rcWnd, NULL, 0);

//	S32 cyMenu = GetSystemMetrics(SM_CYMENU);
//	rect.5bottom += cyMenu;
	

	// The one and only window has been initialized, so show and update it
	if (fCreated) {
		pChildWnd->ShowWindow(SW_SHOW);
		pChildWnd->UpdateWindow();
	}
	else
		AfxMessageBox(_T("Error: Failed to create main window!"));

	return fCreated;
}

int  MainApp::ExitInstance()
{
	Registry::SaveSettings();

	delete pChildWnd;

	SteamAPI_Shutdown();

	return TWinApp::ExitInstance();
}

