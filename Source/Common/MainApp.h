/* MainApp.cpp - The main Application Class
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
 *      11-27-04  TRN  Created
 *
**/

#pragma once


#ifndef __MainApp_h
#define __MainApp_h

#include "AppDefs.h"
#include "TWinApp.h"
#include "TTimer_T.h"

// MainApp:
//
#define APPWNDCLASS		APPNAME

class MainWnd;

class MainApp : public TWinApp {
	typedef TTimerCallbackMethod_T<MainApp> Timer;
public:
	MainApp(): timerSteam(this, &MainApp::OnTimerSteam) { }

public:
	MainWnd *pChildWnd;
	Timer timerSteam;

// Overrides
public:
	virtual BOOL InitInstance();
	int  ExitInstance();

private:
	void OnTimerSteam(Timer*);

public:
	DECLARE_MESSAGE_MAP()
};

extern MainApp theApp;

#endif	// __MainApp_h
