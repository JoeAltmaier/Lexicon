// Configuration.cpp - persistent settings
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-12-02  JAltmaier  Created
 *
**/

#include "Configuration.h"
///#include "Debug.h"
#include "Resource.h"

TCHAR Configuration::aLocalPath[]=TEXT(".");
TCHAR *Configuration::aName[]={TEXT("game"), TEXT("clocked"), TEXT("skin"), TEXT("dictionary"), TEXT("difficulty"), TEXT("max"), TEXT("mapcode"), TEXT("topscores"), TEXT("ntimesplayed"), TEXT("initials"), TEXT("owner"), TEXT("loc"), TEXT("InstallDir"), NULL};
U32 Configuration::aType[]={REG_DWORD, REG_DWORD, REG_SZ, REG_SZ, REG_DWORD, REG_DWORD, REG_DWORD, REG_SZ, REG_DWORD, REG_SZ, REG_SZ, REG_DWORD, REG_SZ};

Configuration::Configuration(HKEY hKey, TCHAR *pPath)
: RegKey(hKey, pPath, true)
{
	// Set up defaults in case this is the first time the program ran
	def.game=GAME_CRASHANDBURN;
	def.bClocked=false;
	lstrcpy(def.szSkin, TEXT("default"));
	lstrcpy(def.szDictionary, TEXT("default"));
	def.difficulty=0;
	def.difMax=0;
	def.mapcode=0;
	lstrcpy(def.szTopScores, TEXT("TRN,1000|JCA,1000|JDH,1000"));
	def.nTimesPlayed=0;
	lstrcpy(def.szInitials, TEXT("AAA"));
	lstrcpy(def.szOwner, TEXT("Owner name"));
	def.loc=0;

	GetModuleFileName(NULL, def.szInstallDir, MAXSIZE);
	TCHAR *pch=def.szInstallDir + sizeof(def.szInstallDir);
	while (*pch != '\\') pch--;
	*pch=0;

	// Read registry copy of values, applying defaults if 1st time
	Read(aName, aType, (U8*)&def, false, (U8*)&values);

	// Separate out three high-score strings

	TCHAR *pSrc=values.szTopScores;

	TCHAR *pDst=szHighShuffle;
	while (*pSrc && *pSrc != TCHAR('|')) *pDst++=*pSrc++;
	*pDst=0;
	pSrc++;

	pDst=szHighClocked;
	while (*pSrc && *pSrc != TCHAR('|')) *pDst++=*pSrc++;
	*pDst=0;
	pSrc++;
	
	pDst=szHighCrash;
	while (*pSrc) *pDst++=*pSrc++;
	*pDst=0;

#ifdef UNDER_CE
	// Get owner name from standard registry location
	RegKey regkey(hKey, TEXT("\\ControlPanel\\Owner"));
	regkey.Read(TEXT("Owner"), szOwner);

	// Build directory paths from path left by installer
	RegKey regApp(HKEY_LOCAL_MACHINE, TEXT("\\Software\\Apps\\Derelict Software Lexicon"));
	regApp.Read(TEXT("InstallDir"), szPathSkin);
#else
	// Get owner name from config
	lstrcpy(szOwner, values.szOwner);

	// Build directory paths from path left by our installer
	lstrcpy(szPathSkin, values.szInstallDir);
#endif

	if (!szPathSkin[0])
		lstrcpy(szPathSkin, aLocalPath);

	lstrcpy(szPathDict, szPathSkin);

	lstrcat(szPathSkin, TEXT("\\Skins"));

	lstrcat(szPathDict, TEXT("\\Dicts"));
}

Configuration::~Configuration() {
}

const TCHAR *Configuration::GetScores() const {
	// Pick three score strings apart, return one
	if (IsCrash())
		return szHighCrash;

	if (IsClocked())
		return szHighClocked;
	
	return szHighShuffle;
}

void Configuration::SetScores(TCHAR *pHs) {
	// Stick together with other two score strings
	if (IsCrash())
		lstrcpy(szHighCrash, pHs);

	else if (IsClocked())
		lstrcpy(szHighClocked, pHs);

	else lstrcpy(szHighShuffle, pHs);

	wsprintf(values.szTopScores, TEXT("%s|%s|%s"), szHighShuffle, szHighClocked, szHighCrash);

	Commit();
}

void Configuration::GetHighScores(S32 *aHighScores, TCHAR aHighNames[NHIGHSCORES][SNAMEMAX]) {

	U32 nHighScores=0;
	memset(aHighScores, 0, NHIGHSCORES * sizeof(U32));

	const TCHAR *pScores=GetScores();

	// Parse out high scores string
	while (*pScores && (isalnum(*pScores) || *pScores == TCHAR(' ')) && nHighScores < NHIGHSCORES) {
		U32 cb=0;
		while (pScores[cb] && pScores[cb] != TCHAR(','))
			cb++;

		memset(aHighNames[nHighScores], 0, SNAMEMAX * sizeof(TCHAR));
		memcpy(aHighNames[nHighScores], pScores, min(SNAMEMAX, cb) * sizeof(TCHAR));

		pScores += cb+1;
		cb=0;
		U32 score=0;
		while (pScores[cb] && isdigit(pScores[cb])) {
			score = score * 10 + (pScores[cb] - TCHAR('0'));
			cb++;
		}

		aHighScores[nHighScores]=score;

		nHighScores++;
		pScores += cb+1;
	}
}

void Configuration::SetHighScores(S32 *aHighScores, TCHAR aHighNames[NHIGHSCORES][SNAMEMAX]) {
	TCHAR aHs[MAXSIZE];
	TCHAR *pHs=aHs;
	for (U32 iHs=0; iHs < NHIGHSCORES && aHighScores[iHs]; iHs++) {
		wsprintf(pHs, TEXT("%s,%d,"), &aHighNames[iHs], aHighScores[iHs]);
		while (*pHs) pHs++;
	}

	SetScores(aHs);
}

void Configuration::SetHighScore(const TCHAR *pInitials, S32 score) {
	S32 aHighScores[NHIGHSCORES];
	TCHAR aHighNames[NHIGHSCORES][SNAMEMAX];

	GetHighScores(aHighScores, aHighNames);

	if (score > aHighScores[NHIGHSCORES-1]) {
		S32 iHs;
		for (iHs=NHIGHSCORES-2; iHs >= 0 && aHighScores[iHs] < score; iHs--) {
			aHighScores[iHs+1] = aHighScores[iHs];
			memcpy(aHighNames[iHs+1], aHighNames[iHs], sizeof(aHighNames[iHs]));
		}
		aHighScores[iHs+1] = score;
		memcpy(aHighNames[iHs+1], pInitials, sizeof(aHighNames[iHs+1]));
		aHighNames[iHs+1][SNAMEMAX-1]=NULL;

		SetHighScores(aHighScores, aHighNames);
	}
}

BOOL Configuration::IsHighScore(U32 score) {
	S32 aHighScores[NHIGHSCORES];
	TCHAR aHighNames[NHIGHSCORES][SNAMEMAX];

	GetHighScores(aHighScores, aHighNames);

	return (score > (U32)aHighScores[NHIGHSCORES-1]);
}

void Configuration::Commit() {
	// Write and read values
	Read(aName, aType, (U8*)&values, true, (U8*)&values);
}

