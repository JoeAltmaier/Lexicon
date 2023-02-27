/* TileGridElmt.cpp -- Play area element
/* TileGridElmt.cpp -- Play area element
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
 *      1-23-23  JCA  Added Mute
 *
**/

#include "TileGridElmt.h"
#include "TColorNames.h"
#include "SWnd.h"
#include "resource1.h"
#include "Random.h"

// 50ms is 20 fps, 33ms is ~30 fps, 20ms is 50 fps , 16.7ms is ~60 fps
#define TICKSLIDE	30
#define TICKBURN	70
#define TICKFLOAT   30
#define MAXCOL		8
#define MAXROW		8

// .OnCreate
//
ERC TileGridElmt::OnCreate(SCreateStruct &_cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	if ((erc = wavClack.LoadResource("IDR_CLACK")) != OK)
		return erc;

	if ((erc = wavBurn.LoadResource(MAKEINTRESOURCE(IDW_BURN))) != OK)
		return erc;

	if ((erc = devWave.Open()) != OK)
		return erc;

	// Assumes bitmap is a horizontal strip of Rowed Tiles in square rectangles
	// so height of bitmap is also the width of each Tile in the strip.
	if ((erc = bmTiles.LoadPngResource("IDB_TILES")) != OK) {
		return ERR;
	}
	// There must be at least one full Tile rectangle.
	if ((cTiles = bmTiles.Width() / bmTiles.Height()) < 1) {
		return ERR;
	}

	cCol = SplMin(cTiles,MAXCOL);
	cRow = SplMin(cCol,MAXROW);

	// Size of each Tile in horizontal bitmap strip
	sizeTile = CSize(bmTiles.Height(), bmTiles.Height());

	if ((erc = floaters.LoadFont("Arial",20)) != OK) {
		return erc;
	}

	PrepareDefaultDissolve();

	return OK;
}

// .OnDestroy
//
void TileGridElmt::OnDestroy()
{
	floaters.DestroyFloaters();

	DestroyTiles();

	//delete pFont; pFont = nullptr;

	devWave.Close();
	
	BaseClass::OnDestroy();
}

// .DestroyTiles -- Destroy all Tiles in all chains
//
void TileGridElmt::DestroyTiles()
{
	// Delete all Tiles in chain
	apChain.DestroyTiles();
}

// .DropPlayTiles -- Place default Tiles
//
bool TileGridElmt::DropPlayTiles(U8 *letters)
{
	TileElmt *pElmtTile;

	int xTile = nMARGINLEFT;
	int yTile = nMARGINTOP;
	int nLastRow = yTile + cRow * bmTiles.Height();

	// Place one initial Tile in each Row chain
	for (int iRow = 0; iRow < cRow; iRow++) {
		for (int iCol = 0; iCol < cCol; iCol++) {
			int iTile = letters[iRow * cCol + iCol];
			if ((pElmtTile = apChain.ForgeTileElmt(esVISIBLE | esNOPARENTCLIP, iTile - 'a', CPoint(xTile, (yTile - nLastRow - iCol * bmTiles.Height())))) == nullptr)
				return FALSE;

			SlideTileElmt(pElmtTile,CPoint(xTile,yTile),(iRow == cRow-1));

			xTile += pElmtTile->Width();
		}
		xTile = nMARGINLEFT;
		yTile += pElmtTile->Height();
	}

	rectTile = pElmtTile->GetRect();

	return TRUE;
}

// .DropGameOver
//
bool TileGridElmt::DropGameOver()
{
	char aiTiles[] = { 'G','A','M','E','O','V','E','R',0 };
	int cLines = 2;

	int nWidth = bmTiles.Height(); // width of one tile, square (not width of entire tile filmstrip)
	int nHeight = bmTiles.Height();

	TileElmt *pElmtTile;
	
	int xTile = nMARGINLEFT;
	int yTile = nMARGINTOP;
	int nLastRow = yTile + cRow * nHeight;

	// Place one initial Tile in each Row chain
	int iTile = 0;
	for (int iRow = cRow-cLines; iRow < cRow; iRow++) {
		for (int iCol = 2; iCol <= 5; iCol++) {
			CPoint ptStart= CPoint(xTile + iCol*nWidth, iRow*nHeight - nLastRow - iCol * nHeight);
			CPoint ptHome = CPoint(xTile + iCol*nWidth, yTile+iRow*nHeight);// - nLastRow - iCol * nHeight));

			if ((pElmtTile = apChain.ForgeTileElmt(esVISIBLE | esNOPARENTCLIP, aiTiles[iTile++]-'A', ptStart )) == nullptr)
				return false;

			pElmtTile->SetNoSwap(true);
			SlideTileElmt(pElmtTile,ptHome,(iRow == cRow-1));
		}
	}
	
	return true;
}

void TileGridElmt::FloatScore(const Coord& cd, S32 score) 
{
	// cd is tile coord
	// make cd into pixel coord; score into text
	char aScore[32];
	sprintf_s(aScore, "%d", score);
	floaters.ForgeFloaterElmt(esVISIBLE, CPoint(cd.x * sizeTile.cx, cd.y * sizeTile.cy), aScore); 
	timerFloat.Start(TICKFLOAT);
}

