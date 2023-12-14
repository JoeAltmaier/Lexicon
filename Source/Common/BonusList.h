// BonusList.h - Steam messages and states for managing BonusList data
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     4-12-23  JAltmaier  Created
 *
**/

#ifndef _BonusList_h
#define _BonusList_h

#include "steam/steam_api.h"
#include <xstring>

class BonusListCallback
{
public:
	typedef struct Item { uint64_t id; char* desc; char* name; } Item;

	virtual void OnBonusListCallback(BonusListCallback::Item*, int nItem) = 0;
	virtual void OnDownloadCallback(uint64_t, char*) = 0;
};

class BonusList {
public:
	BonusList( BonusListCallback* _cbObject);
	~BonusList();

	void Start();

	void DownloadItem(uint64_t id);
	const char* GetBonusListData(int id);

private:
	void Rebuild();

private:
	BonusListCallback* cbObject;
	UGCQueryHandle_t m_hSteamAPIUGCQuery;
	CCallResult<BonusList, SteamUGCQueryCompleted_t> m_SteamCallResultUGCQuery;
	void OnUGCQuery(SteamUGCQueryCompleted_t* pQueryUGCResult, bool);
	static const int N_DETAIL_MAX = 128;
	SteamUGCDetails_t details[N_DETAIL_MAX];
	int nDetail;
	static const int MAX_PATH_NAME = 256;
	char aFolder[MAX_PATH_NAME];
	PublishedFileId_t idDownloaded;
	STEAM_CALLBACK(BonusList, OnDownloadItemResult, DownloadItemResult_t);

	// Render details into callback format
	void Report() {
		BonusListCallback::Item* aItem = new BonusListCallback::Item[nDetail];
		
		for (int i = 0; i < nDetail; i++) {
			aItem[i].id = details[i].m_nPublishedFileId;
			aItem[i].desc = details[i].m_rgchDescription;
			aItem[i].name = details[i].m_rgchTitle;
		}
		cbObject->OnBonusListCallback(aItem, nDetail);

		delete []aItem;
	}
};

#endif
