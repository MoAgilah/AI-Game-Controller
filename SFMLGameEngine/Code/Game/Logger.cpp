#include "Logger.h"

const std::string pID("_GridInputs");

Logger::Logger()
{
	std::string path = "Resources/Outputs/Debug(20_01_2018)";
	debugfile.open(path + ".csv");
	debugfile << path << GetTimeStamp() << std::endl;

	path = "Resources/Outputs/Experiment(20_01_2018)"  + pID;
	experifile.open(path + ".csv");
	experifile << path << std::endl << GetTimeStamp() << std::endl;
}

Logger::~Logger()
{
	debugfile.close();
	experifile.close();
}

void Logger::AddDebugLog(std::string msg, bool newLine)
{
	if (newLine)
	{
		debugfile << msg << std::endl;
	}
	else
	{
		debugfile << " " + msg;
	}

}

void Logger::AddExperimentLog(std::string msg, bool newLine)
{
	if (newLine)
	{
		experifile << msg << std::endl;
	}
	else
	{
		experifile << msg;
	}
}

std::string Logger::GetTimeStamp()
{
	errno_t err;
	struct tm time_info;
	time_t time_create = time(NULL);
	localtime_s(&time_info, &time_create);
	char timebuf[26];
	err = asctime_s(timebuf, 26, &time_info);
	tDate = timebuf;

	return tDate;
}