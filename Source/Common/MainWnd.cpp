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

#include "MainWnd.h"
#include "Registry.h"
#include "AboutDlg.h"
#include "LexSkins.h"
#include "Random.h"
#include "Resource1.h"
#include "VersionString.h"

BEGIN_MESSAGE_MAP(MainWnd, SWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

SkinValuesLex svLex;

extern void Decompress(U8*, U32, U8**, U32*);

// .OnCreate
//
int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ERC erc;

	if ((erc = SWnd::OnCreate(lpCreateStruct)) == -1)
		return erc;

	SetBackgroundColor(TColor(0x80,0x80,0x80));

	// Set the icon for this Application.  
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetModeSnapTo(Registry::IsSnapTo());

	MskBase::SetPath("skin\\");


	memset(&svLex, 0, sizeof(svLex));

	Random::Initialize();

	svdict.pBytes = NULL;
	svdictLoad.pBytes = NULL;

	InitializeDictionary();

	// Skins?
	LoadSkin();

	pBoard = new Board(config, *this);

	CRect rcClient;

	GetClientRect(&rcClient);
	sysMain.Create(SElement::esVISIBLE, rcClient, this);
	///sysMain.menuSys.AppendToMenu(GetSystemMenu(FALSE));

	GetClientRect(&rcClient);
	rcClient.left += 10;
	rcClient.right -= 10;
	rcClient.top += 20;
	rcClient.bottom -= 10;
	
	/*HACK*/
	rcClient.right = rcClient.left + 348;
	rcClient.bottom = rcClient.top + 464;

	if ((erc = elmtPlay.Create(SElement::esVISIBLE, rcClient,this)) != OK)
	 	return erc;
	
	elmtPlay.SetFocus();

	pBoard->Event(ENew, NULL); // New game

	return OK;
}

void MainWnd::OnSize(UINT _nType,int _cx,int _cy)
{
	//elmtPlay.ResizeElement(_cx,_cy);

	SWnd::OnSize(_nType,_cx,_cy);
}

BOOL MainWnd::OnElmtButtonNotify(SButtonControl *_pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDC_SYSMINIMIZE:
		CloseWindow();		// TWnd::CloseWindow()
		break;
	case IDC_SYSMAXIMIZE:
		break;

	case IDC_SYSCLOSE:
		PostMessage(WM_CLOSE);
		return true;

	case IDI_NEWBUTTON:
		pBoard->Event(ENew, NULL); // New game
		return true;

	case IDI_QUITBUTTON:
		pBoard->Event(EQuit, NULL); // Quit game
		elmtPlay.Quit();
		return true;
	}
	
	return false;	// Not processed
}


