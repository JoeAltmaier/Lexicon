/* Glower.h -- Glow an SElement (animate)
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
 *      11-06-23  JCA  Created
 *
**/

#pragma once

#ifndef __Glower_h
#define __Glower_h

#include "AppDefs.h"
#include "SElement.h"
#include "ACircularList_T.h"


class Glower {
public:
    typedef ACircularList_T<Glower> Node;

public:
    Glower(SElement* _pElmt, int _frame = 0) : pElmt(_pElmt), bEnabled(false), node(this), frame(_frame) { }

    bool IsGlowerEnabled() const { return bEnabled; }
    void EnableGlower(bool _bEnable) { bEnabled = _bEnable; }

    bool IsGlowing() const { return node.IsLinked(); }
    void StartGlower(Node& _listGlower, int _frameTo = 20);
    bool StepGlower();

    Glower* NextGlower() const { return node.NextElement(); }
    void LinkLastToGlow(Node& _list) { _list.LinkPrev(node); }
    void UnlinkGlower() { node.Unlink(); }

    virtual void SetGlow(U8) = 0;

private:
    SElement* pElmt;
    Node node;
    bool bEnabled;
    int frame;
    int frameTo;
};

#endif      // __Glower_h

#pragma once
