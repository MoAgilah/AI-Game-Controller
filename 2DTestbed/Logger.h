#ifndef LoggerH
#define LoggerH

#include <SFML\Graphics.hpp>
#include "Utils.h"

#include <vector>
#include <string>
#include <fstream>
#include <ctime>

class Logger
{
public:
	Logger();
	void AddDebugLog(std::string msg, bool newLine = true);
	void AddExperimentLog(std::string msg, bool newLine = true);
	std::string GetTimeStamp();
	~Logger();
private:
	std::string tDate;

	std::ofstream debugfile;
	std::ofstream experifile;
};

#endif