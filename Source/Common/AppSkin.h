/* AppSkin.cpp - Derived Skin Classes
 *
 * Copyright (c) 2005 by
 *      Tom Nelson D.B.A. OpenVista Software
 *
 * This material is a confidential trade secret and proprietary information of
 * OpenVista Software, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of OpenVista Software.
 * This material is also copyrighted as an unpublished work under sections 104
 * and 408 of Title 17 of the United States Code. Law prohibits unauthorized
 * use, copying or reproduction.
 *
 * Revision History:
 *      4-21-05  TRN  Created
 *
**/


#pragma once

#ifndef __AppSkin_h
#define __AppSkin_h

#include "Skin.h"
#include "Registry.h"
#include "SMenuButton.h"

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
	void LoadDataImage(const char* _pDataBytes, int _cbDataBytes, TBitmap& _bm) {
		ERC erc;
		if ((erc = _bm.LoadDataImage(_pDataBytes, _cbDataBytes)) != OK) {
			char szTmp[128];
				sprintf(szTmp, "Unable to load PNG bitmap resource.");
				//throw MeteorExceptionSkinFailedToLoadBitmap(szTmp);
		}
	}

public:
	static void SetPath(const char *_pszPath) { if (_pszPath) strcpy(szPath,_pszPath); else strcpy(szPath,""); }
	static const char *GetPath()	{ return szPath; }

public:
	static char szPath[sPATH+1];
};

class MskResBase {
public:
	void LoadPngResource(int _idRes, TBitmap &_bm) {
		ERC erc;
		if ((erc = _bm.LoadPngResource(MAKEINTRESOURCE(_idRes))) != OK) {
			char szTmp[128];
			sprintf(szTmp, "Unable to load PNG bitmap resource.");
			//throw MeteorExceptionSkinFailedToLoadBitmap(szTmp);
		}
	}
	void LoadPngResource(const char *_pszResource, TBitmap &_bm) {
		ERC erc;
		if ((erc = _bm.LoadPngResource(_pszResource)) != OK) {
			char szTmp[128];
			sprintf(szTmp, "Unable to load PNG bitmap resource.");
			//throw MeteorExceptionSkinFailedToLoadBitmap(szTmp);
		}
	}
#if 0
	void LoadPngRes(const char *_pszBmFile, TBitmap &_bm, const CRect &_rcSegment) {
		ERC erc;
		if ((erc = _bm.LoadPngFile(_rcSegment, _pszBmFile, GetPath())) != OK) {
			char szTmp[128];
			sprintf(szTmp, "Unable to load cropped bitmap resource.");
			//throw MeteorExceptionSkinFailedToLoadBitmap(szTmp);
		}
	}
#endif
};

class MskButton : public MskBase, public SButton {
public:
	BOOL Create(DWORD _nStyle,const char *_pszBmFile,CRect _rcButton,SElementRef *_pParent,UINT _id=0) {
		LoadPngFile(_pszBmFile,bitmap);
		return SButton::Create(_nStyle,&bitmap,bitmap.GetRect(),_rcButton.Size(),_rcButton,_pParent,_id);
	}
#if 0
	BOOL Create(DWORD _nStyle,const char *_pszBmFile,CSize &_sizeButton,CPoint &_point,UINT _flags,UINT _nThemes,SElementRef *_pParent,UINT _id=0) {
		LoadPngFile(_pszBmFile,bitmap);
		return SButton::Create(_nStyle,&bitmap,0,_flags,_nThemes,_point,_pParent,_id);
	}
#endif
	BOOL Create(DWORD _nStyle, const char* _pDataBytes, int _cbDataBytes, CRect _rcButton, SElementRef* _pParent, UINT _id = 0) {
		LoadDataImage(_pDataBytes, _cbDataBytes, bitmap);
		return SButton::Create(_nStyle, &bitmap, bitmap.GetRect(), _rcButton.Size(), _rcButton, _pParent, _id);
	}

	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
};

class MskResButton : public MskResBase, public SButton {
public:
	BOOL Create(DWORD _nStyle, int _idRes, CRect _rcButton, SElementRef *_pParent, UINT _id = 0) {
		LoadPngResource(_idRes, bitmap);
		return SButton::Create(_nStyle, &bitmap, bitmap.GetRect(), _rcButton.Size(), _rcButton, _pParent, _id);
	}
	BOOL Create(DWORD _nStyle, const char *_pszResource, CRect _rcButton, SElementRef *_pParent, UINT _id = 0) {
		LoadPngResource(_pszResource, bitmap);
		return SButton::Create(_nStyle, &bitmap, bitmap.GetRect(), _rcButton.Size(), _rcButton, _pParent, _id);
	}
	UINT Width() { return bitmap.Width(); }
	UINT Height() { return bitmap.Height(); }

public:
	TBitmap bitmap;
};

class MskMenuButton : public MskBase, public SMenuButton {
public:
	BOOL Create(DWORD _nStyle,const char *_pszBmFile,CRect &_rcButton,SElementRef *_pParent,UINT _id=0) {
		LoadPngFile(_pszBmFile,bitmap);
		return SMenuButton::Create(_nStyle,&bitmap,bitmap.GetRect(),_rcButton.Size(),_rcButton,_pParent,_id);
	}
#if 0
//	void Create(SWnd *_pParent,const char *_pszBmFile,CPoint &_point,UINT _id) {
//		LoadPngFile(_pszBmFile,bitmap);
//		SMenuButton::Create(&bitmap,_point,_pParent,_id);
//	}
	BOOL Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _flags,UINT _nThemes,UINT _id) {
		ERC erc; 
		if ((erc = bitmap.LoadPngFile(_pszBmFile,GetPath())) != OK) 
			return FALSE; //throw MeteorExceptionSkinFailedToLoadBitmap("Unable to open Button bitmap file.");

		return SMenuButton::Create(0,&bitmap,_flags,_nThemes,_point,_pParent,_id);
	}
