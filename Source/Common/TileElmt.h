/* TileElmt.h -- The Tile element
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

#pragma once

#ifndef __TileElmt_h
#define __TileElmt_h

#include "AppDefs.h"
#include "SysMenu.h"

#include "SElement.h"
#include "SFont.h"
#include "STextBox.h"
#include "Slider.h"
#include "Burner.h"

#include "TBitmap.h"
#include "ACircularList_T.h"


class CLASS_ABASE3(TileElmt, public, SElement, public, Slider, public, Burner)
public:

	class Params {	// Tile create parameters
	public:
		Params(int _iTile) : pChain(nullptr),iTile(_iTile),rcBmTile(0,0,0,0),pBmTiles(nullptr),bNoSwap(false) { }
		Params(class TileChain *_pChain, int _iTile, int _tState, const CRect &_rcBmTile, const TBitmap *_pBmTiles, const TBitmap* _pBmDissolve, bool _bNoSwap=false) : pChain(_pChain), bNoSwap(_bNoSwap),iTile(_iTile), rcBmTile(_rcBmTile), pBmTiles(_pBmTiles), pBmDissolve(_pBmDissolve) { }

	public:
		class TileChain *pChain;
		bool bNoSwap;
		int iTile;
		CRect rcBmTile;
		const TBitmap* pBmTiles;
		const TBitmap* pBmDissolve;
	};
	typedef ACircularList_T<TileElmt> Node;

public:
	TileElmt() : Slider(this), Burner(this), pChain(nullptr),bNoSwap(false),node(this),iTile(0),rcBmTile(0,0,0,0),nSpeed(2){ }
	~TileElmt()	{ }

	Params GetParams() const			{ return Params(pChain, iTile, 0, rcBmTile, pBmTiles, pBmDissolve, bNoSwap); }

public:
	bool IsNoSwap() const				{ return bNoSwap; }
	bool SetNoSwap(bool _bSet)			{ return bNoSwap = _bSet; }

	TileElmt *NextTileElmt() const		{ return node.NextElement(); }
	void LinkLastTo(Node &_list)		{ _list.LinkPrev(node); }

	class TileChain *GetChain() const	{ return pChain;    }

	CPoint GetCenterPt() const			{ return GetTopLeft() + ptCenter;  }
	static int GetTileCount()			{ return nTiles; 	}

protected://SFC
	virtual ERC  OnCreate(SCreateStruct &_cs) override;
	virtual void OnDestroy() override;

	virtual void OnBlend(TBitmap &bmCanvas, const CRect &_rcWnd, const CRect &_rcClip) override;

private:
	CPoint ptCenter;
	bool bNoSwap;       // No swapping
	int iTile;

	CRect rcBmTile;
	const TBitmap* pBmTiles;
	const TBitmap* pBmDissolve;

	int nSpeed;
	CPoint ptHome;		// Home position when moving

	class TileChain *pChain;
	Node node;
	
	static int nTiles;
};

#endif	// __TileElmt_h
