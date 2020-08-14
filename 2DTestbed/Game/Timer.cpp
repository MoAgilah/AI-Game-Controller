#include "../Game/Timer.h"

Timer* Timer::self = nullptr;

Timer::~Timer() 
{
	if (self)
	{
		delete self;
		self = nullptr;
	}
}

 Timer* Timer::Get()
{
	if (self == nullptr)
	{
		self = new Timer();
	}

	return self;
}

float Timer::CurrentTime()
{
	return m_time;
}

void Timer::UpdateTime(float deltaTime)
{
	m_time -= deltaTime;
}

bool Timer::CheckEnd()
{
	return m_time <= 0.0f;
}

void Timer::ResetTime()
{
	m_time = 300.0f;
}

void Timer::Stop()
{
	stopped = true;
}

Timer::Timer() 
{
	m_time = 300.0f;
}
