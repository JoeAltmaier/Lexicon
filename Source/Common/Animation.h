// Animation.h - structure records display state of a Lexicon tile
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-09-02  JAltmaier  Created
 *
**/

#ifndef __Animation_h
#define __Animation_h

#include "LexTypes.h"
#include "Resource.h"

	enum {NFRAMEBURN=4, NFRAMEFLASH=10};
	typedef enum {ILetter, IFlashing, IBurning, INull} ImageState;
	typedef enum {MAtRest, MFloating, MMoving, MSpinning, MNull} MotionState;
	enum {SPEEDNEW=40, SPEEDFALL=18, SPEEDDRAG=80, SPEEDSWAP=60, SPEEDTEXT=8, SPEEDLOSE=6};
	enum { ZBACKGROUND, ZPICTURE, ZSCORES, ZTILESUNCACHED, ZTILESMOVING, ZTILESDRAG };

	typedef struct { U32 nBurn; U32 nSpin; U32 nMove; } MotionStats;

	class Animation { //: public DdcUpdater {
		MotionStats &mstat;

		Coord cdHome;		// Cell on board in mouse coordinates
		Coord cd;			// Current display location in mouse coordinates
		Coord cdFinal;		// Motion destination in mouse coordinates
		Coord cdOffset;		// Tile image location (column) in pixels
		U32 iFrame;			// Multi-frame animation progress, counts down
		U32 nFrame;			// Reset frame counter to this value
		ImageState image;	// Type of animation
		MotionState motion;	// Type of motion
		S32 speed;			// Motion throttle

		// Painting parameters
		Coord size;
		COLORREF colorTransparent;

	public:
		Animation(DDc &ddcSrc, U32 z, DRect &rect, Coord &_size, COLORREF _colorTransparent, MotionStats &_mstat)
			:DdcUpdater(ddcSrc, z, rect), size(_size), colorTransparent(_colorTransparent), mstat(_mstat), cd(-100,-100), image(INull), motion(MNull) { }

		const TCHAR *Name() { return TEXT("Animation"); }
		const Coord &CdHome() { return cdHome; }
		const Coord CdHome(const Coord &_cdHome) { Coord cdRet=cdHome; cdHome=_cdHome; return cdRet; }
		const Coord &Cd() { return cd; }
		const Coord Cd(const Coord &_cd) { 
			Coord cdRet=cd; 
			cd=_cd; 
			// Set rect so DdcUpdater knows where we are, for painting
			rect=CRect(cd.x, cd.y, cd.x + size.x, cd.y + size.y);
			return cdRet; 
		}
		const Coord &CdFinal() { return cdFinal; }
		const Coord CdFinal(const Coord &_cdFinal) { Coord cdRet=cdFinal; cdFinal=_cdFinal; return cdRet; }
		const Coord &CdOffset() { return cdOffset; }
		const Coord CdOffset(const Coord &_cdOffset) { Coord cdRet=cdOffset; cdOffset=_cdOffset; return cdRet; }

		void SetState(ImageState _image, U32 _iFrame=0, U32 _nFrame=0) { 
			if (_image != image) {
				if (image == IBurning)
					mstat.nBurn--;

				switch(_image) {
				case ILetter:
					break;
			
				case IFlashing:
					iFrame=_iFrame; 
					nFrame=_nFrame;
					mstat.nBurn++;
					break;
				
				case IBurning:
					iFrame=_iFrame; 
					nFrame=_nFrame;
					if (image != IFlashing)
						mstat.nBurn++;
					cdOffset.y=0;
					break;
				
				case INull:
					z=ZTILESUNCACHED; // reset to normal
					break;
				}

				image=_image; 
			}
		}

		void SetMotion(MotionState _motion, U8 _speed) {
			speed=_speed; 

			if (_motion != motion) {

				switch (_motion) {
				case MFloating:
					z=ZTILESDRAG;
					break;

				case MMoving:
					z=ZTILESMOVING;
					mstat.nMove++;
					break;

				case MSpinning:
					z=ZTILESMOVING;
					mstat.nSpin++;
					break;

				case MAtRest:
				case MNull:
					if (motion == MMoving)
						mstat.nMove--;
					if (motion == MSpinning)
						mstat.nSpin--;

					if (z == ZTILESMOVING)
						z=ZTILESUNCACHED;
					break;
				}
	
				motion=_motion; 
			}
		}

		ImageState Image() { return image; }
		U32 Speed() { return speed; }
		MotionState Motion() { return motion; }
		U32 Frame() { return iFrame; }
		U32 Z() { return z; }
		U32 Z(U32 _z) { z=_z; return z; }

		BOOL Move(CRect &rect);

		// DdcUpdater method
		void Paint(DDc &ddc);
	};

#endif
