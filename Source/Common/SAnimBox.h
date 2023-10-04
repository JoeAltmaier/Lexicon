/* SAnimBox.h -- Image element
 *
 * Copyright (c) Tom Nelson D.B.A. OpenVista Software, 2004,2012
 *
 * This material is a confidential trade secret and proprietary information of
 * OpenVista Software, which may not be reproduced, used, sold or transferred to
 * any third party without the prior written consent of OpenVista Software. This
 * material is also copyrighted as an unpublished work under sections 104 and 408
 * of Title 17 of the United States Code. Law prohibits unauthorized use, copying
 * or reproduction.
 *
 * Revision History:
 *      2-04-04  TRN  Created
 *		7-19-10  TRN  New implmention using STextLayout
 *      2-14-23  JCA  Rewrote as SAnimBox
 *
**/

#pragma once

#ifndef __SAnimBox_h
#define __SAnimBox_h

#include "SControl.h"


// SAnimBox -- Multi-frame Image Box Control element
//
//
class SAnimBox: public SControl
{
public:
	typedef struct Context {
		TBitmap* pBmAnimation;
		int nFrame;

		Context(TBitmap* _pBmAnimation, int _nFrame)
			: pBmAnimation(_pBmAnimation), nFrame(_nFrame)
		{ }
		~Context() {  }
	} Context;

public:
	// Construction
	SAnimBox() : context(NULL, 0), iFrame(0), cPixelPer(1) { }
	virtual ~SAnimBox() {  }

	void SetFrame(int _iFrame) { iFrame = _iFrame; Invalidate(); }

protected://SFC
	virtual ERC  OnCreate(SCreateStruct& _cs);
	virtual void OnDestroy() { }
	virtual void OnBlend(TBitmap& _bmCanvas, const CRect& _rcArea, const CRect& _rcClip);

protected:
	Context context;
	int iFrame;
	int cPixelPer;
};

#endif	// __SAnimBox_h
