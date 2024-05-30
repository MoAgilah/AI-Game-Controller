#pragma once

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <random>

//----------------------------------------------------------------------------
//	some random number functions.
//----------------------------------------------------------------------------
//returns a random integer between x and y
inline int RandInt(int x, int y)
{
	std::default_random_engine generator;
	std::uniform_int_distribution distribution(x, y);
	return distribution(generator);
}

//returns a random float between zero and 1
inline double RandFloat()
{
	std::default_random_engine generator;
	std::uniform_real_distribution distribution(0.0, 1.0);
	return distribution(generator);
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }
