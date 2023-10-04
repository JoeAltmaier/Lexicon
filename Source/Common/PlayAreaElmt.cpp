/* PlayAreaElmt.cpp -- Play area element
 *
 * Copyright (c) Tom Nelson, Iowa Software Design, 2020
 *
 * This material is a confidential trade secret and proprietary information of
 * Iowa Software Design, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of Iowa Software Design.
 * This material is also copyrighted as an unpublished work under sections 104 and
 * 408 of Title 17 of the United States Code. Law prohibits unauthorized use, 
 * copying or reproduction.
 *
 * Tom Nelson @ Iowa Software Design, PO Box 649, North Liberty, IA 52317, 319-626-3077
 *
 * Revision History:
 *      4-24-05  TRN  Created
 *      1-23-23  JCA  Added Mute, Pause
 *
**/

#include "PlayAreaElmt.h"
#include "TColorNames.h"
#include "SWnd.h"
#include "resource1.h"
#include "LexSkins.h"

#define MAXCOL		8
#define MAXROW		8

// .OnCreate
//
ERC PlayAreaElmt::OnCreate(SCreateStruct &_cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

    bmBackground.LoadDataImage((char*)svLex.svimgBackground.pBytes, svLex.svimgBackground.cb);
	if (!elmtBackground.Create(esVISIBLE, bmBackground,1,CPoint(0,0),this))
		return ERR;	//*FIX* These old Msk classes in AppSkin.h need a lot of work.

    // The game board

	if ((erc = elmtTileGrid.Create(esVISIBLE,CRect(CPoint(14,14), CSize(320,320)),this)) != OK)
		return erc;

    // Buttons

    if (!elmtBtQuit.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYQUITBUTTON, CRect(CPoint(14,424),CSize(58,31)),this, IDI_QUITBUTTON))
        return ERR;
    
    if (!elmtBtNew.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYNEWBUTTON, CRect(CPoint(68,424),CSize(58,31)),this, IDI_NEWBUTTON))
        return ERR;

    if (!elmtBtPause.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYPAUSEBUTTON, CRect(CPoint(126, 424), CSize(28, 30)), this, IDI_PAUSEBUTTON))
        return ERR;

    if (!elmtBtMute.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYMUTEBUTTON, CRect(CPoint(155, 424), CSize(31, 28)), this, IDI_MUTEBUTTON))
        return ERR;

    // not really buttons, things that display when triggered

    if (!elmtPauseLegend.Create(esVISIBLE, PLAYPAUSELEGENDBUTTON, 2, CPoint(135, 150), this))
        return ERR;
    elmtPauseLegend.SetItem(!elmtTileGrid.IsPauseGame());

    if (!elmtLevelBonus.Create(esVISIBLE, PLAYLEVELBONUSBUTTON, 2, CPoint(135, 150), this))
        return ERR;
    elmtLevelBonus.SetItem(1); // blank

    // text displays

    bmFontStrip.LoadPngResource(MAKEINTRESOURCE(IDB_BONUSFONT)); // Only way transparency works for now
    
    SFontBox::Context context(&bmFontStrip, svLex.svtextBonusScoreFont.pString, svLex.svrgbTransparent.color);

    // Error from OnCreate is returned from Create
    if (elmtBonusWord.Create(esVISIBLE, CRect(BONUSLEFT, BONUSTOP, BONUSRIGHT, BONUSBOTTOM), this, 0, &context))
        return ERR;

    if (elmtBestWord.Create(esVISIBLE, CRect(BESTLEFT, BESTTOP, BESTRIGHT, BESTBOTTOM), this, 0, &context))
        return ERR;

    if (elmtScore.Create(esVISIBLE, CRect(SCORELEFT, SCORETOP, SCORERIGHT, SCOREBOTTOM), this, 0, &context))
        return ERR;


    // clock display (bar graph of moves remaining)

    bmBarAnim.LoadPngResource(MAKEINTRESOURCE(IDB_CLOCK));

    {
        SAnimBox::Context context(&bmBarAnim, CLOCKSTEPS); // 20-frame vertical animation
        if (elmtClock.Create(esVISIBLE, CRect(CLOCKLEFT, CLOCKTOP, CLOCKRIGHT, CLOCKBOTTOM), this, 0, &context))
            return ERR;
        elmtClock.SetFrame(CLOCKSTEPS-1); // Full count of moves
    }

    return OK;
}

// .OnDestroy
//
void PlayAreaElmt::OnDestroy()
{
	BaseClass::OnDestroy();
}

// .OnButtonNotify
//
BOOL PlayAreaElmt::OnButtonNotify(SButtonControl *_pElmt,int _tEvent,CPoint _ptClick)
{
    if (_pElmt->GetId() == IDI_QUITBUTTON && _tEvent == SButtonControl::tbePRESS) {
        return NotifyButtonParent(_pElmt, _tEvent, _ptClick);
    }

    if (_pElmt->GetId() == IDI_NEWBUTTON && _tEvent == SButtonControl::tbePRESS) {
        // Bubble button press back to window
        return NotifyButtonParent(_pElmt, _tEvent, _ptClick);
    }

    if (_pElmt->GetId() == IDI_MUTEBUTTON && _tEvent == SButtonControl::tbePRESS) {
        elmtTileGrid.MuteGame();
        return true;
    }

    if (_pElmt->GetId() == IDI_PAUSEBUTTON && _tEvent == SButtonControl::tbePRESS) {
        elmtTileGrid.PauseGame();
        elmtPauseLegend.SetItem(!elmtTileGrid.IsPauseGame());
        return true;
    }

    return false;   // Event not processed
}

void PlayAreaElmt::SetScore(U32 score)
{
    char aScore[32];
    sprintf_s(aScore, "%d", score);
    elmtScore.SetText(aScore);
}
