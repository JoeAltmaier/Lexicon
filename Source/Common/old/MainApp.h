/* MainApp.cpp - The main Application Class
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
 *      11-27-04  TRN  Created
 *
**/

#pragma once


#ifndef __MainApp_h
#define __MainApp_h

#include "AppDefs.h"
#include "TWinApp.h"

// MainApp:
//
#define APPWNDCLASS		APPNAME

class MainApp : public TWinApp {
public:
	MainApp() { }

public:
	CWnd *pChildWnd;

// Overrides
public:
	virtual BOOL InitInstance();
	int  ExitInstance();

public:
	DECLARE_MESSAGE_MAP()
};

extern MainApp theApp;

#endif	// __MainApp_h
