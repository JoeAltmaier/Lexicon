/* Registry.h -- Application Registry Interface
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
 *      1-07-05  TRN  Created
 *
**/

#pragma once

#ifndef __Registry_h
#define __Registry_h

#include "AppDefs.h"
#include "TRegistry.h"


#define KEYCURRENT	_T("SOFTWARE\\") COMPANYNAME _T("\\") APPNAME _T("\\Current")
#define KEYDESKTOP	_T("SOFTWARE\\") COMPANYNAME _T("\\") APPNAME _T("\\Settings\\Destktop")
#define KEYNETWORK  _T("SOFTWARE\\") COMPANYNAME _T("\\") APPNAME _T("\\Settings\\Network")


class Registry : public TRegistry {
public:
	static ERC LoadSettings();
	static ERC SaveSettings();

	// Application State
	static BOOL GetWindowPos(CPoint &_point)	{ _point = ptWinPos; return TRUE; }
	static VOID SetWindowPos(CPoint &_point)	{ ptWinPos = _point; }

	// Desktop Settings
	static BOOL IsSnapTo()						{ return fSnapTo;		}
	static VOID SetSnapTo(BOOL _fSnapTo)		{ fSnapTo = _fSnapTo;	}

	static BOOL IsInSystemTray()				{ return fInSystemTray;		}
	static VOID SetInSystemTray(BOOL _fInTray)	{ fInSystemTray = _fInTray;	}

	static BOOL IsMultipleInstances()					{ return fMultiple; }
	static VOID SetMultipleInstances(BOOL _fMultiple)	{ fMultiple = _fMultiple; }

	static BOOL IsAlwaysOnTop()					{ return fAlwaysOnTop;		}
	static VOID SetAlwaysOnTop(BOOL _fOnTop)	{ fAlwaysOnTop = _fOnTop;	}

	// Application Specific
	static BOOL IsAutoConnect()					{ return fAutoConnect;	 }
	static VOID SetAutoConnect(BOOL _fAuto)		{ fAutoConnect = _fAuto; }

	static UINT GetRadarType()					{ return tRadar;	}
	static VOID SetRadarType(UINT _tRadar)		{ tRadar = _tRadar; }

protected:
	// Application State
	static CPoint ptWinPos;

	// Desktop Settings
	static BOOL fSnapTo;
	static BOOL fInSystemTray;
	static BOOL fMultiple;
	static BOOL fAlwaysOnTop;

	// Application Specific
	static BOOL fAutoConnect;
	static UINT tRadar;
};


#endif	// __Registry_h
