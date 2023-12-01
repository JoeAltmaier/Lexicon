// HelpElmt.h - Static text display
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     2-27-23  JAltmaier  Created
 *
**/

#ifndef _HelpElmt_h
#define _HelpElmt_h

#include "AppDefs.h"
#include "AppSkin.h"
#include "SElement.h"

#include "SEditBox.h"
#include "SControl.h"

#define HELPBACKGROUNDBUTTON      "IDB_HELP"
#define HELPOKBUTTON              "IDB_HELP_OK"
#define HELPABOUTBUTTON           "IDB_HELP_ABOUT"

class CLASS_ABASE(HelpElmt, public, SElement)
public:
	HelpElmt();
	~HelpElmt();

protected://SElement overrides
	virtual ERC  OnCreate(SCreateStruct& _cs) override;
	virtual void OnDestroy() override { }
	virtual BOOL OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual void OnElementPosChanged(SElementPos &_pos) override;

private:
	char* text;
	SEditBox helpText;

	char* about;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;
	MskSkinDecal elmtBackground;

	MskButton elmtBtOk;
	MskButton elmtBtAbout;
};

#endif


