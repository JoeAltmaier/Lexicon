/* TileGridElmt.h -- Play Area element
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
 *      1-23-23  JCA  Added Mute, Pause
 *
**/


#pragma once

#ifndef __TileGridElmt_h
#define __TileGridElmt_h

#include "AppDefs.h"
#include "SysMenu.h"
#include "AppSkin.h"

#include "SElement.h"
#include "SFont.h"
#include "STextBox.h"

#include "TTimer_T.h"
#include "ACircularList_T.h"

#include "LexTypes.h"
#include "TileElmt.h"
#include "TileChain.h"
#include "FloaterChain.h"

#include "STextbox.h"
#include "TWaveDevice.h"
#include "TWaveFile.h"


// Main Window Non-Client area element
//
class CLASS_ABASE(TileGridElmt,public,SElement)
public:

	enum {
		nMARGINTOP  = 0,//14,
		nMARGINLEFT = 0,//14
		LOSESTEP = 4,
		SLIDESTEP = 20
	};

	typedef TTimerCallbackMethod_T<TileGridElmt> Timer;

public:
	TileGridElmt() : timerSlide(this, &TileGridElmt::OnTimerSlide), timerBurn(this, &TileGridElmt::OnTimerBurn), timerFloat(this, &TileGridElmt::OnTimerFloat),
		cTiles(0), cCol(0), cRow(0), cFrames(0), bGameOver(false), bMute(false), bPause(false), bLButtonDown(false), pElmtDrag(NULL), floaters(this), apChain(this, &bmTiles, &bmDissolve, sizeTile) { }

	~TileGridElmt() { }

	bool DropPlayTiles(U8 *);
	bool DropGameOver();
	void DestroyTiles();
	void MuteGame()					{ bMute = !bMute; }
	void PauseGame() { bPause = !bPause; }
	void PauseGame(bool _bPause) { bPause = _bPause; }
	bool IsPauseGame()				{ return bPause; }
	void FloatScore(const Coord& cd, S32 score);
	void StartAnimation(const Coord& cd);
	void AnimationIdle();

private:
	void OnTimerFloat(Timer* _pTimer);

	TileElmt* TileAt(CPoint _pt);
	void SlideTileElmt(Slider *_pSlider, CPoint _ptTo)	{ return SlideTileElmt(_pSlider,_ptTo,_pSlider->IsNotifyHome()); }
	void SlideTileElmt(Slider *_pSlider, CPoint _ptTo,bool _bNotifyHome);
	void OnTimerSlide(Timer *_pTimer);
	void Release(TileElmt* pElmtTile);

	Slider *FirstSlider() const		{ return listSlider.NextElement(); }
	Slider *LastSlider() const		{ return listSlider.PrevElement(); }

	void BurnTileElmt(Burner* _pBurner, int _frameTo);
	void OnTimerBurn(Timer* _pTimer);
	Burner* FirstBurner() const { return listBurner.NextElement(); }
	Burner* LastBurner() const { return listBurner.PrevElement(); }
	void PrepareDefaultDissolve();
	void LoseAnimation();

protected://SFC overrides
	virtual ERC  OnCreate(SCreateStruct &_cs) override;
	virtual void OnDestroy() override;

	virtual BOOL OnLButtonDown(UINT _nFlags, CPoint _point) override;		// Note: Must override Down to get DblClk
	virtual BOOL OnLButtonUp(UINT _nFlags, CPoint _point) override;
	virtual void OnMouseMove(UINT _nFlags, CPoint _point) override;

	virtual VOID OnBlend(TBitmap &_bmCanvas,const CRect &_rcElmt,const CRect &_rcClip) override;

private:
	uint32_t cFrames;

	bool bGameOver;
	bool bMute;
	bool bPause;
	bool bWin;
	bool bLose;

	bool bLButtonDown;
	CPoint ptDown;
	TileElmt* pElmtDrag;
	Timer timerSlide;
	Slider::Node listSlider;
	
	TBitmap bmTiles;
	CSize sizeTile;
	U32 nFrameBurn;
	TBitmap bmDissolve;
	Timer timerBurn;
	Burner::Node listBurner;

	int cTiles;
	int cRow,cCol;
	TileChain apChain;
	CRect rectTile;

	FloaterChain floaters;
	Timer timerFloat;

	TWaveFile   wavClack;
	TWaveFile   wavBurn;
	TWaveDevice devWave;
};


#endif	// __TileGridElmt_h
