/* Registry.cpp -- Application Registry Interface
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
 *      1-07-04  TRN  Created
 *
**/

#include "Registry.h"

// Application State
CPoint Registry::ptWinPos(0,0);

// Desktop Settings
BOOL Registry::fSnapTo = TRUE;
BOOL Registry::fMultiple = FALSE;
BOOL Registry::fInSystemTray = FALSE;
BOOL Registry::fAlwaysOnTop = FALSE;

// Application Specifc
BOOL Registry::fAutoConnect = TRUE;
UINT Registry::tRadar = 0;


ERC Registry::LoadSettings()
{
	// Desktop
	ReadValue(KEYDESKTOP,_T("SnapTo"),fSnapTo);
	ReadValue(KEYDESKTOP,_T("Multiple"),fMultiple);
	ReadValue(KEYDESKTOP,_T("InSystemTray"),fInSystemTray);
	ReadValue(KEYDESKTOP,_T("AlwaysOnTop"),fAlwaysOnTop);

	// Network
	ReadValue(KEYNETWORK,_T("AutoConnect"),fAutoConnect);

	// Current
	ReadValue(KEYCURRENT,_T("X"),ptWinPos.x);
	ReadValue(KEYCURRENT,_T("Y"),ptWinPos.y);

	return OK;
}

ERC Registry::SaveSettings()
{
	// Desktop
	WriteValue(KEYDESKTOP,_T("SnapTo"),fSnapTo);
	WriteValue(KEYDESKTOP,_T("Multiple"),fMultiple);
	WriteValue(KEYDESKTOP,_T("InSystemTray"),fInSystemTray);
	WriteValue(KEYDESKTOP,_T("AlwaysOnTop"),fAlwaysOnTop);

	// Network
	WriteValue(KEYNETWORK,_T("AutoConnect"),fAutoConnect);

	// Current
	WriteValue(KEYCURRENT,_T("X"),ptWinPos.x);
	WriteValue(KEYCURRENT,_T("Y"),ptWinPos.y);

	return OK;
}




