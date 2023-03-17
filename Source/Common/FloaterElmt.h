/* FloaterElmt.h -- Floating points element
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

#ifndef __FloaterElmt_h
#define __FloaterElmt_h

#include "AppDefs.h"
#include "SysMenu.h"

#include "SFontBox.h"
#include "Slider.h"

#include "TBitmap.h"
#include "ACircularList_T.h"


class CLASS_ABASE2(FloaterElmt, public, SFontBox, public, Slider)
public:
	enum {	// Notifiations to parent element
		notifyRESET  = 1,
		notifyMOVE   = 2,
		notifyCOPY   = 3,
		notifyRELEASE= 4,
		notifyRETURN = 5
	};
	typedef ACircularList_T<FloaterElmt> Node;

public:
	FloaterElmt() : Slider(this), pChain(nullptr),node(this),nSpeed(2) { }
	~FloaterElmt()	{ }

public:
	
	FloaterElmt *NextFloaterElmt() const	{ return node.NextElement(); }
	void LinkLastTo(Node &_list)					{ _list.LinkPrev(node); }

	class FloaterChain *GetChain() const	{ return pChain;    }

	static int GetFloaterCount()			{ return cFloater; 	}

protected://SFC overrides
	virtual ERC  OnCreate(SCreateStruct &_cs) override;
	virtual void OnDestroy() override;

	virtual void OnBlend(TBitmap &bmCanvas, const CRect &_rcWnd, const CRect &_rcClip) override;

private:
	int nSpeed;
	CPoint ptHome;		// Home position when moving

	class FloaterChain *pChain;
	Node node;
	
	static int cFloater;
};

#endif	// __FloaterElmt_h
