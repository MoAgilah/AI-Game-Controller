#pragma once

#include <SFML\Graphics.hpp>
#include "../Utils.h"

#include <vector>
#include <string>
#include <fstream>
#include <format>
#include <chrono>

class Logger
{
public:
	Logger();
	~Logger() = default;
	void AddDebugLog(const std::string& msg, bool newLine = true);
	void AddExperimentLog(const std::string& msg, bool newLine = true);
	std::string GetTimeStamp() const;
private:
	std::ofstream m_debugfile;
	std::ofstream m_experifile;
};