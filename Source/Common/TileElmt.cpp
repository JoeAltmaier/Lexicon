/* TileElmt.cpp -- Play area element
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


#include "TileElmt.h"
#include "TColorNames.h"
#include "SWnd.h"

#include <atlwin.h>


int TileElmt::nTiles = 0;

// .OnCreate
//
ERC TileElmt::OnCreate(SCreateStruct &_cs)
{
	ERC erc;

	Params *pParams;

	if ((pParams = (Params*)_cs.pContext) == nullptr)
		return ERR;

	if ((pChain = pParams->pChain) == nullptr)
		return ERR;
	
	if ((pBmTiles = pParams->pBmTiles) == nullptr)
		return ERR;

	if ((pBmDissolve = pParams->pBmDissolve) == nullptr)
		return ERR;

	bNoSwap = pParams->bNoSwap;
	ptCenter = CPoint(_cs.cx / 2, _cs.cy / 2);
	rcBmTile = pParams->rcBmTile;
	iTile = pParams->iTile;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	++nTiles;

	return OK;
}

// .OnDestroy
//
void TileElmt::OnDestroy()
{
	--nTiles;

	BaseClass::OnDestroy();
}



// .OnBlend
//
void TileElmt::OnBlend(TBitmap &_bmCanvas,const CRect &_rcElmt,const CRect &_rcClip)
{
		Burner::Blend(_bmCanvas, *pBmDissolve, rcBmTile, _rcElmt, _rcClip);
}
