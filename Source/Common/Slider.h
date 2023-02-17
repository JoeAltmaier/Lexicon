/* Slider.h -- Slide an SElement to point
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Engineering, 2008,2020
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
 *      5-20-20  TRN  Created
 *
**/

#pragma once

#ifndef __Slider_h
#define __Slider_h

#include "AppDefs.h"
#include "SElement.h"
#include "ACircularList_T.h"


class Slider {
public:
    typedef ACircularList_T<Slider> Node;
    enum { STEPSIZE	= 20 };

public:
    Slider(SElement *_pElmt) : pElmt(_pElmt),bEnabled(true),bNotifyHome(false),node(this) { }

    bool IsSliderEnabled() const        { return bEnabled;        }
    void EnableSlider(bool _bEnable)    { bEnabled = _bEnable ;   }

    bool IsSliding() const              { return node.IsLinked(); }
	bool StartSlider(CPoint _ptTo, Node &_listSlider,int _nStepSize=STEPSIZE,bool _bNotifyHome=false);
	bool StepSlider();

    bool IsNotifyHome() const           { return bNotifyHome; }
    bool SetNotifyHome(bool _bSet)      { return bNotifyHome = _bSet; }

    void   SetSliderHomePt(CPoint _pt)  { if (!IsSliding()) { ptHome = _pt; } }
    CPoint GetSliderHomePt() const      { return (IsSliding() || !IsSliderEnabled()) ? ptHome : pElmt->GetTopLeft(); }

    Slider *NextSlider() const          { return node.NextElement(); }
    void LinkLastToSlide(Node &_list)   { _list.LinkPrev(node); }
    void UnlinkSlide()                  { node.Unlink(); }
    operator SElement* () const { return pElmt; }
    bool IsHere(CPoint _pt) const { return pElmt->RectElement().PtInRect(_pt); }

private:
    SElement *pElmt;
    Node node;
    CPoint ptHome;
    bool bEnabled;
    bool bNotifyHome;
    
    CPoint ptTo;
	int dX, dY;
	int x0, y0;
	int x1, y1;
	int xDir, yDir;
    bool bVertical;
    int32_t nStep;
    REAL acc;
	REAL incX;
	REAL incY;
};

#endif      // __Slider_h
