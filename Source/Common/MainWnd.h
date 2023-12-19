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
#include "Achievement.h"
#include "ConfigElmt.h"
#include "StartElmt.h"
#include "HelpElmt.h"
#include "LexSkins.h"

extern SkinValuesLex svLex;

class MainWnd : public SWnd {
public:
	MainWnd(): config(HKEY_CURRENT_USER, TEXT("Software\\Iowa Software Design\\Lexicon")), pBoard(NULL), pSkinLoader(NULL), bStarted(false), bNextBonusWord(false) { }

	virtual ~MainWnd() { }

public:
	void About();
	void PopulateTiles(U8 *); // in playelmt, Board, from New etc
	void Time(U32 cTick) { elmtPlay.SetClock(cTick); } // from board, to clock element
	void ScoreAnimation(const Coord& cd, const U8* pWord, S32 score, BOOL bHoriz);
	void SetScore(U32 score) { elmtPlay.SetScore(score); }
	void SetBonusWord(int _iBonus);
	void SetBestWord(const U8* _pBestWord);
	void AddAnimation(const Coord& cd) {}
	void StartAnimation(const Coord& cd);
	void StartAnimation(const Coord& cd, U8 glow);
	void StartAnimation(const Coord& ctAt, const Coord& ctTo);
	void LevelBonus(U32 bOn) { elmtPlay.LevelBonus(bOn); }
	void StartGame();
	void GameOver(U32 score);
	void ReturnToMainScreen();
	void Exit() { exit(0); }
	void StartConfig();
	void StartHelp();
	void Achieve(const char*);
	void Stat(const char*);
	void Timer();
	void NextBonusWord(bool _bNext = false);

// These aren't referenced yet!
//	void StopAnimation(Animation&);
//	void LoseAnimation();
//	void WinAnimation();

protected://CWnd
	int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT _nType,int _cx,int _cy);

	BOOL OnQueryEndSession();

	void OnEndSession(BOOL);

protected://SWnd
    virtual BOOL OnElmtButtonNotify(SButtonControl *_pElmt, int _tEvent, CPoint _ptClick) override;
	virtual ERC OnElmtNotify(SElement* _pElmt, UINT _nCode, WPARAM _wParam, LPARAM _lParam) override;

private:
	void InitializeDictionary();
	void SetBonusList(U8* pText, U32 cbText);
	bool IsBonusList();
	TCHAR* LoadSkin();
	void LoadSkinDefaults();
	void LoadDictionary(TCHAR* pFileName);
	void SelectBonusList();
	void* LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32 *size);
	void Hide(SElement& elmt) { elmt.MoveOrderBottom(); elmt.EnableElement(false); }
	void Show(SElement& elmt) { elmt.MoveOrderTop(); elmt.EnableElement(true); }

public:
    PlayAreaElmt elmtPlay;
    SysMainElmt sysMain;
	Board *pBoard;
	Configuration config;
	SkinLoader* pSkinLoader;

	SkinValueDict svdict;
	SkinValueText svtext;

	SkinValueDict svdictLoad;

	StartElmt elmtStart;
	ConfigElmt elmtConfig;
	HelpElmt elmtHelp;

	Achievement achievement;
	bool bStarted;
	bool bNextBonusWord;
	int nBonusWord;

protected:
	DECLARE_MESSAGE_MAP()
};


#endif	// __MainWnd_h
