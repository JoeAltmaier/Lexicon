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

	if ((erc = elmtTileGrid.Create(esVISIBLE,CRect(CPoint(14,14), CSize(320,320)),this)) != OK)
		return erc;

    if (!elmtBtQuit.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYQUITBUTTON, CRect(CPoint(14,424),CSize(58,31)),this, IDI_QUITBUTTON))
        return ERR;
    
    if (!elmtBtNew.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYNEWBUTTON, CRect(CPoint(68,424),CSize(58,31)),this, IDI_NEWBUTTON))
        return ERR;

    if (!elmtBtPause.Create(esVISIBLE | SButton::bsPUSHSIMPLE, PLAYPAUSEBUTTON, CRect(CPoint(126, 424), CSize(28, 30)), this))
        return ERR;

    if (!elmtBtMute.Create(esHIDDEN | SButton::bsPUSHSIMPLE, PLAYMUTEBUTTON, CRect(CPoint(155, 424), CSize(31, 28)), this))
        return ERR;

    if (!elmtPauseLegend.Create(esVISIBLE, PLAYPAUSELEGENDBUTTON, 2, CPoint(135, 150), this))
        return ERR;
    elmtPauseLegend.SetItem(!elmtTileGrid.IsPauseGame());

    TColor transColor(255, 255, 255);
    font.LoadPngFile("DigitsFontStrip.png", "E:\\IowaSoftwareDesign\\Lexicon\\Source\\Common\\Resource\\", 1, transColor);
    TColor textColor(255, 0, 0);
    font.LoadTrueType(20, "Courier", textColor);
    if (!elmtBonusWord.Create(esVISIBLE, &font, textColor, CRect(BONUSLEFT, BONUSTOP, BONUSRIGHT, BONUSBOTTOM), this))
        return ERR;

    if (!elmtBestWord.Create(esVISIBLE, &font, textColor, CRect(BESTLEFT, BESTTOP, BESTRIGHT, BESTBOTTOM), this))
        return ERR;

    if (!elmtScore.Create(esVISIBLE, &font, textColor, CRect(SCORELEFT, SCORETOP, SCORERIGHT, SCOREBOTTOM), this))
        return ERR;

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
    if (_pElmt == &elmtBtQuit && _tEvent == SButtonControl::tbePRESS) {
        return NotifyButtonParent(_pElmt, _tEvent, _ptClick);
    }

    if (_pElmt == &elmtBtNew && _tEvent == SButtonControl::tbePRESS) {
        // Bubble button press back to window
        return NotifyButtonParent(_pElmt, _tEvent, _ptClick);
    }

    if (_pElmt == &elmtBtMute && _tEvent == SButtonControl::tbePRESS) {
        elmtTileGrid.MuteGame();
        return true;
    }

    if (_pElmt == &elmtBtPause && _tEvent == SButtonControl::tbePRESS) {
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
