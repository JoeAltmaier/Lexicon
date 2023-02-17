/* Burner.h -- Burn an SElement (animate then delete)
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
 *      1-30-20  JCA  Created
 *
**/

#pragma once

#ifndef __Burner_h
#define __Burner_h

#include "AppDefs.h"
#include "SElement.h"
#include "ACircularList_T.h"


class Burner {
public:
    typedef ACircularList_T<Burner> Node;

public:
    Burner(SElement* _pElmt, int _frame=0) : pElmt(_pElmt), bEnabled(false), node(this), frame(_frame) { }

    bool IsBurnerEnabled() const { return bEnabled; }
    void EnableBurner(bool _bEnable) { bEnabled = _bEnable; }

    bool IsBurning() const { return node.IsLinked(); }
    void StartBurner(Node& _listBurner, int _frameTo = 20);
    bool StepBurner();

    Burner* NextBurner() const { return node.NextElement(); }
    void LinkLastToBurn(Node& _list) { _list.LinkPrev(node); }
    void UnlinkBurner() { node.Unlink(); }

protected:
    void Blend(TBitmap& _bmCanvas, const TBitmap& _bmDissolve, const CRect& _rcTile, const CRect& _rcElmt, const CRect& _rcClip);

private:
    SElement* pElmt;
    Node node;
    bool bEnabled;
    int frame;
    int frameTo;
};

#endif      // __Burner_h

