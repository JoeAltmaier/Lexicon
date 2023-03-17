/* FloaterElmt.cpp -- Play area element
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


#include "FloaterElmt.h"
#include "TColorNames.h"
#include "SWnd.h"

#include <atlwin.h>

//
// Best practice is to never have an element change it's own placement or visibility.
// This includes anything that can effect the layout of the parent element.
//
// Instead notify the parent of the desired change and let the parent do it.
// This also allows the parent to adjust sibling elements effected by the change.
//

int FloaterElmt::cFloater = 0;

// .OnCreate
//
ERC FloaterElmt::OnCreate(SCreateStruct &_cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	++cFloater;

	return OK;
}

// .OnDestroy
//
void FloaterElmt::OnDestroy()
{
	--cFloater;

	BaseClass::OnDestroy();
}

// .OnBlend
//
void FloaterElmt::OnBlend(TBitmap &_bmCanvas,const CRect &_rcElmt,const CRect &_rcClip)
{
//    _bmCanvas.CopyFillColor(0x008000,_rcElmt,_rcClip);
    
    BaseClass::OnBlend(_bmCanvas,_rcElmt,_rcClip);
}
