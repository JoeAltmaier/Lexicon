// LexSkins.h - Lexicon skin structure definitions
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-04-02  JCA  Created
 *
**/

#ifndef __LexSkins_h
#define __LexSkins_h

#include "LexTypes.h"
#include "Skins.h"

extern SkinResources aSr[];

enum {NPICTURE=25};

typedef struct {
	SkinValueRgb svrgbTransparent;

	// Help screen
	SkinValueImage svimgHelp;

	SkinValueRect svrectHelpBox;

	SkinValueImage svimgHelpOk;
	SkinValueRect svrectHelpOk;
	SkinValueRect svrectHelpOkTouch;

	SkinValueImage svimgHelpAbout;
	SkinValueRect svrectHelpAbout;
	SkinValueRect svrectHelpAboutTouch;

	SkinValueText svtextHelp;

	// Config screen
	SkinValueImage svimgSelect;
	SkinValueRect svrectSkinsBox;
	SkinValueRect svrectDictsBox;

	SkinValueImage svimgSelectOk;
	SkinValueRect svrectSelectOk;
	SkinValueRect svrectSelectOkTouch;

	SkinValueImage svimgSelectCancel;
	SkinValueRect svrectSelectCancel;
	SkinValueRect svrectSelectCancelTouch;

	// Main screen
	SkinValueImage svimgMain;

	SkinValueRect svrectHighScoresBox;
	SkinValueRect svrectGameModeBox;

	SkinValueImage svimgMainPlay;
	SkinValueRect svrectMainPlay;
	SkinValueRect svrectMainPlayTouch;

	SkinValueImage svimgMainExit;
	SkinValueRect svrectMainExit;
	SkinValueRect svrectMainExitTouch;

	SkinValueImage svimgMainConfig;
	SkinValueRect svrectMainConfig;
	SkinValueRect svrectMainConfigTouch;

	SkinValueImage svimgMainHelp;
	SkinValueRect svrectMainHelp;
	SkinValueRect svrectMainHelpTouch;


	// Play screen
	SkinValueImage svimgBackground;

	SkinValueRect svrectBoard;
	
	SkinValueImage svimgTiles;

	SkinValueRect svrectTilesInBoard;

	SkinValueRect svrectScore;
	SkinValueRect svrectBonus;
	SkinValueRect svrectBest;

	SkinValueImage svimgQuit;
	SkinValueRect svrectQuit;
	SkinValueRect svrectQuitTouch;

	SkinValueImage svimgNew;
	SkinValueRect svrectNew;
	SkinValueRect svrectNewTouch;

	SkinValueImage svimgPause;
	SkinValueRect svrectPause;
	SkinValueRect svrectPauseTouch;

	SkinValueImage svimgMute;
	SkinValueRect svrectMute;
	SkinValueRect svrectMuteTouch;

	SkinValueImage svimgPauseLegend;
	SkinValueRect svrectPauseLegend;
	SkinValueRect svrectPauseLegendTouch;

#ifdef DEBUG
	SkinValueRect svrectDebug;
#endif

	SkinValueImage svimgClock;
	SkinValueRect svrectClock;

	SkinValueImage svimgBonusLevel;
	SkinValueRect svrectBonusLevel;

	SkinValueRect svrectDiscard;

	SkinValueImage svimgScoreDigits;
	SkinValueText svtextScoreDigits;

	SkinValueImage svimgBonusScoreFont;
	SkinValueText svtextBonusScoreFont;

	SkinValueWave svwavePick;
	SkinValueWave svwaveDrop;
	SkinValueWave svwaveBurn;
	SkinValueWave svwaveScore;
	SkinValueWave svwaveBonus;
	SkinValueWave svwaveLevel;
	SkinValueWave svwaveDebit;
	SkinValueWave svwaveButton;
	SkinValueWave svwaveScroll;
	SkinValueWave svwaveSelect;

	SkinValueDict svdictWordList;

	SkinValueText svtextBonusList;
	SkinValueBoolean svboolBonusRandom;
	SkinValueScalar svscalarWordLengthMin;
	SkinValueScalar svscalarClockPerSwap;
	SkinValueScalar svscalarClockPerLetter;
	SkinValueScalar svscalarPictureDelay;

	SkinValueBoolean svboolTest;

	SkinValueImage aSvimgPicture[NPICTURE];

} SkinValuesLex;

extern SkinValuesLex svLex;

// Default values
enum {WIDTHINTILES=8, HEIGHTINTILES=8, BORDERWIDTH=8, BORDERHEIGHT=8, WIDTHDIGIT=10, HEIGHTDIGIT=14};
enum {SMALLESTWORD=4, CLOCKPERSWAP=1, CLOCKPERLETTER=1};

#define SCORELEFT 84
#define SCORETOP 390
#define SCORERIGHT 232
#define SCOREBOTTOM 416

#define BONUSLEFT 224
#define BONUSTOP 390
#define BONUSRIGHT 338
#define BONUSBOTTOM 416

#define BESTLEFT 224
#define BESTTOP 438
#define BESTRIGHT 338
#define BESTBOTTOM 464

#define EXITLEFT 10
#define EXITRIGHT 47
#define EXITTOP 294
#define EXITBOTTOM 312
#define EXITWIDTH 37
#define EXITHEIGHT 18

#define NEWLEFT 49
#define NEWRIGHT 86
#define NEWTOP 294
#define NEWBOTTOM 312
#define NEWWIDTH 37
#define NEWHEIGHT 18


// Pause button - not pause legend
#define PAUSELEFT 88
#define PAUSERIGHT 106
#define PAUSETOP 294
#define PAUSEBOTTOM 312
#define PAUSEWIDTH 18
#define PAUSEHEIGHT 18

#define MUTELEFT 108
#define MUTERIGHT 128
#define MUTETOP 294
#define MUTEBOTTOM 312
#define MUTEWIDTH 20
#define MUTEHEIGHT 18

#define CLOCKLEFT 8
#define CLOCKRIGHT 230
#define CLOCKTOP 240
#define CLOCKBOTTOM 248

#define DISCARDLEFT 11
#define DISCARDRIGHT 41
#define DISCARDTOP 254
#define DISCARDBOTTOM 286

#define LEGENDLEFT 79
#define LEGENDRIGHT 161
#define LEGENDTOP 109
#define LEGENDBOTTOM 131

#define LEVELLEFT 70
#define LEVELRIGHT 170
#define LEVELTOP 90
#define LEVELBOTTOM 110

#define SCREENWIDTH 240
#define SCREENHEIGHT 320

#endif
