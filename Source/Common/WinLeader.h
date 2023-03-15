// WinLeader.h - Main menu when starting game
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     2-23-23  JAltmaier  Created
 *
**/

#ifndef _WinLeader_h
#define _WinLeader_h

#include "MainWnd.h"
#include "SListBox.h"
#include "STextbox.h"
#include "steam/steam_api.h"
#include <xstring>

#define LEADERBKGD      "IDB_LEADERBACKGROUND"
#define LEADEREXITBUTTON      "IDB_HELP_OK"

class WinLeader : public SWnd {
public:
	WinLeader(MainWnd&, char** _leaderboards);
	~WinLeader();

	int  OnCreate(LPCREATESTRUCT lpCreateStruct);

	void UpdateLeaderboards(int score);

private:
	virtual BOOL OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick) override;
	virtual VOID OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip) override;
	afx_msg void OnShowWindow(BOOL _bShow, UINT _nStatus);
	void Rebuild();

private:
	MainWnd& winBoard;

	CRect rectSteam;

	SFont font;
	U32 fontHeight;
	TBitmap bmBackground;

	MskButton elmtBtOk;
	MskButton elmtBtAbout;
	STextbox elmtSteam;

	char** leaderboards;
	SteamLeaderboard_t m_hSteamLeaderboard;
	static const int k_nMaxLeaderboardEntries = 10;						// maximum number of leaderboard entries we can display
	LeaderboardEntry_t m_leaderboardEntries[k_nMaxLeaderboardEntries];	// leaderboard entries we received from DownloadLeaderboardEntries
	int m_nLeaderboardEntries;											// number of leaderboard entries we received
	ELeaderboardDataRequest m_eLeaderboardData;
	std::string text;

	// Called when SteamUserStats()->FindOrCreateLeaderboard() returns asynchronously
	void OnFindLeaderboard(LeaderboardFindResult_t* pFindLearderboardResult, bool bIOFailure);
	CCallResult<WinLeader, LeaderboardFindResult_t> m_SteamCallResultCreateLeaderboard;

	void OnUploadScore(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure);
	CCallResult<WinLeader, LeaderboardScoreUploaded_t> m_SteamCallResultUploadScore;

	void OnLeaderboardDownloadEntries(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure);
	CCallResult<WinLeader, LeaderboardScoresDownloaded_t> m_SteamCallResultDownloadEntries;
		
protected:
	DECLARE_MESSAGE_MAP()
};

#endif

