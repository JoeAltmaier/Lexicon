/* BurnerChain.h -- List of BurnerElmt
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
 *      2/12/23  JCA  Created
 *
**/

#pragma once

#ifndef __BurnerChain_h
#define __BurnerChain_h

#include "AppDefs.h"
#include "BurnerElmt.h"

#include "ACircularList_T.h"


class BurnerChain {
public:
    BurnerChain(class SElement* _pParentElmt) : pFont(nullptr), pParentElmt(_pParentElmt) { }
    ~BurnerChain() { ASSERT(listBurner.NextElement() == nullptr); }  // You must call DestroyBurners() before destructing!

    ERC  LoadFont(const char* _pszFontName, int _nFontHeight);
    void DestroyBurners();

public:
    BurnerElmt* ForgeBurnerElmt(int _tStyle, CPoint _ptBurner, const char* _pszText);
    bool MakeBurnersTopmost();
    void SlideBurners();

    Slider* FirstSlider() const { return listSlider.NextElement(); }
    Slider* LastSlider() const { return listSlider.PrevElement(); }

public:
    BurnerElmt* FirstBurnerElmt() const { return listBurner.NextElement(); }

private:
    class SFont* pFont;
    class SElement* pParentElmt;
    BurnerElmt::Node listBurner;   // BurnerElmt in this chain
    Slider::Node listSlider;
};

#endif	// __BurnerChain_h
