// BonusList.cpp - Steam BonusList
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Softare Design
 *
 * Revision History:
 *     4-12-23  JAltmaier  Created
 *
**/

#include "BonusList.h"
#include "Resource1.h"


BonusList::BonusList(BonusListCallback* _cbObject)
	: cbObject(_cbObject), m_hSteamAPIUGCQuery(NULL), nDetail(0)
{
}


BonusList::~BonusList() {
}

void BonusList::Start()
{
	//CreateItem, AddAppDependency, SetItemTitle, SetItemDescription, SetItemContent(file), SetItemVisibility(k_ERemoteStoragePublishedFileVisibilityFriendsOnly), SubmitItemUpdate,

	// Close any redundant query
	if (m_hSteamAPIUGCQuery)
		SteamUGC()->ReleaseQueryUGCRequest(m_hSteamAPIUGCQuery);

	m_hSteamAPIUGCQuery = SteamUGC()->CreateQueryUserUGCRequest(SteamUser()->GetSteamID().GetAccountID(), k_EUserUGCList_Published, k_EUGCMatchingUGCType_Items, k_EUserUGCListSortOrder_TitleAsc, SteamUtils()->GetAppID(), SteamUtils()->GetAppID(), (uint32)1);

	if (m_hSteamAPIUGCQuery != k_UGCQueryHandleInvalid)
	{
		SteamAPICall_t hSteamAPICall = SteamUGC()->SendQueryUGCRequest(m_hSteamAPIUGCQuery);
		m_SteamCallResultUGCQuery.Set(hSteamAPICall, this, &BonusList::OnUGCQuery);
	}
}

void BonusList::OnUGCQuery(SteamUGCQueryCompleted_t* pUGCQuerymResult, bool bIOFailure)
{
	// Load details of each result
	for (nDetail = 0; nDetail < N_DETAIL_MAX && SteamUGC()->GetQueryUGCResult(m_hSteamAPIUGCQuery, nDetail, details + nDetail); nDetail++)
		;

	// Deliver
	Report();

	SteamUGC()->ReleaseQueryUGCRequest(m_hSteamAPIUGCQuery);
	m_hSteamAPIUGCQuery = NULL;
}

const char* BonusList::GetBonusListData(int id) {
	return "Somethin";
}

void BonusList::DownloadItem(uint64_t id) {
	SteamUGC()->DownloadItem(id, true);
}

void BonusList::OnDownloadItemResult(DownloadItemResult_t* pDownloadItemResult) {
	if (pDownloadItemResult->m_unAppID == SteamUtils()->GetAppID()) {
		uint64_t size;
		uint32_t timestamp;
		SteamUGC()->GetItemInstallInfo(pDownloadItemResult->m_nPublishedFileId, &size, aFolder, sizeof(aFolder), &timestamp);
		aFolder[MAX_PATH_NAME-1] = 0;

		idDownloaded = pDownloadItemResult->m_nPublishedFileId;
		
		// Deliver item path in callback
		cbObject->OnDownloadCallback(idDownloaded, aFolder);
	}
}

#if 0
//-----------------------------------------------------------------------------
// Purpose: Called when SteamUserStats()->DownloadBonusListEntries() returns asynchronously
//-----------------------------------------------------------------------------
void BonusList::OnBonusListDownloadEntries(BonusListScoresDownloaded_t* pBonusListScoresDownloaded, bool bIOFailure)
{
	// BonusList entries handle will be invalid once we return from this function. Copy all data now.
	m_nBonusListEntries = pBonusListScoresDownloaded->m_cEntryCount;
	if (m_nBonusListEntries > k_nMaxBonusListEntries)
		m_nBonusListEntries = k_nMaxBonusListEntries;

	for (int index = 0; index < m_nBonusListEntries; index++)
	{
		SteamUserStats()->GetDownloadedBonusListEntry(pBonusListScoresDownloaded->m_hSteamBonusListEntries,
			index, &m_BonusListEntries[index], NULL, 0);
	}

	// show our new data
	Rebuild();
}


//-----------------------------------------------------------------------------
// Purpose: Updates BonusLists with stats from our just finished game
//-----------------------------------------------------------------------------
void BonusList::UpdateBonusLists(int score)
{
#if 0
	if (m_hSteamBonusList)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadBonusListScore(m_hSteamBonusList, k_EBonusListUploadScoreMethodKeepBest, score, NULL, 0);
		if (hSteamAPICall)
			m_SteamCallResultUploadScore.Set(hSteamAPICall, this, &BonusList::OnUploadScore);
	}
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Called when SteamUserStats()->UploadBonusListScore() returns asynchronously
//-----------------------------------------------------------------------------
void BonusList::OnUploadScore(BonusListScoreUploaded_t* pScoreUploadedResult, bool bIOFailure)
{
#if 0
	if (!pScoreUploadedResult->m_bSuccess)
	{
		// error
		Report("Error in OnUploadScore");
	}

	if (pScoreUploadedResult->m_bScoreChanged)
	{
		Rebuild();
		// could display new rank
	}
#endif
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Creates BonusList menu
//-----------------------------------------------------------------------------
void BonusList::Rebuild()
{
#if 0
	Report("Rebuild");

	if (m_hSteamBonusList)
	{
		// create a header for the BonusList

		text.clear();

		text += SteamUserStats()->GetBonusListName(m_hSteamBonusList);

		if (m_eBonusListData == k_EBonusListDataRequestGlobal)
			text += ", Top";
		else if (m_eBonusListData == k_EBonusListDataRequestGlobalAroundUser)
			text += ", Around User";
		else if (m_eBonusListData == k_EBonusListDataRequestFriends)
			text += ", Friends of User";

		// create BonusList
		{
			if (m_nBonusListEntries == 0)
			{
				// Requesting for global scores around the user will return successfully with 0 results if the
				// user does not have an entry on the BonusList

				if (m_eBonusListData != k_EBonusListDataRequestGlobalAroundUser)
				{
					text += "\nNo scores for this BonusList";
				}
				else
				{
					text += "\n";
					text += SteamFriends()->GetPersonaName();
					text += " does not have a score for this BonusList";
				}
			}

			for (int index = 0; index < m_nBonusListEntries; index++)
			{
				char rgchMenuText[256];
				const char* pchName = SteamFriends()->GetFriendPersonaName(m_BonusListEntries[index].m_steamIDUser);
				sprintf_s(rgchMenuText, "\n(%d) %s - %d", m_BonusListEntries[index].m_nGlobalRank,
					pchName, m_BonusListEntries[index].m_nScore);

				text += rgchMenuText;
			}
		}
		(cbObject->*callback)(text);
	}
#endif
}
