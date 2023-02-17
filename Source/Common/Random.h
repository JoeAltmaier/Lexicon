// Random.h - random number generators

#ifndef __Random_h
#define __Random_h

#include "LexTypes.h"

#ifdef UNDER_CE
extern "C" U32 Random();
#define RANDOM ::Random
#define INITIALIZERANDOM 
#else
#include <stdlib.h>
#define RANDOM rand
#include <time.h>
#define INITIALIZERANDOM srand((unsigned)time(NULL))
#endif



//   srand( (unsigned)time( NULL ) );

class Random {
public:
	static void Initialize() { INITIALIZERANDOM; }
	static U32 Value() { return RANDOM(); }
	template <class T>
	static T Value(T limit) { return (limit? Value() % limit :0); }
	static U32 Range(U32 minValue, U32 maxValue) { return minValue + (maxValue > minValue? Value(maxValue - minValue) :0); }
	static U32 Flip() { return Value(2); }
};

#endif
