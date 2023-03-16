/* SFontBox.cpp -- Textbox Element
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

#include "Appafx.h"

#include "SFontBox.h"

// .Create 
//
BOOL SFontBox::Create(TBitmap *_pBmFontStrip, const char* _pCharset, TColor _transparent, CRect& _rcElement, SElementRef* _pParent, UINT _nID, void* _pContext)
{
	pBmFontStrip = _pBmFontStrip;
	pCharset = (char*)_pCharset;
	cPixelPer = pBmFontStrip->Width() / strlen(pCharset);

	return SElement::Create(SElement::esVISIBLE,_rcElement,_pParent,_nID,_pContext) == OK;
}

// .OnCreate -- Initialize control
// 
// Derived class OnCreate methods must pass control here before calling
// any of this classes methods.
//
ERC SFontBox::OnCreate(SCreateStruct &_cs)
{
	return OK;
}

// .SetText
//
void SFontBox::SetText(const char *_pszText)
{
	pText = new char[strlen(_pszText) + 1];
	strcpy(pText, _pszText);

	Invalidate();
}

// .OnSize
//
void SFontBox::OnSize(int _cx,int _cy)
{
}

// .OnBlend
//
VOID SFontBox::OnBlend(TBitmap &_bmCanvas,const CRect &_rcWnd,const CRect &_rcClip)
{
	// Render onto bmCanvas the string pText character by character
	// Get the character image from a snippet of bmFontStrip
	CSize singleChar(cPixelPer, pBmFontStrip->Height());

	if (pText)
	for (unsigned int iCh = 0; iCh < strlen(pText); iCh++)
	{
		int chFont = strchr(pCharset, pText[iCh]) - pCharset;
		CPoint snippet(cPixelPer * chFont, 0);
		CPoint blendPt(_rcWnd.left + iCh * cPixelPer, _rcWnd.top);
		_bmCanvas.BlendFrom(*pBmFontStrip, CRect(snippet, singleChar), CRect(blendPt, singleChar));
	}
}

