#include <SFML/Graphics.hpp>

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#ifndef UtilsH
#define UtilsH

	//Dcontrol shows debug for the controller
	#define DControl

	//DRender shows debug for the testbed
	#define DRender

	//scale factor
	#define sX 2.34375f
	#define sY 2.5f

	//screen dimensions
	#define scrX 600
	#define scrY 600

	#define Automated false
	#define RightMost 11350.f

	#define FPS 60.0f


struct Sensor
{
	sf::Vertex points[3];
	bool halfCol, endCol;
	void ClearSensor()
	{
		halfCol = endCol = false;
	}
};

	//----------------------------------------------------------------------------
	//	some random number functions.
	//----------------------------------------------------------------------------

	//returns a random integer between x and y
inline int	  RandInt(int x, int y) { return rand() % (y - x + 1) + x; }

//returns a random float between zero and 1
inline double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }

//returns a random bool
inline bool   RandBool()
{
	if (RandInt(0, 1)) return true;

	else return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }


//-----------------------------------------------------------------------
//
//	some handy little functions
//-----------------------------------------------------------------------

//converts an integer to a string
inline std::string itos(int arg)
{
	std::ostringstream buffer;

	//send the int to the ostringstream
	buffer << arg;

	//capture the string
	return buffer.str();
}



//converts a float to a string
inline std::string ftos(float arg)
{
	std::ostringstream buffer;

	//send the int to the ostringstream
	buffer << arg;

	//capture the string
	return buffer.str();
}

//clamps the first argument between the second two
inline void Clamp(double &arg, double min, double max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}

inline void Clamp(int &arg, int min, int max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}

//rounds a double up or down depending on its value
inline int Rounded(double val)
{
	int    integral = (int)val;
	double mantissa = val - integral;

	if (mantissa < 0.5)
	{
		return integral;
	}

	else
	{
		return integral + 1;
	}
}

//rounds a double up or down depending on whether its
//mantissa is higher or lower than offset
inline int RoundUnderOffset(double val, double offset)
{
	int    integral = (int)val;
	double mantissa = val - integral;

	if (mantissa < offset)
	{
		return integral;
	}

	else
	{
		return integral + 1;
	}
}


/////////////////////////////////////////////////////////////////////
//
//	Point structure
//
/////////////////////////////////////////////////////////////////////
struct SPoint
{
	float x, y;

	SPoint() {}
	SPoint(float a, float b) :x(a), y(b) {}
};


#endif // !UtilsH