ERC MainWnd::OnElmtNotify(SElement* _pElmt, UINT _nCode, WPARAM _wParam, LPARAM _lParam) 
{
	switch (_wParam)
	{
	case notifyRELEASE:
		pBoard->Event(ESwap, (SwapTiles*)_lParam);
		pBoard->Event(ETileDropped, NULL);
		break;

	case notifyIDLE:
		elmtPlay.AnimationIdle();
		break;

	case notifySTILL:
		pBoard->Event(EStill, NULL);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// .About
//
void MainWnd::About()
{
	AboutDlg dlgAbout;
	dlgAbout.DoModal(); 
}

void MainWnd::PopulateTiles(U8 *letters)
{
	// Last callback during new game.
	// 
	// Create burnable slidable tiles from board
	elmtPlay.PopulateTiles(letters);
}

void MainWnd::ScoreAnimation(const Coord& cd, const U8* pWord, S32 score, BOOL bHoriz)
{
	// translate cd to pixel coordinates. bHoriz if word is horizontal else vertical
	// cd is tile coordinate on play grid
	int cCh = strlen((char*)pWord);

	// float score from center of word
	Coord center = cd.Center(bHoriz ? cCh : 0, !bHoriz ? cCh : 0);

	elmtPlay.CreateFloater(center, score);
}

void MainWnd::SetBonusWord(const U8* _pBonusWord) 
{
	// the word is from a wordlist. They are separated with \r
	char aWord[16];
	int cCh = strchr((char*)_pBonusWord, '\r') - (char*)_pBonusWord;
	if (cCh > 0 && cCh < 15)
	{
		memcpy(aWord, _pBonusWord, cCh);
		aWord[cCh] = '\0';
		elmtPlay.SetBonusWord(aWord);
	}
}

void MainWnd::SetBestWord(const U8* _pBestWord)
{
	// the word is from a wordlist. They are separated with \r
	char aWord[16];
	int cCh = strlen((char*)_pBestWord);
	if (cCh > 0 && cCh < 15)
	{
		memcpy(aWord, _pBestWord, cCh);
		aWord[cCh] = '\0';
		elmtPlay.SetBestWord(aWord);
	}
}

void MainWnd::StartAnimation(const Coord& cd)
{
	// A letter is burned. 
	elmtPlay.StartAnimation(cd);
}

void MainWnd::InitializeDictionary() {
	HRSRC hrwl = FindResource(NULL, MAKEINTRESOURCE(IDD_WORDLIST), TEXT("DICT"));
	void* pData = LoadResource(NULL, hrwl);
	Decompress((U8*)pData, SizeofResource(NULL, hrwl), (U8**)&svdict.pBytes, (U32*)&svdict.cb);

	// Find bonus word list
	HRSRC hrbl = FindResource(NULL, MAKEINTRESOURCE(IDD_BONUSLIST), TEXT("DICT"));
	U8* pText = (U8*)LoadResource(NULL, hrbl);
	U32 cbText = SizeofResource(NULL, hrbl);

	// Squish out \n's and blank lines (double-\r's)
	svtext.pString = new char[cbText];
	svtext.cb = 0;
	for (U32 iCh = 0; iCh < cbText; iCh++)
		switch (pText[iCh]) {
		case 0: return;
		case '\n':
			break;
		default:
			if (pText[iCh] != '\r' || svtext.pString[svtext.cb - 1] != '\r')
				svtext.pString[svtext.cb++] = tolower(pText[iCh]);
			break;
		}
}

TCHAR* MainWnd::LoadSkin() {
	TCHAR* pRet = NULL;

	// Load any configured skin

	// First, sync sound so we don't delete some sound buffer while its playing
	while (!PlaySound(MAKEINTRESOURCE(IDW_BLANK), NULL, SND_RESOURCE | SND_SYNC | SND_NOSTOP));

	// Delete any previous skin
	if (pSkinLoader)
		delete pSkinLoader;

	pSkinLoader = NULL;

	// Loaded skins may make use of some defaults, so always load them first
	LoadSkinDefaults();

	// Find the skin file
	TCHAR aName[256];
	lstrcpy(aName, config.GetPathSkin());
	lstrcat(aName, TEXT("\\"));
	lstrcat(aName, config.GetSkin());
	lstrcat(aName, TEXT(".skn"));

	// If "default" don't load 
	if (lstrcmp(config.GetSkin(), TEXT("default")) != 0) {

		pSkinLoader = new SkinLoader();

		// Open file
		HANDLE hSkin = CreateFile(
			aName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_SEQUENTIAL_SCAN, 
			NULL
		);

		if (hSkin == (HANDLE)-1)
			pRet = TEXT("Skin file not found");

		else {
			if (!pSkinLoader->LoadResourcesFromSkinfile(hSkin))
				pRet = TEXT("Not a Lexicon skin file");

			else {

				// Test version number
				// Version text resource must exist, must start with major version number and version must match
				const Resource& resource = pSkinLoader->Find((U8*)"SKINLexicon", SKIN_TEXT);
				char aCh[20] = "0";
				if (&resource) {
					U32 cb;
					for (cb = 0; cb < resource.srd.cb; cb++)
						if ((aCh[cb] = resource.pData[cb]) == '.')
							break;
					aCh[cb] = 0;
				}

				U32 version;
				if (sscanf(aCh, "%d", &version) == EOF
					|| (version != SKIN_MAJORVERSION))
					pRet = TEXT("Skin has incompatible version");
				else
					pSkinLoader->BuildResourceStructure(aSr, &svLex);

				CloseHandle(hSkin);
			}
		}
	}


	// Load any configured dictionary

	lstrcpy(aName, config.GetPathDict());
	lstrcat(aName, TEXT("\\"));
	lstrcat(aName, config.GetDictionary());
	lstrcat(aName, TEXT(".dict"));

	// If "default" don't load 
	if (lstrcmp(config.GetDictionary(), TEXT("default")) != 0)
		LoadDictionary(aName);

	return pRet;
}

void MainWnd::LoadSkinDefaults() {
	memset(&svLex, 0, sizeof(svLex));

	svLex.svrgbTransparent.color = PALETTERGB(0xFF, 0, 0xFF);

	// Main setup screens
	
	// Help screen
	svLex.svimgHelp.pBytes = LocateResource(IDI_HELP, TEXT("IMAGE"), &svLex.svimgHelp.cb);
	svLex.svimgHelp.rect = CRect(0, 0, 240, 320);

	svLex.svrectHelpBox.rect = CRect(14, 79, 226, 279);

	svLex.svimgHelpOk.pBytes = LocateResource(IDI_HELP_OK, TEXT("IMAGE"), &svLex.svimgHelpOk.cb);
	svLex.svimgHelpOk.rect = CRect(0, 0, 84, 28);
	svLex.svrectHelpOk.rect = CRect(8, 288, 92, 316);
	svLex.svrectHelpOkTouch.rect = CRect(10, 290, 90, 310);

	svLex.svimgHelpAbout.pBytes = LocateResource(IDI_HELP_ABOUT, TEXT("IMAGE"), &svLex.svimgHelpAbout.cb);
	svLex.svimgHelpAbout.rect = CRect(0, 0, 84, 28);
	svLex.svrectHelpAbout.rect = CRect(148, 288, 232, 316);
	svLex.svrectHelpAboutTouch.rect = CRect(150, 290, 230, 310);

	svLex.svtextHelp.pString = (char*)LocateResource(IDT_HELP, TEXT("TEXT"), &svLex.svtextHelp.cb);

	// Config (Select) screen
	svLex.svimgSelect.pBytes = LocateResource(IDI_SELECT, TEXT("IMAGE"), &svLex.svimgSelect.cb);
	svLex.svimgSelect.rect = CRect(0, 0, 240, 320);

	svLex.svrectSkinsBox.rect = CRect(14, 80, 226, 162);
	svLex.svrectDictsBox.rect = CRect(14, 197, 226, 279);

	svLex.svimgSelectOk.pBytes = LocateResource(IDI_SELECT_OK, TEXT("IMAGE"), &svLex.svimgSelectOk.cb);
	svLex.svimgSelectOk.rect = CRect(0, 0, 84, 28);
	svLex.svrectSelectOk.rect = CRect(8, 288, 92, 316);
	svLex.svrectSelectOkTouch.rect = CRect(10, 290, 90, 310);

	svLex.svimgSelectCancel.pBytes = LocateResource(IDI_SELECT_CANCEL, TEXT("IMAGE"), &svLex.svimgSelectCancel.cb);
	svLex.svimgSelectCancel.rect = CRect(0, 0, 84, 28);
	svLex.svrectSelectCancel.rect = CRect(148, 288, 232, 316);
	svLex.svrectSelectCancelTouch.rect = CRect(150, 290, 230, 310);

	// Main screen
	svLex.svimgMain.pBytes = LocateResource(IDI_MAIN, TEXT("IMAGE"), &svLex.svimgMain.cb);
	svLex.svimgMain.rect = CRect(0, 0, 240, 320);

	svLex.svrectHighScoresBox.rect = CRect(14, 79, 226, 161);
	svLex.svrectGameModeBox.rect = CRect(14, 197, 226, 251);

	svLex.svimgMainPlay.pBytes = LocateResource(IDI_MAIN_PLAY, TEXT("IMAGE"), &svLex.svimgMainPlay.cb);
	svLex.svimgMainPlay.rect = CRect(0, 0, 84, 28);
	svLex.svrectMainPlay.rect = CRect(28, 260, 112, 288);
	svLex.svrectMainPlayTouch.rect = CRect(30, 262, 110, 282);

	svLex.svimgMainExit.pBytes = LocateResource(IDI_MAIN_EXIT, TEXT("IMAGE"), &svLex.svimgMainExit.cb);
	svLex.svimgMainExit.rect = CRect(0, 0, 84, 28);
	svLex.svrectMainExit.rect = CRect(8, 288, 92, 316);
	svLex.svrectMainExitTouch.rect = CRect(10, 290, 90, 310);

	svLex.svimgMainConfig.pBytes = LocateResource(IDI_MAIN_CONFIG, TEXT("IMAGE"), &svLex.svimgMainConfig.cb);
	svLex.svimgMainConfig.rect = CRect(0, 0, 84, 28);
	svLex.svrectMainConfig.rect = CRect(128, 260, 212, 288);
	svLex.svrectMainConfigTouch.rect = CRect(130, 262, 210, 282);

	svLex.svimgMainHelp.pBytes = LocateResource(IDI_MAIN_HELP, TEXT("IMAGE"), &svLex.svimgMainHelp.cb);
	svLex.svimgMainHelp.rect = CRect(0, 0, 84, 28);
	svLex.svrectMainHelp.rect = CRect(148, 288, 232, 316);
	svLex.svrectMainHelpTouch.rect = CRect(150, 290, 230, 310);

	// Game board
	svLex.svimgBackground.pBytes = LocateResource(IDI_BACKGROUND, TEXT("IMAGE"), &svLex.svimgBackground.cb);
	svLex.svimgBackground.rect = CRect(0, 0, 348, 464);

	svLex.svrectBoard.rect = CRect(BORDERWIDTH, BORDERHEIGHT, 232, 232);

	svLex.svimgTiles.pBytes = LocateResource(IDI_TILE, TEXT("IMAGE"), &svLex.svimgTiles.cb);
	svLex.svimgTiles.rect = CRect(0, 0, 728, 308);

	svLex.svrectTilesInBoard.rect = CRect(0, 0, WIDTHINTILES, HEIGHTINTILES);

	// Buttons and displays
	svLex.svrectScore.rect = CRect(SCORELEFT, SCORETOP, SCORERIGHT, SCOREBOTTOM);
	svLex.svrectBonus.rect = CRect(BONUSLEFT, BONUSTOP, BONUSRIGHT, BONUSBOTTOM);
	svLex.svrectBest.rect = CRect(BESTLEFT, BESTTOP, BESTRIGHT, BESTBOTTOM);
	svLex.svrectQuit.rect = CRect(EXITLEFT, EXITTOP, EXITRIGHT, EXITBOTTOM);
	svLex.svrectNew.rect = CRect(NEWLEFT, NEWTOP, NEWRIGHT, NEWBOTTOM);
	svLex.svrectPause.rect = CRect(PAUSELEFT, PAUSETOP, PAUSERIGHT, PAUSEBOTTOM);
	svLex.svrectMute.rect = CRect(MUTELEFT, MUTETOP, MUTERIGHT, MUTEBOTTOM);

	svLex.svimgQuit.pBytes = LocateResource(IDI_QUITBUTTON, TEXT("IMAGE"), &svLex.svimgQuit.cb);
	svLex.svimgQuit.rect = CRect(0, 0, EXITRIGHT - EXITLEFT, EXITBOTTOM - EXITTOP);
	//	svLex.svimgQuitTouch.rect		=CRect(0,0, 0,0);

	svLex.svimgNew.pBytes = LocateResource(IDI_NEWBUTTON, TEXT("IMAGE"), &svLex.svimgNew.cb);
	svLex.svimgNew.rect = CRect(0, 0, NEWRIGHT - NEWLEFT, NEWBOTTOM - NEWTOP);
	//	svLex.svimgNewTouch.rect		=CRect(0,0, 0,0);

	svLex.svimgPause.pBytes = LocateResource(IDI_PAUSEBUTTON, TEXT("IMAGE"), &svLex.svimgPause.cb);
	svLex.svimgPause.rect = CRect(0, 0, PAUSERIGHT - PAUSELEFT, PAUSEBOTTOM - PAUSETOP);
	//	svLex.svimgPauseTouch.rect		=CRect(0,0, 0,0);

	svLex.svimgMute.pBytes = LocateResource(IDI_MUTEBUTTON, TEXT("IMAGE"), &svLex.svimgMute.cb);
	svLex.svimgMute.rect = CRect(0, 0, MUTERIGHT - MUTELEFT, MUTEBOTTOM - MUTETOP);
	//	svLex.svimgMuteTouch.rect		=CRect(0,0, 0,0);

	svLex.svimgPauseLegend.pBytes = LocateResource(IDI_PAUSELEGEND, TEXT("IMAGE"), &svLex.svimgPauseLegend.cb);
	svLex.svimgPauseLegend.rect = CRect(0, 0, 82, 22);
	svLex.svrectPauseLegend.rect = CRect(LEGENDLEFT, LEGENDTOP, LEGENDRIGHT, LEGENDBOTTOM);
	svLex.svrectPauseLegendTouch.rect = CRect(0, 0, 0, 0);

	svLex.svrectClock.rect = CRect(CLOCKLEFT, CLOCKTOP, CLOCKRIGHT, CLOCKBOTTOM);
	svLex.svimgClock.pBytes = LocateResource(IDI_CLOCK, TEXT("IMAGE"), &svLex.svimgClock.cb);
	svLex.svimgClock.rect = CRect(0, 0, 222, 168);

	svLex.svrectBonusLevel.rect = CRect(LEVELLEFT, LEVELTOP, LEVELRIGHT, LEVELBOTTOM);
	svLex.svimgBonusLevel.pBytes = LocateResource(IDI_BONUSLEVEL, TEXT("IMAGE"), &svLex.svimgBonusLevel.cb);
	svLex.svimgBonusLevel.rect = CRect(0, 0, 100, 20);

	svLex.svrectDiscard.rect = CRect(DISCARDLEFT, DISCARDTOP, DISCARDRIGHT, DISCARDBOTTOM);

	svLex.svimgScoreDigits.pBytes = LocateResource(IDI_DIGITS, TEXT("IMAGE"), &svLex.svimgScoreDigits.cb);
	svLex.svimgScoreDigits.rect = CRect(0, 0, WIDTHDIGIT * 11, HEIGHTDIGIT);
	svLex.svtextScoreDigits.pString = "0123456789-";

	svLex.svimgBonusScoreFont.pBytes = LocateResource(IDI_BONUSFONT, TEXT("IMAGE"), &svLex.svimgBonusScoreFont.cb);
	svLex.svimgBonusScoreFont.rect = CRect(0, 0, 407, 14);
	svLex.svtextBonusScoreFont.pString = "abcdefghijklmnopqrstuvwxyz0123456789";

	svLex.svwavePick.pBytes = LocateResource(IDW_CLICK, TEXT("WAVE"), &svLex.svwavePick.cb);
	svLex.svwaveDrop.pBytes = LocateResource(IDW_CLACK, TEXT("WAVE"), &svLex.svwaveDrop.cb);
	svLex.svwaveBurn.pBytes = LocateResource(IDW_BURN, TEXT("WAVE"), &svLex.svwaveBurn.cb);
	svLex.svwaveScore.pBytes = LocateResource(IDW_CHIME, TEXT("WAVE"), &svLex.svwaveScore.cb);
	svLex.svwaveBonus.pBytes = LocateResource(IDW_DRUMROLL, TEXT("WAVE"), &svLex.svwaveBonus.cb);
	svLex.svwaveLevel.pBytes = LocateResource(IDW_SPIRAL, TEXT("WAVE"), &svLex.svwaveLevel.cb);
	svLex.svwaveDebit.pBytes = LocateResource(IDW_DISAPPOINTMENT, TEXT("WAVE"), &svLex.svwaveDebit.cb);
	svLex.svwaveButton.pBytes = LocateResource(IDW_CLICK, TEXT("WAVE"), &svLex.svwaveButton.cb);
	svLex.svwaveScroll.pBytes = LocateResource(IDW_CLICK, TEXT("WAVE"), &svLex.svwaveScroll.cb);
	svLex.svwaveSelect.pBytes = LocateResource(IDW_CLICK, TEXT("WAVE"), &svLex.svwaveSelect.cb);

	svLex.svdictWordList = svdict;

	svLex.svtextBonusList = svtext;

	svLex.svboolBonusRandom.b = true;

	svLex.svscalarWordLengthMin.u32 = SMALLESTWORD;
	svLex.svscalarClockPerSwap.u32 = CLOCKPERSWAP;
	svLex.svscalarClockPerLetter.u32 = CLOCKPERLETTER;
	svLex.svscalarPictureDelay.u32 = 100;

	svLex.svboolTest.b = false;

	// Pictures are an array
	for (U32 iPicture = 0; iPicture < NPICTURE; iPicture++) {
		svLex.aSvimgPicture[iPicture].pBytes = LocateResource(IDI_PHOTO0 + iPicture, TEXT("IMAGE"), &svLex.aSvimgPicture[iPicture].cb);
		svLex.aSvimgPicture[iPicture].rect = CRect(0, 0, svLex.svrectBoard.rect.Width(), svLex.svrectBoard.rect.Height());
	}
}

void MainWnd::LoadDictionary(TCHAR* pFileName) {
	HANDLE hDict = CreateFile(
		pFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hDict == (void*)-1)
		return;

	int size;
	size = GetFileSize(hDict, NULL);
	char* buf = new char[(int)size];

	unsigned long cb;
	ReadFile(hDict, buf, size, &cb, NULL);
	CloseHandle(hDict);

	// Free previous dictionary
	if (svdictLoad.pBytes)
		delete svdictLoad.pBytes;

	svdictLoad.pBytes = NULL;

	// Load this dictionary as plain text
	Decompress((U8*)buf, cb, (U8**)&svdictLoad.pBytes, (U32*)&svdictLoad.cb);

	svLex.svdictWordList = svdictLoad;

	// Free compression buffer
	delete buf;
}

void* MainWnd::LocateResource(_In_     int id, _In_     LPCSTR lpType, _Out_ UINT32* size)
{
	HRSRC hr = FindResource(NULL, MAKEINTRESOURCE(id), lpType);
	*size = SizeofResource(NULL, hr);
	return LoadResource(NULL, hr);
}

