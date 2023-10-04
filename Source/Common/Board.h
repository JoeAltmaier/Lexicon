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

class MainWnd;

class Board: public Lexicon {
	MainWnd &winBoard;

public:
	Board(Configuration& _config, MainWnd& _winBoard);
	~Board() {}

	virtual void Event(EventType, void *) override;
	void Event(EventType, Coord);
};

#endif
