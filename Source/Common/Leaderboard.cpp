// Leaderboard.cpp - Steam Leaderboard
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Softare Design
 *
 * Revision History:
 *     3-10-23  JAltmaier  Created
 *
**/

#include "Leaderboard.h"
#include "Resource1.h"


Leaderboard::Leaderboard(char** _leaderboards, LeaderboardCallback* _cbObject, Callback _callback)
	: leaderboards(_leaderboards), cbObject(_cbObject), callback(_callback), m_nLeaderboardEntries(0), m_eLeaderboardData(k_ELeaderboardDataRequestGlobal), m_hSteamLeaderboard(NULL)
{
}


Leaderboard::~Leaderboard() {
}


void Leaderboard::Rotate()
{
	switch (m_eLeaderboardData)
	{
	case k_ELeaderboardDataRequestGlobal:
		m_eLeaderboardData = k_ELeaderboardDataRequestGlobalAroundUser;
		break;
	case k_ELeaderboardDataRequestGlobalAroundUser:
		m_eLeaderboardData = k_ELeaderboardDataRequestFriends;
		break;
	case k_ELeaderboardDataRequestFriends:
		m_eLeaderboardData = k_ELeaderboardDataRequestGlobal;
		break;
	}
	if (m_hSteamLeaderboard) {
		// Retrigger fetch of stats
		SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_hSteamLeaderboard, m_eLeaderboardData,
			-k_nMaxLeaderboardEntries / 2, k_nMaxLeaderboardEntries / 2);

		if (hSteamAPICall) {
			// Register for the async callback
			m_SteamCallResultDownloadEntries.Set(hSteamAPICall, this, &Leaderboard::OnLeaderboardDownloadEntries);

			m_nLeaderboardEntries = 0;
//			Rebuild();

			return;
		}
		else {
			Report("No stats");
			return;
		}
	}

	Report("No Download API");
}

void Leaderboard::Start()
{
	Report("Finding board...");
	if (m_hSteamLeaderboard)
		return;

	// Prompt library to download leaderboard stats for all configured boards
	for (int iBoard = 0; leaderboards[iBoard]; iBoard++)
	{
		Report(leaderboards[iBoard]);
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindOrCreateLeaderboard(leaderboards[iBoard], k_ELeaderboardSortMethodDescending, k_ELeaderboardDisplayTypeNumeric);

		if (hSteamAPICall)
			m_SteamCallResultCreateLeaderboard.Set(hSteamAPICall, this, &Leaderboard::OnFindLeaderboard);
		else
			Report("No hSteamAPICall");
	}
}

void Leaderboard::OnFindLeaderboard(LeaderboardFindResult_t* pFindLeaderboardResult, bool bIOFailure)
{
	// see if we encountered an error during the call
	if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure)
		return;

	m_hSteamLeaderboard = pFindLeaderboardResult->m_hSteamLeaderboard;

	// load the specified leaderboard data. We only display k_nMaxLeaderboardEntries entries at a time
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_hSteamLeaderboard, m_eLeaderboardData,
		-k_nMaxLeaderboardEntries / 2, k_nMaxLeaderboardEntries / 2);

	// Register for the async callback
	m_SteamCallResultDownloadEntries.Set(hSteamAPICall, this, &Leaderboard::OnLeaderboardDownloadEntries);
}


//-----------------------------------------------------------------------------
// Purpose: Called when SteamUserStats()->DownloadLeaderboardEntries() returns asynchronously
//-----------------------------------------------------------------------------
void Leaderboard::OnLeaderboardDownloadEntries(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure)
{
	// leaderboard entries handle will be invalid once we return from this function. Copy all data now.
	m_nLeaderboardEntries = pLeaderboardScoresDownloaded->m_cEntryCount;
	if (m_nLeaderboardEntries > k_nMaxLeaderboardEntries)
		m_nLeaderboardEntries = k_nMaxLeaderboardEntries;

	for (int index = 0; index < m_nLeaderboardEntries; index++)
	{
		SteamUserStats()->GetDownloadedLeaderboardEntry(pLeaderboardScoresDownloaded->m_hSteamLeaderboardEntries,
			index, &m_leaderboardEntries[index], NULL, 0);
	}

	// show our new data
	Rebuild();
}


//-----------------------------------------------------------------------------
// Purpose: Updates leaderboards with stats from our just finished game
//-----------------------------------------------------------------------------
void Leaderboard::UpdateLeaderboards(int score)
{
	if (m_hSteamLeaderboard)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(m_hSteamLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0);
		if (hSteamAPICall)
			m_SteamCallResultUploadScore.Set(hSteamAPICall, this, &Leaderboard::OnUploadScore);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Called when SteamUserStats()->UploadLeaderboardScore() returns asynchronously
//-----------------------------------------------------------------------------
void Leaderboard::OnUploadScore(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure)
{
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
}

//-----------------------------------------------------------------------------
// Purpose: Creates leaderboard menu
//-----------------------------------------------------------------------------
void Leaderboard::Rebuild()
{
	Report("Rebuild");

	if (m_hSteamLeaderboard)
	{
		// create a header for the leaderboard

		text.clear();

		text += SteamUserStats()->GetLeaderboardName(m_hSteamLeaderboard);

		if (m_eLeaderboardData == k_ELeaderboardDataRequestGlobal)
			text += ", Top";
		else if (m_eLeaderboardData == k_ELeaderboardDataRequestGlobalAroundUser)
			text += ", Around User";
		else if (m_eLeaderboardData == k_ELeaderboardDataRequestFriends)
			text += ", Friends of User";

		// create leaderboard
		{
			if (m_nLeaderboardEntries == 0)
			{
				// Requesting for global scores around the user will return successfully with 0 results if the
				// user does not have an entry on the leaderboard

				if (m_eLeaderboardData != k_ELeaderboardDataRequestGlobalAroundUser)
				{
					text += "\nNo scores for this leaderboard";
				}
				else
				{
					text += "\n";
					text += SteamFriends()->GetPersonaName();
					text += " does not have a score for this leaderboard";
				}
			}

			for (int index = 0; index < m_nLeaderboardEntries; index++)
			{
				char rgchMenuText[256];
				const char* pchName = SteamFriends()->GetFriendPersonaName(m_leaderboardEntries[index].m_steamIDUser);
				sprintf_s(rgchMenuText, "\n(%d) %s - %d", m_leaderboardEntries[index].m_nGlobalRank,
					pchName, m_leaderboardEntries[index].m_nScore);

				text += rgchMenuText;
			}
		}
		(cbObject->*callback)(text);
	}
}
