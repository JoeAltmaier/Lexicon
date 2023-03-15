// WinLeader.cpp - Steam Leaderboard
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Iowa Softare Design
 *
 * Revision History:
 *     3-07-23  JAltmaier  Created
 *
**/

#include "WinLeader.h"
#include <mmsystem.h>
#include "Resource1.h"
#include "LexSkins.h"


BEGIN_MESSAGE_MAP(WinLeader, SWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



WinLeader::WinLeader(MainWnd& _winBoard, char** _leaderboards)
	: winBoard(_winBoard), leaderboards(_leaderboards), m_eLeaderboardData(k_ELeaderboardDataRequestGlobal)
{
}


WinLeader::~WinLeader() {
}

int  WinLeader::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ERC erc;

	if ((erc = SWnd::OnCreate(lpCreateStruct)) == -1)
		return erc;

	LOGFONT logfont;
	memset((char*)&logfont, 0, sizeof(logfont));
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN;
	logfont.lfHeight = 20;
	logfont.lfWeight = FW_BOLD;

	font.LoadLogFont(&logfont, svLex.svrgbTransparent.color);
	elmtSteam.Create(SElement::esVISIBLE, &font, TColor(0), svLex.svrectHelpBox.rect, this);
	
	bmBackground.Create(svLex.svimgBackground.rect.Size(), 0);
	bmBackground.LoadDataImage((const char*)svLex.svimgHelp.pBytes, svLex.svimgHelp.cb);

	if (!elmtBtOk.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgHelpOk.pBytes, (int)svLex.svimgHelpOk.cb, svLex.svrectHelpOk.rect, this, IDB_HELP_OK))
		return ERR;
	if (!elmtBtAbout.Create(SElement::esVISIBLE | SButton::bsPUSHSIMPLE, (const char*)svLex.svimgHelpAbout.pBytes, (int)svLex.svimgHelpAbout.cb, svLex.svrectHelpAbout.rect, this, IDB_HELP_ABOUT))
		return ERR;

	return 0;
}

BOOL WinLeader::OnElmtButtonNotify(SButtonControl* _pElmt, int _tEvent, CPoint _ptClick)
{
	if (_tEvent == 2)
		return false;

	switch (_pElmt->GetId()) {
	case IDB_HELP_OK:
		winBoard.ReturnToMainScreen();
		return true;

	case IDB_HELP_ABOUT:
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
		// Retrigger fetch of stats
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_hSteamLeaderboard, m_eLeaderboardData,
				-k_nMaxLeaderboardEntries / 2, k_nMaxLeaderboardEntries / 2);

			// Register for the async callback
			m_SteamCallResultDownloadEntries.Set(hSteamAPICall, this, &WinLeader::OnLeaderboardDownloadEntries);

			elmtSteam.SetText("Initializing...");
		}
		return true;
	}

	return false;
}

VOID WinLeader::OnShowWindow(BOOL _bShow, UINT _nStatus)
{
	if (_bShow)
	{
		// Prompt library to download leaderboard stats for all configured boards
		for (int iBoard = 0; leaderboards[iBoard]; iBoard++)
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->FindOrCreateLeaderboard(leaderboards[iBoard],
				k_ELeaderboardSortMethodDescending, k_ELeaderboardDisplayTypeNumeric);

			if (hSteamAPICall)
				m_SteamCallResultCreateLeaderboard.Set(hSteamAPICall, this, &WinLeader::OnFindLeaderboard);

			elmtSteam.SetText("Initializing...");
		}
	}

	SWnd::OnShowWindow(_bShow, _nStatus);
}

void WinLeader::OnFindLeaderboard(LeaderboardFindResult_t* pFindLeaderboardResult, bool bIOFailure)
{
	// see if we encountered an error during the call
	if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure)
		return;

	m_hSteamLeaderboard = pFindLeaderboardResult->m_hSteamLeaderboard;

	// load the specified leaderboard data. We only display k_nMaxLeaderboardEntries entries at a time
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_hSteamLeaderboard, m_eLeaderboardData,
		-k_nMaxLeaderboardEntries / 2, k_nMaxLeaderboardEntries / 2);

	// Register for the async callback
	m_SteamCallResultDownloadEntries.Set(hSteamAPICall, this, &WinLeader::OnLeaderboardDownloadEntries);
}


//-----------------------------------------------------------------------------
// Purpose: Called when SteamUserStats()->DownloadLeaderboardEntries() returns asynchronously
//-----------------------------------------------------------------------------
void WinLeader::OnLeaderboardDownloadEntries(LeaderboardScoresDownloaded_t* pLeaderboardScoresDownloaded, bool bIOFailure)
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
void WinLeader::UpdateLeaderboards(int score)
{
	if (m_hSteamLeaderboard)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(m_hSteamLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0);
		if (hSteamAPICall)
			m_SteamCallResultUploadScore.Set(hSteamAPICall, this, &WinLeader::OnUploadScore);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Called when SteamUserStats()->UploadLeaderboardScore() returns asynchronously
//-----------------------------------------------------------------------------
void WinLeader::OnUploadScore(LeaderboardScoreUploaded_t* pScoreUploadedResult, bool bIOFailure)
{
	if (!pScoreUploadedResult->m_bSuccess)
	{
		// error
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
void WinLeader::Rebuild()
{
	text = "";

	if (m_hSteamLeaderboard)
	{
		// create a header for the leaderboard

		text += SteamUserStats()->GetLeaderboardName(m_hSteamLeaderboard);

		if (m_eLeaderboardData == k_ELeaderboardDataRequestGlobal)
			text += ", Top " + m_nLeaderboardEntries;
		else if (m_eLeaderboardData == k_ELeaderboardDataRequestGlobalAroundUser)
			text += ", Around User";
		else if (m_eLeaderboardData == k_ELeaderboardDataRequestFriends)
			text += ", Friends of User";
	}

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
			sprintf(rgchMenuText, "\n(%d) %s - %d", m_leaderboardEntries[index].m_nGlobalRank,
				pchName, m_leaderboardEntries[index].m_nScore);

			text += rgchMenuText;
		}
	}

	elmtSteam.SetText(text.c_str());
}


VOID WinLeader::OnBlend(TBitmap& _bmCanvas, const CRect& _rcElmt, const CRect& _rcClip)
{
	_bmCanvas.CopyFrom(bmBackground, svLex.svimgBackground.rect, svLex.svimgBackground.rect, svLex.svimgBackground.rect, 0, 0);
}
