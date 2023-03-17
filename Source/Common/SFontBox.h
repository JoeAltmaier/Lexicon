/* SFontBox.h -- Image element
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
 *      2-14-23  JCA  Rewrote as SFontBox
 *
**/

#pragma once

#ifndef __SFontBox_h
#define __SFontBox_h

#include "SControl.h"


// SFontBox -- Image Box Control element
//
// This control implements miniumal functionality stand-along but provides
// enough support to implement much more complex derived text box controls.
//
class SFontBox: public SControl
{
public:
	typedef struct Context {
		TBitmap* pBmFontStrip;
		const char* pCharset;
		TColor transparent;

		Context(TBitmap* _pBmFontStrip, const char* _pCharset, TColor _transparent)
			: pBmFontStrip(_pBmFontStrip), pCharset(_pCharset), transparent(_transparent)
		{ };
		~Context() {  }
	} Context;

public:
	// Construction
	SFontBox() : context(NULL, NULL, 0), pText(NULL) { }
	virtual ~SFontBox() { if (pText) delete pText; }

	// Text content
	void  SetText(const char* _pszText);

	BOOL IsOverText(CPoint _pt) const;

protected:
	void AdjustLayout(int _cx, int _cy);

protected://SFC
	virtual ERC  OnCreate(SCreateStruct& _cs);
	virtual void OnDestroy() { }
	virtual void OnBlend(TBitmap& _bmCanvas, const CRect& _rcArea, const CRect& _rcClip);

protected:
	Context context;
	int cPixelPer;
	char* pText;
};

#endif	// __SFontBox_h
