// Achievement.h - Steam messages and states for managing Achievement data
/*
 * Copyright (c) 2023 by
 *      Joe Altmaier  Iowa Software Design
 *
 * Revision History:
 *     9-15-23  JAltmaier  Created
 *
**/

#ifndef _Achievement_h
#define _Achievement_h

#include "steam/steam_api.h"
#include <xstring>

struct Achieved_t
{
	const char* name;
	bool bAchieved;
};

class Achievement {
public:
	Achievement();
	~Achievement();

	bool Start();

	bool SetAchievement(const char *);
	bool IncStat(const char*);
	void Commit();

private:
};

#endif // _Achievement_h
