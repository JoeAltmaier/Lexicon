/* MainApp.cpp - MainApp Application
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
 *      11-26-04  TRN  Created
 *
**/

#include "AppDefs.h"
#include "MainApp.h"
#include "MainWnd.h"
#include "Registry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MainApp, TWinApp)
END_MESSAGE_MAP()

// The one and only MainAppApp object

MainApp theApp;

// MainAppApp initialization

BOOL MainApp::InitInstance()
{
	Registry::LoadSettings();

	if (!Registry::IsMultipleInstances() && !IsFirstInstance(APPWNDCLASS))
		return FALSE;
  
	WNDCLASS wndcls;

    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

    wndcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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
    wndcls.lpszClassName = APPWNDCLASS;

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

#if 0
#ifdef DESKTOP
	if (!AfxSocketInit()) {
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
#endif
#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(COMPANYNAME);

	pChildWnd = new MainWnd;
	if (!pChildWnd)
		return FALSE;

	m_pMainWnd = pChildWnd;

	DWORD dwStyle = (WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CAPTION | WS_THICKFRAME);

	CPoint point;
	if (!Registry::GetWindowPos(point)) {
		point.x = 5;
		point.y = 10;
	}
	else {
		if (point.x < 0)
			point.x = 0;
		if (point.y < 0)
			point.y = 0;
	}

	CRect rect(0,0,1024,768);

	BOOL fCreated = pChildWnd->CreateEx(0,_T(APPWNDCLASS),_T(APPNAME),dwStyle,CRect(point,rect.Size()), NULL, 0);	
	

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

	return TWinApp::ExitInstance();
}