void TileGridElmt::OnTimerFloat(Timer* _pTimer)
{
	floaters.SlideFloaters();

	if (listSlider.IsEmpty() && floaters.IsEmpty())
		NotifyParent(notifySTILL, 0);
	else
		timerFloat.Start(TICKFLOAT);
}

void TileGridElmt::StartAnimation(const Coord& cd)
{
	TileElmt* pElmtTile = TileAt(CPoint(cd.x * sizeTile.cx, cd.y * sizeTile.cy));
	if (pElmtTile) BurnTileElmt(pElmtTile, nFrameBurn);
}

TileElmt* TileGridElmt::TileAt(CPoint _pt)
{
	// Search apChain at row containing _pt.y for tile near _pt.x
	TileElmt* pElmt = apChain.FirstTileElmt();
	while (pElmt)
	{
		// May be in motion. Identify by where it calls home
		CPoint ptHome = pElmt->GetSliderHomePt();
		CPoint ptBottomRight = ptHome; ptBottomRight.Offset(CSize(rectTile.Width(), rectTile.Height()));
		if (CRect(ptHome, ptBottomRight).PtInRect(_pt))
			return pElmt;
		pElmt = pElmt->NextTileElmt();
	}
	return NULL;
}

// .SlideTile
//

void TileGridElmt::SlideTileElmt(Slider *_pSlider,CPoint _ptTo,bool _bNotifyHome)
{
	_pSlider->StartSlider(_ptTo, listSlider, SLIDESTEP, _bNotifyHome);

	timerSlide.Start(TICKSLIDE);
}

void TileGridElmt::OnTimerSlide(Timer *_pTimer)
{
	Slider *pSliderNext;

	for (Slider *pSlider = FirstSlider(); pSlider != nullptr; pSlider = pSliderNext) {
		pSliderNext = pSlider->NextSlider();

		if (!pSlider->StepSlider()) {
			pSlider->UnlinkSlide();
			if (pSlider->IsNotifyHome()) {
				if (!bMute)
					devWave.Play(&wavClack);
			}
			else
				pSlider->SetNotifyHome(true);
		}
	}

	if (listSlider.IsEmpty() && floaters.IsEmpty())
		NotifyParent(notifySTILL, 0);
	else
		timerSlide.Start(TICKSLIDE);
}

void TileGridElmt::Release(TileElmt* pElmtTile)
{
	TileElmt* pElmtUnder;
	CPoint ptElmt;
	CPoint ptElmtSwap;

	// Not a valid swap?
	if ((pElmtUnder = (TileElmt*)FindChildElementAt(pElmtTile->GetCenterPt(), pElmtTile)) == nullptr || pElmtUnder->GetId() != 0xAB12 || pElmtUnder->IsNoSwap()) {
		SlideTileElmt(pElmtTile, pElmtTile->GetSliderHomePt());
		return;
	}

	pElmtUnder->MoveOrderTop();
	pElmtTile->MoveOrderTop();

	if (pElmtUnder->IsSliding()) {
		ptElmt = pElmtUnder->GetSliderHomePt();
		SlideTileElmt(pElmtUnder, pElmtTile->GetSliderHomePt());
		SlideTileElmt(pElmtTile, ptElmt);
	}
	else {
		ptElmt = pElmtTile->GetSliderHomePt();
		ptElmtSwap = pElmtUnder->GetSliderHomePt();
		SlideTileElmt(pElmtTile, pElmtUnder->GetSliderHomePt());
		SlideTileElmt(pElmtUnder, ptElmt);

		// wParam is ptr to double cell coord for swap
		NotifyParent(notifyRELEASE, (LPARAM)new SwapTiles(Coord(ptElmt.x / pElmtTile->Width(), ptElmt.y / pElmtTile->Height()), Coord(ptElmtSwap.x / pElmtUnder->Width(), ptElmtSwap.y / pElmtUnder->Height())));
	}

	floaters.MakeFloatersTopmost();
}

BOOL TileGridElmt::OnLButtonDown(UINT _nFlags, CPoint _point) 
{
	if (!bPause && !bGameOver)
	{
		pElmtDrag = TileAt(_point);
		if (pElmtDrag)
		{
			bLButtonDown = true;
			ptDown = pElmtDrag->GetTopLeft() - _point;		// Mouse position relative to upper left of this element

			if (pElmtDrag->IsSliderEnabled()) {
				pElmtDrag->SetSliderHomePt(pElmtDrag->GetTopLeft());
				pElmtDrag->EnableSlider(false);	// No sliding when dragging
			}
		}
	}

	return TRUE;
}

BOOL TileGridElmt::OnLButtonUp(UINT _nFlags, CPoint _point)
{
	bLButtonDown = false;
	if (pElmtDrag)
	{
		Release(pElmtDrag);

		pElmtDrag->EnableSlider(true);

		pElmtDrag = NULL;
	}

	return TRUE;
}


