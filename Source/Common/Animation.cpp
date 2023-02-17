// Animation.cpp - Lexicon tile for display
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-09-02  JAltmaier  Created
 *
**/
#include "Animation.h"
#include "Random.h"
#include "Sqrt.h"

BOOL Animation::Move(DRect &rectInv) {
	BOOL fInvalidate=false;

	// Update position

	switch(motion) {
	case MAtRest:		// No motion
	case MNull:			// Tile not in use
		break;

	case MSpinning:{
		// Update motion direction
		// cdHome is actually center of spin, not home square
		Coord cdRelative=cd - cdHome;

		S32 dx=cdRelative.x / 2 + Random::Value(5);
		S32 dy=cdRelative.y / 2 + Random::Value(5);
		// Spiral out: * 12 /10
		cdFinal=Coord(cdHome.x + (cdRelative.x - dy) * 12 / 10, cdHome.y + (cdRelative.y + dx) * 12 / 10);
		}
		// Falls through to MMoving...

	case MFloating:		// Mouse-controlled
	case MMoving:		// Straight-line motion toward cdFinal
		Coord cdDelta(CdFinal() - Cd());
		// Just moving, proceed at a reasonable speed
		S32 velocity=isqrt((cdDelta.x * cdDelta.x) + (cdDelta.y * cdDelta.y));
		if (velocity > speed) {
			cdDelta.x = (S16)(((S32)cdDelta.x) * speed / velocity);
			cdDelta.y = (S16)(((S32)cdDelta.y) * speed / velocity);
		}

		fInvalidate = (cdDelta.x != 0 || cdDelta.y != 0);

		if (fInvalidate)
			rectInv.Expand(cd.x, cd.y, size.x, size.y);

		Cd(Cd() + cdDelta);

		if (fInvalidate)
			rectInv.Expand(cd.x, cd.y, size.x, size.y);

		if (!fInvalidate && motion == MMoving)
			SetMotion(MAtRest, 0);

		break;
	}


	// Update animation state

	// If burning, change image
	// If done, reorder tiles
	if (Image() == IBurning) {
		cdOffset.y += size.y;
		iFrame--;
		if (iFrame == 0)
			SetState(INull);

		fInvalidate=true;
		rectInv.Expand(rect);
	}

	// If flashing, change image
	// If done, burn tiles
	if (Image() == IFlashing) {
		iFrame--;
		if (iFrame == 0)
			SetState(IBurning, nFrame);

		fInvalidate=true;
		rectInv.Expand(rect);
	}

	return fInvalidate;
}


// DdcUpdater virtual method

void Animation::Paint(DDc &ddc) {

	switch (Image()) {
	case ILetter:
		ddc.TransparentImage(
			Cd().x, Cd().y,
			size.x, size.y,
			ddcSrc,
			CdOffset().x, CdOffset().y,
			size.x, size.y,
			colorTransparent
			);
		break;

	case IFlashing:
		if (Frame() % 3)
			ddc.TransparentImage(
				Cd().x, Cd().y,
				size.x, size.y,
				ddcSrc,
				CdOffset().x, CdOffset().y,
				size.x, size.y,
				colorTransparent
				);
		break;

	case IBurning:
		if (Frame())
			ddc.TransparentImage(
				Cd().x, Cd().y,
				size.x, size.y,
				ddcSrc,
				CdOffset().x, CdOffset().y,
				size.x, size.y,
				colorTransparent
				);
		break;

	case INull:
		break;
	}
}
