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
	// Construction
	SFontBox() : pCharset(NULL), pText(NULL), offset(0, 0) { }
	virtual ~SFontBox() { }

	BOOL Create(TBitmap *_pBmFontStrip, const char* _pCharset, TColor _transparent, CRect& _rect, SElementRef* _pParent, UINT _nID = 0, void* _pContext = NULL);

	// Text content
	void  SetText(const char* _pszText);

	BOOL IsOverText(CPoint _pt) const;

protected:
	void AdjustLayout(int _cx, int _cy);

protected://SFC
	virtual ERC  OnCreate(SCreateStruct& _cs);
	virtual void OnDestroy() { }
	virtual void OnSize(int _cx, int _cy);
	virtual void OnBlend(TBitmap& _bmCanvas, const CRect& _rcArea, const CRect& _rcClip);

protected:
	TBitmap *pBmFontStrip;
	char* pCharset;
	int cPixelPer;
	char* pText;
	CPoint offset;			// Pixel offset of text
};

#endif	// __SFontBox_h
