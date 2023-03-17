/* SAnimBox.cpp -- Textbox Element
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

#include "Appafx.h"

#include "SAnimBox.h"


// .OnCreate -- Initialize control
// 
// Derived class OnCreate methods must pass control here before calling
// any of this classes methods.
//
ERC SAnimBox::OnCreate(SCreateStruct &_cs)
{
	context = *(Context*)_cs.pContext;
	cPixelPer = context.pBmAnimation->Height() / context.nFrame;

	return OK;
}


// .OnBlend
//
VOID SAnimBox::OnBlend(TBitmap &_bmCanvas,const CRect &_rcWnd,const CRect &_rcClip)
{
	// Render onto bmCanvas the frame selected
	// Get the image from a snippet of bmAnimation
	CSize singleFrame(context.pBmAnimation->Width(), cPixelPer);
	CPoint snippet(0, cPixelPer * iFrame);
	_bmCanvas.BlendFrom(*context.pBmAnimation, CRect(snippet, singleFrame), _rcWnd);
}

