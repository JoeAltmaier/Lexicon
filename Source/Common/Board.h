// Board.h - declaration of board animation of Lexicon rules
/*
 * Copyright (c) 2002 by
 *      Joe Altmaier  Derelict Software
 *
 * Revision History:
 *     11-25-02  JCA  Created
 *
**/

#ifndef __Board_h
#define __Board_h

#include "Lexicon.h"
#include "Configuration.h"

class MainWnd;

class Board: public Lexicon {
	MainWnd &winBoard;

//	MainApp &app;
	Configuration &config;

public:
	Board(Configuration& _config, MainWnd& _winBoard);
	~Board() {}

	void Event(EventType, void *);
	void Event(EventType, Coord);

#ifdef DEBUG
//	void Debug() { app.Debug(); }
#endif
private:
};

#endif
