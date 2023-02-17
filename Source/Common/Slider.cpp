/* Slider.cpp -- Slide an SElement to point
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

#include "Slider.h"

//*FIX* Need to be able to specify the stepsize
//*FIX* Need a way to fade the element


bool Slider::StartSlider(CPoint _ptTo, Node & _listSlide,int _nStepSize,bool _bNotifyHome)
{
	SetNotifyHome(_bNotifyHome);

	LinkLastToSlide(_listSlide);
	ptHome = _ptTo;

	x0 = pElmt->GetTopLeft().x;
	y0 = pElmt->GetTopLeft().y;
	x1 = _ptTo.x;
	y1 = _ptTo.y;
	
	//TRACE("Slide %d,%d to %d,%d\n", x0, y0, x1, y1);

	dX = x1 - x0;
	dY = y1 - y0;

	xDir = 1;
	if (dX < 0) {
		xDir = -1;
		dX = -dX;
	}
	yDir = 1;
	if (dY < 0) {
		yDir = -1;
		dY = -dY;
	}	
	//TRACE("dX,dY = %d,%d; xDir,yDir = %d,%d\n", dX, dY, xDir, yDir);

	// One jump if distance is less than STEPSIZE*2
	if (dY > dX) {
		bVertical = true;
		nStep = SplMax(dY/_nStepSize,1);
		int32_t nSize = SplMin(dY,_nStepSize);
		incX = ((REAL) dX / dY * nSize) * xDir;
		incY = (REAL) nSize * yDir;
	}
	else {
		bVertical = false;
		nStep = SplMax(dX/_nStepSize,1);
		int32_t nSize = SplMin(dX, _nStepSize);
		incX = (REAL) nSize * xDir;
		incY = ((REAL) dY / dX * nSize) * yDir;
	}
	acc = 0;
	//TRACE("%s incX,incY = %.2lf,%.2lf\n",bVertical ? "Vert": "Horz",incX,incY);

	return true;
}

bool Slider::StepSlider()
{
	if (bVertical) { // y-major line
		if (--nStep < 0) {
			pElmt->MoveElement(ptHome);
			return false;
		}
		y0 += (int32_t)incY;
		acc += incX;
		x1 = x0 + (int32_t)acc;
		//TRACE("Step Vert %d,%d  inc:%.2lf,%.2lf\n", x1, y0, incX,incY);
		pElmt->MoveElement(x1, y0);
	}
	else { // x-major line
		if (--nStep < 0) {
			pElmt->MoveElement(ptHome);
			return false;
		}
		x0 += (int32_t)incX;
		acc += incY;
		y1 = y0 + (int32_t)acc;
		//TRACE("Step Vert %d,%d  inc:%.2lf,%.2lf\n", x0, y1, incX, incY);
		pElmt->MoveElement(x0, y1);
	}
	return true;
}

