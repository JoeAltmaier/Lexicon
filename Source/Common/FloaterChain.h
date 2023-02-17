/* FloaterChain.h -- List of FloaterElmt
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

#ifndef __FloaterChain_h
#define __FloaterChain_h

#include "AppDefs.h"
#include "FloaterElmt.h"

#include "ACircularList_T.h"


class FloaterChain {
public:
    FloaterChain(class SElement *_pParentElmt) : pFont(nullptr),pParentElmt(_pParentElmt) { }
    ~FloaterChain()         { ASSERT(listFloater.NextElement() == nullptr); }  // You must call DestroyFloaters() before destructing!

    ERC  LoadFont(const char *_pszFontName,int _nFontHeight);
    void DestroyFloaters();

public:
    FloaterElmt *ForgeFloaterElmt(int _tStyle, CPoint _ptFloater,const char *_pszText);
    bool MakeFloatersTopmost();
    void SlideFloaters();
    
    Slider *FirstSlider() const		{ return listSlider.NextElement(); }
    Slider *LastSlider() const		{ return listSlider.PrevElement(); }
    bool IsEmpty() const { return listFloater.IsEmpty(); }

public:
    FloaterElmt *FirstFloaterElmt() const       { return listFloater.NextElement(); }

private:
    class SFont *pFont;
    class SElement *pParentElmt;
	FloaterElmt::Node listFloater;   // FloaterElmt in this chain
    Slider::Node listSlider;
};

#endif	// __FloaterChain_h
