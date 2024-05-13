#include "Logger.h"

const std::string pID("_SensorInputs");

Logger::Logger()
{
	std::string path = "Debug(20_01_2018)";
	m_debugfile.open(path + ".csv");
	m_debugfile << path << GetTimeStamp() << std::endl;

	path = "Experiment(20_01_2018)"  + pID;
	m_experifile.open(path + ".csv");
	m_experifile << path << std::endl << GetTimeStamp() << std::endl;
}

void Logger::AddDebugLog(const std::string& msg, bool newLine)
{
	if (newLine)
	{
		m_debugfile << msg << std::endl;
	}
	else
	{
		m_debugfile << " " + msg;
	}
}

void Logger::AddExperimentLog(const std::string& msg, bool newLine)
{
	if (newLine)
	{
		m_experifile << msg << std::endl;
	}
	else
	{
		m_experifile << msg;
	}
}

std::string Logger::GetTimeStamp() const
{
	auto const time = std::chrono::current_zone()
		->to_local(std::chrono::system_clock::now());
	return std::format("{:%Y-%m-%d %X}", time);
}


