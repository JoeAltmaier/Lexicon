/* MainWnd.cpp -- SFC Demo Main Window
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
 *      1-07-05  TRN  Created
 *
**/


#pragma once

#ifndef __MainWnd_h
#define __MainWnd_h

#include "AppDefs.h"
#include "PlayAreaElmt.h"
#include "SysMainElmt.h"
#include "Board.h"
#include "Configuration.h"
#include "SWnd.h"
#include "MainApp.h"
#include "Skins.h"

class WinStart;
class WinConfig;
class WinHelpAbout;

class MainWnd : public SWnd {
public:
	MainWnd(): config(HKEY_CURRENT_USER, TEXT("Software\\Iowa Software Design\\Lexicon")), pSkinLoader(NULL) { }// : hCurSave(NULL) { } 

	virtual ~MainWnd() { } //DestroyWindow(); }

public:
	void About();
	void PopulateTiles(U8 *); // in playelmt, Board, from New etc
	void Time(S32 cTick) {} // from board, to clock element
	void ScoreAnimation(const Coord& cd, const U8* pWord, S32 score, BOOL bHoriz);
	void SetScore(U32 score) { elmtPlay.SetScore(score); }
	void SetBonusWord(const U8* _pBonusWord);
	void SetBestWord(const U8* _pBestWord);
	void AddAnimation(const Coord& cd) {}
	void StartAnimation(const Coord& cd);

	void StartGame();
	void ReturnToMainScreen();
	void Exit() { exit(0); }
	void StartSelect();
	void StartHelp();

// These aren't referenced yet!
//	void StopAnimation(Animation&);
	/*
	void LoseAnimation();
	void WinAnimation();
	void GameOver();
	void SetTime(U32 _clock);
	*/

protected://CWnd
	int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT _nType,int _cx,int _cy);

protected://SWnd
    virtual BOOL OnElmtButtonNotify(SButtonControl *_pElmt, int _tEvent, CPoint _ptClick) override;
	virtual ERC OnElmtNotify(SElement* _pElmt, UINT _nCode, WPARAM _wParam, LPARAM _lParam) override;

private:
	void InitializeDictionary();
	TCHAR* LoadSkin();
	void LoadSkinDefaults();
	void LoadDictionary(TCHAR* pFileName);
	void* LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32 *size);

public:
    PlayAreaElmt elmtPlay;
    SysMainElmt sysMain;
	Board *pBoard;
	Configuration config;
	SkinLoader* pSkinLoader;

	SkinValueDict svdict;
	SkinValueText svtext;

	SkinValueDict svdictLoad;

	WinStart* pWinStart;
	WinConfig* pWinConfig;
	WinHelpAbout* pWinHelp;

protected:
	DECLARE_MESSAGE_MAP()
};


#endif	// __MainWnd_h
