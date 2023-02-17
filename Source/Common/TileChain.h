/* Chain.h -- List of TileElmt
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Design, 2008,2009
 *
 * This material is a confidential trade secret and proprietary information of
 * Iowa Software Design, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of Iowa Software Design.
 * This material is also copyrighted as an unpublished work under sections 104 and
 * 408 of Title 17 of the United States Code. Law prohibits unauthorized use,
 * copying or reproduction.
 *
 * Tom Nelson, PO Box 649, North Liberty,IA 52317, 319-936-6856
 * Iowa Software Design, PO Box 649, North Liberty,IA 52317, 319-626-3077
 *
 * Revision History:
 *      5-07-20  TRN  Created
 *
**/

#pragma once

#ifndef __TileChain_h
#define __TileChain_h

#include "AppDefs.h"
#include "TileElmt.h"

#include "ACircularList_T.h"


class TileChain {
public:
    TileChain(class SElement *_pParentElmt, TBitmap *_pBmTiles, TBitmap* _pBmDissolve, const CSize &_sizeBmTile) : pParentElmt(_pParentElmt), sizeBmTile(_sizeBmTile), pBmTiles(_pBmTiles), pBmDissolve(_pBmDissolve) { }
    ~TileChain() { ASSERT(listTiles.NextElement() == nullptr); }  // You must call DestroyTiles() before destructing!

public:
    TileElmt *ForgeTileElmt(int _tStyle, int _iTile, CPoint _ptTile);
    TileElmt *ForgeTileElmt(int _tStyle, CRect _rcElmt, TileElmt::Params _params);

    static TileElmt *DuplicateTileElmt(int _tStyle, TileElmt *_pElmtTile);

    void DestroyTiles();

public:
    CSize GetTilesize() const           { return sizeBmTile; }
    TileElmt *FirstTileElmt() const     { return listTiles.NextElement(); }

private:
    class SElement *pParentElmt;
    const TBitmap *pBmTiles;
    const CSize &sizeBmTile;
    const TBitmap *pBmDissolve;
    TileElmt::Node listTiles;   // TileElmt in this chain
};

#endif	// __TileChain_h
