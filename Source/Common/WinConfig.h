// WinConfig.h - Two select boxes for skin, dict
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#ifndef _WinConfig_h
#define _WinConfig_h

#include "MainWnd.h"
#include "SListBox.h"

#define SELECTBACKGROUNDBUTTON      "IDB_SELECTBACKGROUND"
#define SELECTOKBUTTON              "IDB_SELECT_OK"
#define SELECTCANCELBUTTON          "IDB_SELECT_CANCEL"

class WinConfig: public SWnd {
public:
	WinConfig(MainWnd&);
	~WinConfig();

	int  OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void Done();
	void Process();

	virtual BOOL OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual BOOL OnElmtListBox(SListBox* _pElmt, int _nNotification) override;
	virtual VOID OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip) override;

private:
	MainWnd& winBoard;

	SListBox selectSkin;
	SListBox selectDict;

	CRect rectScores;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;

	MskButton elmtBtOk;
	MskButton elmtBtCancel;

protected:
	DECLARE_MESSAGE_MAP()
};

#endif