// .OnMouseMove -- Drag Tiles
//
// Request to move element by specified relative pixels
//
void TileGridElmt::OnMouseMove(UINT _nFlags, CPoint _point)
{
	if (bLButtonDown)
	{
		CPoint ptElmt = ptDown - (pElmtDrag->GetTopLeft() -_point);
		pElmtDrag->UnlinkSlide();
		pElmtDrag->MoveOrderTop();
		pElmtDrag->MoveElement(ptElmt + pElmtDrag->GetTopLeft(), true);
	}

	BaseClass::OnMouseMove(_nFlags, _point);
}

// .PrepareDefaultDissolve -- Create progressive-burn animation from original tiles
//
void TileGridElmt::PrepareDefaultDissolve()
{
	// t is the source bitmap - a row of square alphabet tiles
	// nFrameBurn is now many frames of dissolve to create
	// Return a bitmap of tile rows, one tile per letter per dissolve

	nFrameBurn = 20;

	// Start dissolve with tile image in first row
	bmDissolve.Create(bmTiles.Width(), bmTiles.Height() * nFrameBurn, TColor(0, 0, 255));
	CRect rectTiles(0, 0, bmTiles.Width(), bmTiles.Height());
	bmDissolve.CopyFrom(bmTiles, rectTiles, rectTiles, rectTiles, 0, 0);

	// Loop over rows of tile masks
	// Each row takes the previous row and applies even more masking

	for (U32 iFrame = 1; iFrame < nFrameBurn; iFrame++) {
		U32 y0 = iFrame * sizeTile.cy;

		// Initialize frame with previous frame
		CRect rectFrom(0, y0 - sizeTile.cy, bmTiles.Width(), y0);
		CRect rectTo(0, y0, bmTiles.Width(), y0 + sizeTile.cy);
		bmDissolve.CopyFrom(bmDissolve, rectFrom, rectTo, rectTo);

		// Loop over bits
		for (U32 y = y0; y < y0 + sizeTile.cy; y++) {
			for (S32 x = 0; x < bmTiles.Width(); x++) {
				U8 mask = Random::Range(0, 255) | Random::Range(0, 255);
				TColor& color = *bmDissolve.GetColorPtr(x, y);
				color.red &= mask;
				color.green &= mask;
				color.blue &= mask;
			}
		}
	}

}

void TileGridElmt::BurnTileElmt(Burner* _pBurner, int _frameTo)
{
	_pBurner->StartBurner(listBurner, _frameTo);
	if (!bMute)
		devWave.Play(&wavBurn);

	timerBurn.Start(TICKBURN);
}

void TileGridElmt::OnTimerBurn(Timer* _pTimer)
{
	Burner* pBurnerNext;

	for (Burner* pBurner = FirstBurner(); pBurner != nullptr; pBurner = pBurnerNext) {
		pBurnerNext = pBurner->NextBurner();

		if (pBurner->StepBurner())
			((TileElmt*)pBurner)->Invalidate();
		else
		{
			((TileElmt*)pBurner)->DestroyElement();
			delete (TileElmt*)pBurner;
		}
	}

	if (listBurner.IsEmpty())
		NotifyParent(notifyIDLE, 0); // This will come back to us as a call to AnimationIdle
	else
		timerBurn.Start(TICKBURN);
}

void TileGridElmt::AnimationIdle()
{
	// All burning has ceased. Time to drop tiles to squash gaps

	int xTile = nMARGINLEFT;

	// Search from bottom up for blanks
	for (int iCol = 0; iCol < cCol; iCol++) {
		int yTile = nMARGINTOP + cRow * rectTile.Height();
		int yTarget = -1;
		for (int iRow = cRow - 1; iRow >= 0; iRow--) {
			yTile -= rectTile.Height();

			TileElmt* pElmtTile;
			if ((pElmtTile = TileAt(CPoint(xTile, yTile))) == 0)
			{
				// Empty space. Fill it if it's the bottom (first) one
				if (yTarget == -1)
					yTarget = yTile;
			}
			else
			{
				if (yTarget >= 0) // if a blank exists below this tile
				{
					SlideTileElmt(pElmtTile, CPoint(xTile, yTarget));
					yTarget -= rectTile.Height();
				}
			}
		}

		xTile += rectTile.Width();
	}

	return;
}

void TileGridElmt::LoseAnimation() {
	bLose = true;

	TileElmt* pElmtTile;

	pElmtTile = apChain.FirstTileElmt();
	while (pElmtTile)
	{
		BurnTileElmt(pElmtTile, nFrameBurn);
		CPoint ptCenter = this->GetRect().CenterPoint();
		CPoint radii(CPoint(pElmtTile->GetCenterPt() - ptCenter));
		pElmtTile->StartSlider(CPoint(ptCenter.x + radii.x * 10, ptCenter.y + radii.y * 10), listSlider, LOSESTEP);
		pElmtTile = pElmtTile->NextTileElmt();
	}

	timerSlide.Start(TICKSLIDE);

}



// .OnBlend
//
void TileGridElmt::OnBlend(TBitmap& bitmap, const CRect& _rcWnd, const CRect& _rcClip)
{
	// Nothing to paint ourselves
}
