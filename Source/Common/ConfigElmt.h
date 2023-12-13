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
#include "BonusList.h"

#define SELECTBACKGROUNDBUTTON      "IDB_SELECTBACKGROUND"
#define SELECTOKBUTTON              "IDB_SELECT_OK"
#define SELECTCANCELBUTTON          "IDB_SELECT_CANCEL"

class CLASS_ABASE2(ConfigElmt, public, SElement, public, BonusListCallback)
public:
	ConfigElmt():bonusList(this) {}
	~ConfigElmt() { }

	const char* GetBonusListData();

protected://SElement overrides
	virtual ERC  OnCreate(SCreateStruct& _cs) override;
	virtual void OnDestroy() override { }

	virtual BOOL OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual ERC OnNotify(SElement* _pChild, WPARAM _wParam, LPARAM _lParam) override { return NotifyParent(_wParam, _lParam); }

private:
	void Done();
	void Process();

private:
	virtual void OnBonusListCallback(BonusListCallback::Item *, int nItem) override;
	virtual void OnDownloadCallback(uint64_t, char*) override;

	SListBox selectSkin;
	SListBox selectDict;

	CRect rectScores;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;
	MskSkinDecal elmtBackground;

	MskButton elmtBtOk;
	MskButton elmtBtCancel;
	BonusList bonusList;
};

#endif // _ConfigElmt_h

