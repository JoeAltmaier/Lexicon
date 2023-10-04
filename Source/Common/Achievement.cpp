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

Achievement::Achievement()
{
}

Achievement::~Achievement() {
}

bool Achievement::Start() 
{
	return SteamUserStats()->RequestCurrentStats();
}

bool Achievement::SetAchievement(const char* pChName) {
	return SteamUserStats()->SetAchievement(pChName);
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

