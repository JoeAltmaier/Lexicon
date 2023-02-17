/* AppSkin.cpp - Derived Skin Classes
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Engineering, 2008,2009
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
 *      4-21-05  TRN  Created
 *
**/


#pragma once

#ifndef __AppSkin_h
#define __AppSkin_h

#include "SkinX.h"
#include "Registry.h"


class MskBase {
public:
	enum { sPATH=128 };

	void LoadPngFile(const char *_pszBmFile,TBitmap &_bm) {
		ERC erc; 
		if ((erc = _bm.LoadPngFile(_pszBmFile,GetPath())) != OK)  {
			char szTmp[128];
			sprintf(szTmp,"Unable to load bitmap from \"%s\\%s\" file.",GetPath(),_pszBmFile);
			//throw MeteorExceptionSkinFailedToLoadBitmap(szTmp);
		}
	}
	void LoadPngFile(const char *_pszBmFile,TBitmap &_bm,const CRect &_rcSegment) {
		ERC erc; 
		if ((erc = _bm.LoadPngFile(_rcSegment,_pszBmFile,GetPath())) != OK)  {
			char szTmp[128];
			sprintf(szTmp,"Unable to load cropped bitmap from \"%s\\%s\" file.",GetPath(),_pszBmFile);
			//throw MeteorExceptionSkinFailedToLoadBitmap(szTmp);
		}
	}

public:
	static void SetPath(const char *_pszPath) { if (_pszPath) strcpy(szPath,_pszPath); else strcpy(szPath,""); }
	static const char *GetPath()	{ return szPath; }

public:
	static char szPath[sPATH+1];
};


class MskButton : public MskBase, public SButton {
public:
	void Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _id) {
		LoadPngFile(_pszBmFile,bitmap);
		SButton::Create(&bitmap,_point,_pParent,_id);
	}
	void Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _flags,UINT _nThemes,UINT _id) {
		LoadPngFile(_pszBmFile,bitmap);
		SButton::Create(&bitmap,_flags,_nThemes,_point,_pParent,_id);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
};

class TitleBar : public SStatic {
public:
	virtual void OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized) { SetRow(nState != WA_INACTIVE); }
};

class MskTitleBar : public MskBase, public TitleBar {
public:
	void Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _nImages) {
		LoadPngFile(_pszBmFile,bitmap);
		TitleBar::Create(&bitmap,_nImages,_point,_pParent);
	}
	void Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,CSize &_dims) {
		LoadPngFile(_pszBmFile,bitmap);
		TitleBar::Create(&bitmap,_dims,_point,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
};


class MskMenuButton : public MskBase, public SMenuButton {
public:
	void Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _id) {
		LoadPngFile(_pszBmFile,bitmap);
		SMenuButton::Create(&bitmap,_point,_pParent,_id);
	}
//	void Create(SWnd *_pParent,const char *_pszBmFile,CPoint &_point,UINT _id) {
//		LoadPngFile(_pszBmFile,bitmap);
//		SMenuButton::Create(&bitmap,_point,_pParent,_id);
//	}
	void Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _flags,UINT _nThemes,UINT _id) {
		ERC erc; 
		if ((erc = bitmap.LoadPngFile(_pszBmFile,GetPath())) != OK) 
			return; //throw MeteorExceptionSkinFailedToLoadBitmap("Unable to open Button bitmap file.");

		SMenuButton::Create(&bitmap,_flags,_nThemes,_point,_pParent,_id);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
};

class MskScrollBar : public MskBase, public SScrollBar {
public:
//	void Create(SWnd *_pSkinWnd,const char *_pszBmFile,const CSize & _size,int _dwStyle,const CRect &_rect) {
//		LoadPngFile(_pszBmFile,bitmap);
//		SScrollBar::Create(&bitmap,_size,_dwStyle,_rect,_pSkinWnd,0);
//	}
	void Create(SElementRef *_pParent,const char *_pszBmFile,const CSize & _size,int _dwStyle,const CRect &_rect) {
		LoadPngFile(_pszBmFile,bitmap);
		SScrollBar::Create(&bitmap,_size,_dwStyle,_rect,_pParent,0);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
};


class MskMove : public MskBase, public SMoveRegion {
public:
	void Create(SElementRef *_pParent,CRect &rect) { 
		SMoveRegion::Create(rect,_pParent,0); 
		SetSnapToMode(Registry::IsSnapTo());
	}
	virtual void OnMouseMove(UINT _nFlags, CPoint _point) {
		CRect rect;

		SMoveRegion::OnMouseMove(_nFlags,_point);
		GetParentWnd()->GetWindowRect(&rect);
		Registry::SetWindowPos(rect.TopLeft());

		SElement::OnMouseMove(_nFlags,_point);
	}
};

class MskDecal : public MskBase, public SStatic {
public:
	void Create(SElementRef *_pParent,CPoint &_point,const char *_pszBmFile,UINT _nImages=1) {
		LoadPngFile(_pszBmFile,bitmap);
		SStatic::Create(&bitmap,_nImages,_point,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

	virtual void OnTheme(UINT _iTheme)	{ SStatic::SetItem(_iTheme); }

public:
	TBitmap bitmap;
};

class MskDecalPattern : public MskBase, public SStaticPattern {
public:
	void Create(SElementRef *_pParent,CRect &_rcFill,const char *_pszBmFile,UINT _nImages=1) {
		LoadPngFile(_pszBmFile,bitmap);
		SStaticPattern::Create(&bitmap,_rcFill,_nImages,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

	virtual void OnTheme(UINT _iTheme)	{ SStatic::SetItem(_iTheme); }

public:
	TBitmap bitmap;
};

class MskDecalFill : public MskBase, public SStaticFill {
public:
	void Create(SElementRef *_pParent,CRect &_rcFill,const TColor &_color,TAlpha _alpha) {
		SStaticFill::Create(_color,_alpha,_rcFill,_pParent);
	}
//	virtual void OnTheme(UINT _iTheme)	{ SStatic::SetItem(_iTheme); }
};

class MskDecalMasked : public MskBase, public SStaticMasked {
public:
	MskDecalMasked() : iTheme(0),iImage(0) {}

	void Create(SElementRef *_pParent,CPoint &_point,const char *_pszBmFile,TColor _rgb,UINT _nImages=1) {
		LoadPngFile(_pszBmFile,bitmap);
		SStaticMasked::Create(&bitmap,_rgb,_nImages,_point,_pParent);
	}
	void Create(SElementRef *_pParent,CPoint &_point,const char *_pszBmFile,TColor _rgb,CSize &_dims) {
		LoadPngFile(_pszBmFile,bitmap);
		SStaticMasked::Create(&bitmap,_rgb,_dims,_point,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
	UINT iTheme;
	UINT iImage;
};


#endif	// __AppSkin_h

