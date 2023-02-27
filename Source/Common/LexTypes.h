// LexTypes.h - simple Lexicon types

#ifndef __LexTypes_h
#define __LexTypes_h

//#include "SimpleTypes.h"
#include "AppAfx.h"

#include <cstdint>
#include "stdafx.h"

typedef uint8_t U8;
typedef int16_t S16;
typedef uint16_t U16;
typedef int32_t S32;
typedef uint32_t U32;

typedef enum { ENextLevel, ELetterNew, ELetterUsed, ELettersUsed, EQuit, ENew, EPause, EMute, ETileDropped, ESwap, EStill, EScore, EScoreUpdate, EBonusWord, EBonusUsed, EBestWord, EBoardExhausted, EBoardImpossible, EWinComplete, EMainWindow, ELevelBonus
} EventType;

typedef enum { NLeft, NRight, NTop, NBottom, NNot } NeighborType;

enum {	// Notifiations to parent element
	notifyRESET = 1,
	notifyMOVE = 2,
	notifyCOPY = 3,
	notifyRELEASE = 4,
	notifyRETURN = 5,
	notifyFLOATER = 6,
	notifyIDLE = 7,
	notifySTILL = 8
};

class Coord { 
public:
	S16 x; 
	S16 y; 
	Coord(): x(0), y(0) {}; 
	Coord(U32 _x, U32 _y):x((S16)_x),y((S16)_y) {} 
	bool operator ==(const Coord &cd) const { return (cd.x == x && cd.y == y); }
	Coord &operator =(const Coord &cd) { x=cd.x; y=cd.y; return *this; }
	Coord Center(U32 dx, U32 dy) const { return Coord(x + (S16)dx/2, y + (S16)dy/2); }
	Coord operator*(U32 c) { return Coord(x*c, y*c); }
	Coord &operator+=(Coord &cd) { x += cd.x; y += cd.y; return *this; }
	operator CPoint() const{ return CPoint(x,y); }
};

// Must be global scope so 1st argument can be const (?)
extern Coord operator+(const Coord &cd1, const Coord &cd2);
extern Coord operator-(const Coord &cd1, const Coord &cd2);

typedef struct SwapTiles {
	Coord tileA;
	Coord tileB;
	SwapTiles(Coord& a, Coord& b) :tileA(a), tileB(b) {}
} SwapTiles;

#endif

