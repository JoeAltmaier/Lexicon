// Achivement.cpp - Steam Achivement
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Softare Design
 *
 * Revision History:
 *     9-15-23  JAltmaier  Created
 *
**/

#include "Achievement.h"

Achievement::Achievement(): bStarted(false)
{
}

Achievement::~Achievement() {
}

bool Achievement::Start() 
{
	int32 stat; // for probing init complete

	if (!bStarted)
		bStarted = SteamUserStats()->RequestCurrentStats();

	if (bStarted)
		return SteamUserStats()->GetStat("LEXICON_TESTINIT", &stat); // stats received, fully initialized
	else
		return false;
}

bool Achievement::SetAchievement(const char* pChName) {
	return SteamUserStats()->SetAchievement(pChName);
}

bool Achievement::GetStat(const char* pChName, int32 *stat) {
	return SteamUserStats()->GetStat(pChName, stat);
}

bool Achievement::SetStat(const char* pChName, int32 stat) {
	return SteamUserStats()->SetStat(pChName, stat);
}

bool Achievement::IncStat(const char* pChName) {
	int32 stat;
	SteamUserStats()->GetStat(pChName, &stat);
	stat++;
	return SteamUserStats()->SetStat(pChName, stat);
}

void Achievement::Commit() {
	SteamUserStats()->StoreStats();
}

bool Achievement::Reset(const char* pChName) {
	return SteamUserStats()->ClearAchievement(pChName);
}

