// WinConfig.h - Two select boxes for skin, dict
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#ifndef _ConfigElmt_h
#define _ConfigElmt_h

#include "AppDefs.h"
#include "AppSkin.h"
#include "SElement.h"
#include "SFont.h"
#include "SListBox.h"

#define SELECTBACKGROUNDBUTTON      "IDB_SELECTBACKGROUND"
#define SELECTOKBUTTON              "IDB_SELECT_OK"
#define SELECTCANCELBUTTON          "IDB_SELECT_CANCEL"

class CLASS_ABASE(ConfigElmt, public, SElement)
public:
	ConfigElmt() { }
	~ConfigElmt() { }

protected://SElement overrides
	virtual ERC  OnCreate(SCreateStruct& _cs) override;
	virtual void OnDestroy() override { }

	virtual BOOL OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual ERC OnNotify(SElement* _pChild, WPARAM _wParam, LPARAM _lParam) override { return NotifyParent(_wParam, _lParam); }
	virtual BOOL OnListBox(SListBox* _pElmt, int _nNotification) override;

private:
	void Done();
	void Process();

private:
	SListBox selectSkin;
	SListBox selectDict;

	CRect rectScores;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;
	MskSkinDecal elmtBackground;

	MskButton elmtBtOk;
	MskButton elmtBtCancel;
};

#endif // _ConfigElmt_h

