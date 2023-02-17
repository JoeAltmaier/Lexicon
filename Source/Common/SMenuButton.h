/* SMenuButton.h - System Menu Button
 *
 * Copyright (c) Tom Nelson, 2004-2007
 * Copyright (c) Iowa Software Engineering, 2008
 *
 * This material is a confidential trade secret and proprietary information of
 * Iowa Software Engineering, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of Iowa Software Engineering.
 * This material is also copyrighted as an unpublished work under sections 104 and
 * 408 of Title 17 of the United States Code. Law prohibits unauthorized use, 
 * copying or reproduction.
 *
 * Revision History:
 *      11-17-04  TRN  Created
 *
**/

#pragma once

#ifndef __SMenuButton_h
#define __SMenuButton_h

//#include "VistaDefs.h"
#include "SButton.h"
#include "ATimer.h"

// SMenuButton class.
// 
// Intercepts mouse LClicks and LDblClicks looking for a DblClick to close the application.
// Normal LDblClick first sends the LDown/LUp events which brings up the Meteor Menu and
// interferes with the LDblClick.
//
class SMenuButton : public SButton, public ATimer {
public:
	SMenuButton() : fExpired(FALSE),fLButtonUp(TRUE) { }

	// On LButtonUp start a timer equal to some about more than the interval 
	// returned by UINT GetDoubleClickTime(VOID). 
	virtual BOOL OnLButtonDown(UINT _nFlags,CPoint _point) {
		fExpired = fLButtonUp = FALSE;
		return SButton::OnLButtonDown(_nFlags,_point);
	}
	//*** Need to change this so the timer starts onLButtonUp().
	//*** If the timer expires then do the OnLButtonUp()
	// If the timer is expired on LButtonUp then send the LClick message to the app window.
	virtual BOOL OnLButtonUp(UINT _nFlags,CPoint _point) {
		Start(GetDoubleClickTime()+1,0);
		//SButton::OnLButtonUp(nFlags,point);	//**HACK - maybe a 
		fLButtonUp = TRUE;
		nFlags = _nFlags;
		point = _point;
		return TRUE;
	}
	// On LButtonDblClk cancel the timer and send the LDblClick message to the app window.
	virtual BOOL OnLButtonDblClk(UINT _nFlags,CPoint _point) {
		Stop();
		fExpired = fLButtonUp = FALSE;
		NotifyLButtonDblClicked(_point);
		SButton::OnLButtonUp(_nFlags,_point);
		return TRUE;
	}

	void OnTimerExpired() { 
		if (fLButtonUp)
			SButton::OnLButtonUp(nFlags,point);	// Doesn't work here - maybe a MouseMove/IsOver problem in SButton
	}

protected:
	BOOL fExpired;
	BOOL fLButtonUp;
	UINT nFlags;
	CPoint point;
};


#endif	// __SMenuButton_h

