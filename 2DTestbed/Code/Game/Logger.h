#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <ctime>

class Logger
{
public:
	Logger();
	~Logger();

	void AddDebugLog(std::string msg, bool newLine = true);
	void AddExperimentLog(std::string msg, bool newLine = true);
	std::string GetTimeStamp();

private:

	std::string tDate;
	std::ofstream debugfile;
	std::ofstream experifile;
};