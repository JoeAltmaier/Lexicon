// Configuration.h - persistent settings
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-12-02  JAltmaier  Created
 *
**/

#ifndef __Configuration
#define __Configuration

#include "LexTypes.h"
//#include "DDefs.h"
#include "RegKey.h"
#include <String.h>
#include "Appafx.h"

class Configuration: public RegKey {
public:
	typedef enum  {GAME_SHUFFLE=1, GAME_CRASHANDBURN=2} TypeGame;

private:
	// 12 fields in Values
	enum {NCONFIG=14};
	typedef struct {
		TypeGame game;
		BOOL bClocked;
		TCHAR szSkin[MAXSIZE];
		TCHAR szDictionary[MAXSIZE];
		U32 difficulty;
		U32 difMax;
		U32 mapcode;
		TCHAR szTopScores[MAXSIZE];
		U32 nTimesPlayed;
		TCHAR szInitials[MAXSIZE];
		TCHAR szOwner[MAXSIZE];
		S32 loc;
		TCHAR szInstallDir[MAXSIZE];
	} Values;
	
	Values def;
	Values values;

	enum { NHIGHSCORES=6, SNAMEMAX=20 };
	TCHAR szHighShuffle[MAXSIZE];
	TCHAR szHighClocked[MAXSIZE];
	TCHAR szHighCrash[MAXSIZE];

	TCHAR szOwner[MAXSIZE];
	TCHAR szPathSkin[MAXSIZE];
	TCHAR szPathDict[MAXSIZE];
	TCHAR szId[11];
	U32 cbId;

	static
	TCHAR aLocalPath[];
	static
	TCHAR *aName[NCONFIG+1];  // field names + NULL
	static
	U32 aType[NCONFIG];

public:
	Configuration(HKEY hKey, TCHAR *pPathValues);
	~Configuration();

	U32 Game() const { return values.game; }
	BOOL IsShuffle() const { return (values.game == GAME_SHUFFLE); }
	BOOL IsCrash() const { return (values.game == GAME_CRASHANDBURN); }
	BOOL IsClocked() const { return values.bClocked; }
	void SetClocked(BOOL _bClocked) { values.bClocked=_bClocked; Commit(); }
	BOOL TrialOver(U32 nInc=0);
	U32 TrialDelay();
	BOOL IsHighScore(U32 score);

	void SetSkin(const TCHAR *pNameSkin) { lstrcpy(values.szSkin, pNameSkin); Commit(); }
	void SetDictionary(const TCHAR *pNameDictionary) { lstrcpy(values.szDictionary, pNameDictionary); Commit(); }
	void SetGame(TypeGame _game) { values.game=_game; Commit(); }
	void SetHighScore(const TCHAR *pInitials, S32 score);
	void SetInitials(const TCHAR *pInitials) { lstrcpy(values.szInitials, pInitials); Commit(); }
#ifndef UNDER_CE
	void SetOwner(const TCHAR *pOwner){ lstrcpy(values.szOwner, pOwner); lstrcpy(szOwner, pOwner); Commit(); }
#endif
	void SetLoc(const CPoint &pt) { values.loc=(pt.x << 16) | (U16)pt.y; Commit(); }

	const TCHAR *GetPathSkin() const { return szPathSkin; }
	const TCHAR *GetPathDict() const { return szPathDict; }
	const TCHAR *GetScores() const ;
	const TCHAR *GetSkin() const { return values.szSkin; }
	const TCHAR *GetDictionary() const { return values.szDictionary; }
	const TCHAR *GetInitials() const { return values.szInitials; }
	const TCHAR *GetOwner() const { return szOwner; }
	CPoint GetLoc() const { return CPoint(values.loc >> 16, (S16)(values.loc & 0xFFFF)); }
	const TCHAR *GetInstallDir() const { return values.szInstallDir; }

private:
	void Commit();

	void SetHighScores(S32 *aHighScores, TCHAR aHighNames[NHIGHSCORES][SNAMEMAX]);
	void GetHighScores(S32 *aHighScores, TCHAR aHighNames[NHIGHSCORES][SNAMEMAX]);
	void SetScores(TCHAR *pHs);

	void Hash(TCHAR *szId, U32 cb, TCHAR *szKey, S32 mod, S32 offset);
	void BuildId();
};

#endif
