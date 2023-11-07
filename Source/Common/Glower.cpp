/* Glower.cpp -- Burn an SElement
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
 *      1-30-23  JCA  Created
 *
**/

#include "Glower.h"

void Glower::StartGlower(Node& _listGlower, int _frameTo) {
	LinkLastToGlow(_listGlower);
	frameTo = _frameTo;
	frame = 0;
	EnableGlower(true);
}

bool Glower::StepGlower() { frame++; if (frameTo) SetGlow(127 + 128 * frame / frameTo);  if (frame >= frameTo) { UnlinkGlower(); return false; } return true; }


