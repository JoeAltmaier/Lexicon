/* AppMsg.h - Application Message Definitions
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
 *      1-17-05  TRN  Created
 *
**/

#pragma once

#ifndef __AppMsg_h
#define __AppMsg_h

//#include "stdafx.h"
#include "TTypes.h"

// Message Range Meaning 
//		0 through WM_USER–1 Messages reserved for use by the system. 
//		WM_USER through 0x7FFF Integer messages for use by private window classes. 
//		WM_APP through 0xBFFF Messages available for use by applications. 
//		0xC000 through 0xFFFF String messages for use by applications. 
//		Greater than 0xFFFF Reserved by the system for future use. 

#define WM_APP_VISTA	WM_APP+128

#define AM_SKINFIRST	WM_APP_VISTA
#define AM_SKINLAST		WM_APP_VISTA+63


// NetThread Messages
enum {
	WM_NET_MAPLOADED = WM_APP_VISTA+64,
	WM_NET_SITESELECT,
	WM_NET_PROGRESS,
	WN_NET_COUNT
};


#endif	// __AppMsg_h
