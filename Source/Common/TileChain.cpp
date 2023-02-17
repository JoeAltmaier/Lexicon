/* TileChain.cpp -- List of TileElmt
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Design, 2008,2009,2020
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
 *      5-07-20 TRN  Created
 *
**/

#include "TileChain.h"
#include "TColorNames.h"


// .ForgeTileElmt
//
TileElmt *TileChain::ForgeTileElmt(int _tStyle, CRect _rcElmt, const TileElmt::Params _params)
{
    TileElmt *pTileElmt = new TileElmt;
    ERC erc;

    // Params overrides
    TileElmt::Params params = _params;
    params.pChain = this;
    params.rcBmTile = CRect(CPoint(sizeBmTile.cx * params.iTile, 0), sizeBmTile);
    params.pBmTiles = pBmTiles;
    params.pBmDissolve = pBmDissolve;

     if ((erc = pTileElmt->Create(_tStyle, _rcElmt, pParentElmt, 0xAB12, (void *)&params)) != OK) {
        delete pTileElmt;
        return nullptr;
    }
    pTileElmt->LinkLastTo(listTiles);

    return pTileElmt;
}

// .ForgeTileElmt
//
TileElmt *TileChain::ForgeTileElmt(int _tStyle, int _iTile, CPoint _ptTile)
{
    return ForgeTileElmt(_tStyle, CRect(_ptTile, sizeBmTile), TileElmt::Params(_iTile));
}

//static
TileElmt *TileChain::DuplicateTileElmt(int _tStyle, TileElmt *_pElmtTile)
{    
    TileChain *pChain = _pElmtTile->GetChain();
    TileElmt::Params params = _pElmtTile->GetParams();

    TileElmt *pElmtTile;

    if ((pElmtTile = pChain->ForgeTileElmt(_tStyle, _pElmtTile->GetRect(), params)) != nullptr)
       pElmtTile->MoveOrderBelow(_pElmtTile);

    return pElmtTile;
}


// .DestroyTiles
//
void TileChain::DestroyTiles()
{
    while (FirstTileElmt()) {
        FirstTileElmt()->DestroyElement();
        delete FirstTileElmt();
    }
}

