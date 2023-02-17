// LexSkin.h - skin record definitions (format and values)
/*
 * Copyright (c) 2002,2003 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     12-04-02  JCA  Created
 *     03-04-04  JCA  Made into macros to save storing descriptions strings
 *
**/

#include "SkinResource.h"

SkinResources aSr[]={
SKINDESCRIPTOR("TransparentColor", SKIN_RGB, "One color must be chosen as the transparent color for the Tiles, ScoreDigits and BonusScoreFont images."),

SKINDESCRIPTOR("HelpBackground", SKIN_IMAGE, "A 240X320 image displayed behind scrolling text\n"	\
								"Can be gif or jpeg."),

SKINDESCRIPTOR("HelpBox", SKIN_RECT, "Text box for scrolling help."),

SKINDESCRIPTOR("HelpOk", SKIN_IMAGE, "Pressed OK button"),
SKINDESCRIPTOR("HelpOk", SKIN_RECT, "Location of OK button on background"),
SKINDESCRIPTOR("HelpOkTouch", SKIN_RECT, "Touch-sensitive part of OK button"),

SKINDESCRIPTOR("HelpAbout", SKIN_IMAGE, "Pressed About button"),
SKINDESCRIPTOR("HelpAbout", SKIN_RECT, "Location of About button on background"),
SKINDESCRIPTOR("HelpAboutTouch", SKIN_RECT, "Touch-sensitive part of About button"),

SKINDESCRIPTOR("Help", SKIN_TEXT, "The scrolling help text"),

// Config screen
SKINDESCRIPTOR("ConfigBackground", SKIN_IMAGE, "A 240X320 image displayed behind select boxes\n"
								"Can be gif or jpeg."),

SKINDESCRIPTOR("SkinsBox",			SKIN_RECT, "Location of skin selection box"),
SKINDESCRIPTOR("DictsBox",			SKIN_RECT, "Location of dictionary selections box"),

SKINDESCRIPTOR("ConfigOk", SKIN_IMAGE, "Pressed OK button"),
SKINDESCRIPTOR("ConfigOk", SKIN_RECT, "Location of OK button on background"),
SKINDESCRIPTOR("ConfigOkTouch", SKIN_RECT, "Touch-sensitive part of OK button"),

SKINDESCRIPTOR("ConfigCancel", SKIN_IMAGE, "Pressed OK button"),
SKINDESCRIPTOR("ConfigCancel", SKIN_RECT, "Location of OK button on background"),
SKINDESCRIPTOR("ConfigCancelTouch", SKIN_RECT, "Touch-sensitive part of OK button"),

// Main screen
SKINDESCRIPTOR("MainBackground", SKIN_IMAGE, "A 240X320 image displayed behind buttons\n"
								"Can be gif or jpeg."),
SKINDESCRIPTOR("HighScoresBox",		SKIN_RECT, "Location of text box"),
SKINDESCRIPTOR("GameModeBox",			SKIN_RECT, "Location of game select box"),

SKINDESCRIPTOR("MainPlay",			SKIN_IMAGE, "Pressed Play button"),
SKINDESCRIPTOR("MainPlay",			SKIN_RECT, "Location of Play button on background"),
SKINDESCRIPTOR("MainPlayTouch",		SKIN_RECT, "Touch-sensitive part of Play button"),

SKINDESCRIPTOR("MainExit",			SKIN_IMAGE, "Pressed Exit button"),
SKINDESCRIPTOR("MainExit",			SKIN_RECT, "Location of Exit button on background"),
SKINDESCRIPTOR("MainExitTouch",		SKIN_RECT, "Touch-sensitive part of Exit button"),

SKINDESCRIPTOR("MainConfig",			SKIN_IMAGE, "Pressed Config button"),
SKINDESCRIPTOR("MainConfig",			SKIN_RECT, "Location of Config button on background"),
SKINDESCRIPTOR("MainConfigTouch",		SKIN_RECT, "Touch-sensitive part of Config button"),

SKINDESCRIPTOR("MainHelp",			SKIN_IMAGE, "Pressed Help button"),
SKINDESCRIPTOR("MainHelp",			SKIN_RECT, "Location of Help button on background"),
SKINDESCRIPTOR("MainHelpTouch",		SKIN_RECT, "Touch-sensitive part of Help button"),

// Play screen
SKINDESCRIPTOR("GameBackground", SKIN_IMAGE, "A 240X320 image displayed behind everything else, including any border and the score boxes\n"
								"Can be gif or jpeg."),

SKINDESCRIPTOR("Board", SKIN_RECT, "Location of the playing pieces on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("Tiles", SKIN_IMAGE, "Letter block images in A-Z order.\nLetter match animation tiles appear in sequence below each letter."),
SKINDESCRIPTOR("BoardTiles", SKIN_RECT, "Number of tiles in board.\n0,0,nTileWide,nTilesHigh."),

SKINDESCRIPTOR("Score", SKIN_RECT, "Location of the numeric score on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("Bonus", SKIN_RECT, "Location of the bonus word on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("Best", SKIN_RECT, "Location of the best word on the background\nLeft,Top,Right,Bottom."),

SKINDESCRIPTOR("Quit", SKIN_IMAGE, "A 'pushed button' image.\n"),
SKINDESCRIPTOR("Quit", SKIN_RECT, "Location of the Quit button on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("QuitTouch", SKIN_RECT, "Touch-sensitive part of Quit button"),

SKINDESCRIPTOR("New", SKIN_IMAGE, "A 'pushed button' image.\n"),
SKINDESCRIPTOR("New", SKIN_RECT, "Location of the New button on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("NewTouch", SKIN_RECT, "Touch-sensitive part of New button"),

SKINDESCRIPTOR("Pause", SKIN_IMAGE, "A 'pushed button' image.\n"),
SKINDESCRIPTOR("Pause", SKIN_RECT, "Location of the Pause button on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("PauseTouch", SKIN_RECT, "Touch-sensitive part of Pause button"),

SKINDESCRIPTOR("Mute", SKIN_IMAGE, "A 'pushed button' image.\n"),
SKINDESCRIPTOR("Mute", SKIN_RECT, "Location of the Mute button on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("MuteTouch", SKIN_RECT, "Touch-sensitive part of Mute button"),

SKINDESCRIPTOR("PauseLegend", SKIN_IMAGE, "A legend to splat on the board when paused.\n"
								"Should blend with whatever is under it."),
SKINDESCRIPTOR("PauseLegend", SKIN_RECT, "Location of the pause legend on the background\nLeft,Top,Right,Bottom."),
SKINDESCRIPTOR("PauseLegendTouch", SKIN_RECT, "Location of the pause touch-sensitive region.\n"
								"Left,Top,Right,Bottom.\nShould be contained by PauseLegend."),
#ifdef DEBUG
SKINDESCRIPTOR("Debug", SKIN_RECT, "Location of the Debug button on the background\nLeft,Top,Right,Bottom."),
#endif


SKINDESCRIPTOR("Clock", SKIN_IMAGE, "A vertical 'filmstrip' of clock images.\n"
								"Zero time at the top, full time at the bottom."),
SKINDESCRIPTOR("Clock", SKIN_RECT, "Location of the clock on the background\nLeft,Top,Right,Bottom."),

SKINDESCRIPTOR("BonusLevel", SKIN_IMAGE, "A legend to splat on the board when the level is cleared in Crash.\n"),
SKINDESCRIPTOR("BonusLevel", SKIN_RECT, "Location of the level legend on the background\nLeft,Top,Right,Bottom."),

SKINDESCRIPTOR("Discard", SKIN_RECT, "Location of the discard area on the background\nLeft,Top,Right,Bottom."),

SKINDESCRIPTOR("ScoreDigits", SKIN_IMAGE, "A horizontal 'filmstrip' of the digits 0123456789-, used to display scores behind used tiles.\n"
								"Each tile should be smaller than a letter tile."),
SKINDESCRIPTOR("ScoreDigits", SKIN_TEXT, "The digits 0123456789-, used to display scores behind used tiles.\n"
								"They should correspond to their position in ScoreDigits image."),

SKINDESCRIPTOR("BonusScoreFont", SKIN_IMAGE, "Font of each digit in the numeric score and bonus word.\n0-9a-z."),
SKINDESCRIPTOR("BonusScoreFont", SKIN_TEXT, "Characters present in BonusScoreFont.\n0-9a-z."),

SKINDESCRIPTOR("Pick", SKIN_WAVE, "Sound of picking up a tile.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Drop", SKIN_WAVE, "Sound of dropping a tile.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Burn", SKIN_WAVE, "Sound of dissolving a tile that matched.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Score", SKIN_WAVE, "Sound of a word match score.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Bonus", SKIN_WAVE, "Sound of clearing the level.  Should play in < 0.75 seconds."),
SKINDESCRIPTOR("Level", SKIN_WAVE, "Sound of winning the level.  Should play in < 1 second."),
SKINDESCRIPTOR("Debit", SKIN_WAVE, "Sound of being penalized for a letter left on the board.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Button", SKIN_WAVE, "Sound of pressing button.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Scroll", SKIN_WAVE, "Sound of touching scroll bar.  Should play in < 0.5 seconds."),
SKINDESCRIPTOR("Select", SKIN_WAVE, "Sound of selecting from a Select box.  Should play in < 0.5 seconds."),

SKINDESCRIPTOR("WordList", SKIN_DICTIONARY, "Compressed dictionary of words to match against.  Can be any length."),
SKINDESCRIPTOR("BonusWordList", SKIN_TEXT, "List of bonus words in presentation order.\nComma-separated words no longer than the board width."),
SKINDESCRIPTOR("BonusRandom", SKIN_BOOLEAN, "True if bonus words are to be presented in random order.\nIf false, bonus words are presented in order listed."),
SKINDESCRIPTOR("WordMinimunLength", SKIN_SCALAR, "Minimum word length in letters to match (e.g. 4).\nDictionary can contain words smaller than this, but they won't match."),
SKINDESCRIPTOR("ClockPerSwap", SKIN_SCALAR, "Time penalty for each letter swapped.\nCan be zero."),
SKINDESCRIPTOR("ClockPerLetter", SKIN_SCALAR, "Time bonus for each letter matched.\nCan be zero."),
SKINDESCRIPTOR("PictureDelay", SKIN_SCALAR, "Time delay after clearing level."),

SKINDESCRIPTOR("Test", SKIN_BOOLEAN, "True if testing a skin and want Pause to mean EndLevel."),

SKINDESCRIPTOR("Picture0", SKIN_IMAGE, "A board-sized image displayed behind the tiles\n"
								"If more than one is present, they are displayed in sequence as the level is cleared."),
SKINDESCRIPTOR("Picture1", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture2", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture3", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture4", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture5", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture6", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture7", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture8", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture9", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture10", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture11", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture12", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture13", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture14", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture15", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture16", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture17", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture18", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture19", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture20", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture21", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture22", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture23", SKIN_IMAGE, ""),
SKINDESCRIPTOR("Picture24", SKIN_IMAGE, ""),

SKINDESCRIPTOR(NULL, SKIN_EOF, NULL)
};

