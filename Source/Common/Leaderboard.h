// Leaderboard.h - Steam messages and states for managing leaderboard data
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     3-09-23  JAltmaier  Created
 *
**/

#ifndef _Leaderboard_h
#define _Leaderboard_h

#include "steam/steam_api.h"
#include <xstring>

class LeaderboardCallback
{
public:
	virtual void OnLeaderboardCallback(std::string&) = 0;
};

typedef void (LeaderboardCallback::*Callback)(std::string&);

class Leaderboard {
public:
	Leaderboard(char** _leaderboards, LeaderboardCallback* _cbObject, Callback _callback);
	~Leaderboard();

	void Start();
	void UpdateLeaderboards(int score);
	void Rotate();

private:
	void Rebuild();

private:
	char** leaderboards;
	SteamLeaderboard_t m_hSteamLeaderboard;
	static const int k_nMaxLeaderboardEntries = 10;						// maximum number of leaderboard entries we can display
	LeaderboardEntry_t m_leaderboardEntries[k_nMaxLeaderboardEntries];	// leaderboard entries we received from DownloadLeaderboardEntries
	int m_nLeaderboardEntries;											// number of leaderboard entries we received
	ELeaderboardDataRequest m_eLeaderboardData;
	std::string text;
	LeaderboardCallback *cbObject;
	void (LeaderboardCallback::* callback)(std::string&) ;

	// Called when SteamUserStats()->FindOrCreateLeaderboard() returns asynchronously
	void OnFindLeaderboard(LeaderboardFindResult_t* pFindLearderboardResult, bool bIOFailure);
	CCallResult<Leaderboard, LeaderboardFindResult_t> m_SteamCallResultCreateLeaderboard;

	void OnUploadScore(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure);
	CCallResult<Leaderboard, LeaderboardScoreUploaded_t> m_SteamCallResultUploadScore;

	void OnLeaderboardDownloadEntries(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure);
	CCallResult<Leaderboard, LeaderboardScoresDownloaded_t> m_SteamCallResultDownloadEntries;

	void Report(const char *msg) {
		text.clear();
		text = msg;
		(cbObject->*callback)(text);
	}
};

#endif
