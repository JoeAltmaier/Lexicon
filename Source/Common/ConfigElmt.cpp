// ConfigElmt.cpp - Paints a GifAnimator, manages buttons
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-13-02  JAltmaier  Created
 *
**/

#include "ConfigElmt.h"
#include "resource1.h"
#include "LexSkins.h"

int  ConfigElmt::OnCreate(SCreateStruct &_cs)
{
	ERC erc;

	if ((erc = BaseClass::OnCreate(_cs)) != OK)
		return erc;

	bmBackground.LoadDataImage((const char*)svLex.svimgSelect.pBytes, svLex.svimgSelect.cb);
	if (!elmtBackground.Create(esVISIBLE, bmBackground, 1, CPoint(0, 0), this))
		return ERR;	//*FIX* These old Msk classes in AppSkin.h need a lot of work.

	LOGFONT logfont;
	memset((char*)&logfont, 0, sizeof(logfont));
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
	logfont.lfHeight = 20;
	logfont.lfWeight = FW_BOLD;

	font.LoadLogFont(&logfont, 0);
	selectSkin.Create(esCAPTURECLICKS | MK_LBUTTON, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectSkinsBox.rect, this, 0);

	selectSkin.AddString(TEXT("none"));
	selectSkin.SetCurSel(0);
	selectSkin.SetFocus();

	selectDict.Create(esCAPTURECLICKS, &font, TColor(0), TColor(0xFF, 0, 0xFF), SElement::esVISIBLE, svLex.svrectDictsBox.rect, this, 0);
	selectDict.AddString(TEXT("English"));
	selectDict.SetCurSel(0);

	if (!elmtBtOk.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgSelectOk.pBytes, (int)svLex.svimgSelectOk.cb, svLex.svrectSelectOk.rect, this, IDB_SELECT_OK))
		return ERR;
	if (!elmtBtCancel.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgSelectCancel.pBytes, (int)svLex.svimgSelectCancel.cb, svLex.svrectSelectCancel.rect, this, IDB_SELECT_CANCEL))
		return ERR;

	// Display available bonus words lists from the cloud
	bonusList.Start();

	return 0;
}

BOOL ConfigElmt::OnButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	switch (_pElmt->GetId()) {
	case IDB_SELECT_OK:
		Process();

		// drop thru
	case IDB_SELECT_CANCEL:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		NotifyParent(IDB_SELECT_CANCEL, 0);
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		return true;
	}

	return false;
}

void ConfigElmt::Process() {
	int iItem = selectSkin.GetCurSel();
	uint64_t *pId = (uint64_t*)selectSkin.GetItemDataPtr(iItem);
	bonusList.DownloadItem(*pId);
}

void ConfigElmt::OnBonusListCallback(BonusListCallback::Item*itemList, int nItem)
{
	selectSkin.ResetContent();

	for (int iItem = 0; iItem < nItem; iItem++) {
		selectSkin.AddString(itemList[iItem].desc);
		BonusListCallback::Item* pItem = new BonusListCallback::Item(itemList[iItem]);
		selectSkin.SetItemDataPtr(iItem, pItem);
	}
}

// A Bonus Word List (.bwl) file is available
void ConfigElmt::OnDownloadCallback(uint64_t id, char *pFolder) {
	BonusListCallback::Item* pItem = (BonusListCallback::Item*)selectSkin.GetItemDataPtr(selectSkin.GetCurSel());
	if (id == pItem->id)
	{
		// Deliver notification of content of downloaded bonus word list

		char aPath[PATH_MAX];
		sprintf_s(aPath, "%s\\%s.bwl", pFolder, pItem->name);

		// Read bonus word list
		FILE* f; 
		f = fopen(aPath, "rb");
		if (f)
		{
			fseek(f, 0L, SEEK_END);
			long sz = ftell(f);
			rewind(f);
			char* pBWL = new char[sz+1];
			fread(pBWL, 1, sz, f);
			pBWL[sz] = 0;

			NotifyParent(notifyBONUSWORDLIST, (LPARAM)pBWL);

			//? Do this here, or only when a game is actually started?//
			// 
			// Determine if the word list has changed since our last game
			int32 bwlSize = SteamRemoteStorage()->GetFileSize("BonusWordList.txt");
			if (bwlSize > 0) {
				char bwlName[128];
				if (SteamRemoteStorage()->FileRead("BonusWordList.txt", bwlName, bwlSize) == bwlSize) {
					bwlName[bwlSize] = 0;
					if (strcmp(bwlName, pItem->name) != 0) {
						NotifyParent(notifyBONUSWORDLISTRESET, NULL);
					}
				}
			}

			SteamRemoteStorage()->FileWrite("BonusWordList.txt", pItem->name, strlen(pItem->name));
		}
	}
}