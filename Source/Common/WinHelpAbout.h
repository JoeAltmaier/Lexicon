// WinHelpAbout.h - Static text display
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     2-27-23  JAltmaier  Created
 *
**/

#ifndef _WinHelpAbout_h
#define _WinHelpAbout_h

#include "MainWnd.h"
#include "STextBox.h"

#define HELPBACKGROUNDBUTTON      "IDB_HELP"
#define HELPOKBUTTON              "IDB_HELP_OK"
#define HELPABOUTBUTTON           "IDB_HELP_ABOUT"

class WinHelpAbout : public SWnd {
public:
	WinHelpAbout(MainWnd&, const char *);
	~WinHelpAbout();

	int  OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	void Done();
	void Process();

	virtual BOOL OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual VOID OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip) override;

private:
	MainWnd& winBoard;

	const char* text;
	STextbox helpText;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;

	MskButton elmtBtOk;
	MskButton elmtBtAbout;

protected:
	DECLARE_MESSAGE_MAP()
};

#endif


