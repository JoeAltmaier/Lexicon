/* Burner.cpp -- Burn an SElement
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Engineering, 2008,2020
 * Copyright (c) Iowa Software Design, 2023
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
 *      1-30-23  JCA  Created
 *
**/

#include "Burner.h"

void Burner::StartBurner(Node& _listBurner, int _frameTo) {
	LinkLastToBurn(_listBurner);
	frameTo = _frameTo;
	frame = 0;
	EnableBurner(true);
}

bool Burner::StepBurner() { frame++; if (frame >= frameTo) { UnlinkBurner(); return false; }  return true; }

void Burner::Blend(TBitmap& _bmCanvas, const TBitmap& _bmDissolve, const CRect& _rcTile, const CRect& _rcElmt, const CRect& _rcClip)
{
	CRect rcFrame(_rcTile);
	rcFrame.MoveToY(rcFrame.top + frame * _rcTile.Height());
	_bmCanvas.BlendFrom(_bmDissolve, rcFrame, _rcElmt, _rcClip);
}