#endif
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
	BOOL Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,UINT _nImages) {
		LoadPngFile(_pszBmFile,bitmap);
		return TitleBar::Create(0,&bitmap,_nImages,_point,_pParent);
	}
	BOOL Create(SElementRef *_pParent,const char *_pszBmFile,CPoint &_point,CSize &_dims) {
		LoadPngFile(_pszBmFile,bitmap);
		return TitleBar::Create(0,&bitmap,_dims,_point,_pParent);
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
	BOOL Create(DWORD _nStyle,const char *_pszBmFile,const CSize & _size,int _dwStyle,const CRect &_rect,SElementRef *_pParent,UINT _id=0) {
		LoadPngFile(_pszBmFile,bitmap);
		return SScrollBar::Create(_nStyle,&bitmap,_size,_dwStyle,_rect,_pParent,_id);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
};


class MskMove : public MskBase, public SMoveRegion {
public:
	BOOL Create(DWORD _nStyle,CRect &rect,SElementRef *_pParent) { 
		BOOL bRc = SMoveRegion::Create(_nStyle,rect,_pParent); 
		if (bRc) SetSnapToMode(Registry::IsSnapTo());
		return bRc;
	}
	virtual VOID OnMouseMove(UINT _nFlags, CPoint _point) {
		CRect rect;

		SMoveRegion::OnMouseMove(_nFlags,_point);
		GetParentWnd()->GetWindowRect(&rect);
		Registry::SetWindowPos(rect.TopLeft());
	}
};

class MskDecal : public MskBase, public SStatic {
public:
	BOOL Create(DWORD _nStyle,const char *_pszBmFile,UINT _nImages,CPoint &_point,SElementRef *_pParent) {
		LoadPngFile(_pszBmFile,bitmap);
		return SStatic::Create(_nStyle,&bitmap,_nImages,_point,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}
	
	virtual BOOL OnMouseOver(CPoint _pt) override { return false; }
	virtual void OnTheme(UINT _iTheme)	{ SStatic::SetItem(_iTheme); }

public:
	TBitmap bitmap;
};

class MskSkinDecal : public MskBase, public SStatic {
public:
	BOOL Create(DWORD _nStyle, TBitmap &_bitmap, UINT _nImages, CPoint& _point, SElementRef* _pParent) {
		pBitmap = &_bitmap;
		return SStatic::Create(_nStyle,pBitmap, _nImages, _point, _pParent);
	}
	UINT Width() { return pBitmap->Width(); }
	UINT Height() { return pBitmap->Height(); }

	virtual BOOL OnMouseOver(CPoint _pt) override { return false; }
	virtual void OnTheme(UINT _iTheme) { SStatic::SetItem(_iTheme); }

public:
	TBitmap *pBitmap;
};

class MskResDecal : public MskResBase, public SStatic {
public:
	BOOL Create(DWORD _nStyle, int idRes, UINT _nImages, CPoint &_point, SElementRef *_pParent) {
		LoadPngResource(idRes, bitmap);
		return SStatic::Create(_nStyle, &bitmap, _nImages, _point, _pParent);
	}
	BOOL Create(DWORD _nStyle, const char *_pszResource, UINT _nImages, CPoint &_point, SElementRef *_pParent) {
		LoadPngResource(_pszResource, bitmap);
		return SStatic::Create(_nStyle, &bitmap, _nImages, _point, _pParent);
	}
	UINT Width() { return bitmap.Width(); }
	UINT Height() { return bitmap.Height(); }

	virtual BOOL OnMouseOver(CPoint _pt) override { return false; }
	virtual void OnTheme(UINT _iTheme) { SStatic::SetItem(_iTheme); }

public:
	TBitmap bitmap;
};

class MskDecalPattern : public MskBase, public SStaticPattern {
public:
	BOOL Create(SElementRef *_pParent,CRect &_rcFill,const char *_pszBmFile,UINT _nImages=1) {
		LoadPngFile(_pszBmFile,bitmap);
		return SStaticPattern::Create(0,&bitmap,_rcFill,_nImages,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

	virtual void OnTheme(UINT _iTheme)	{ SStatic::SetItem(_iTheme); }

public:
	TBitmap bitmap;
};

class MskDecalFill : public MskBase, public SStaticFill {
public:
	BOOL Create(SElementRef *_pParent,CRect &_rcFill,const TColor &_color,TAlpha _alpha) {
		return SStaticFill::Create(0,_color,_alpha,_rcFill,_pParent);
	}
//	virtual void OnTheme(UINT _iTheme)	{ SStatic::SetItem(_iTheme); }
};

class MskDecalMasked : public MskBase, public SStaticMasked {
public:
	MskDecalMasked() : iTheme(0),iImage(0) {}

	BOOL Create(SElementRef *_pParent,CPoint &_point,const char *_pszBmFile,TColor _rgb,UINT _nImages=1) {
		LoadPngFile(_pszBmFile,bitmap);
		return SStaticMasked::Create(0,&bitmap,_rgb,_nImages,_point,_pParent);
	}
	BOOL Create(SElementRef *_pParent,CPoint &_point,const char *_pszBmFile,TColor _rgb,CSize &_dims) {
		LoadPngFile(_pszBmFile,bitmap);
		return SStaticMasked::Create(0,&bitmap,_rgb,_dims,_point,_pParent);
	}
	UINT Width()	{ return bitmap.Width(); }
	UINT Height()	{ return bitmap.Height();}

public:
	TBitmap bitmap;
	UINT iTheme;
	UINT iImage;
};


#endif	// __AppSkin_h

