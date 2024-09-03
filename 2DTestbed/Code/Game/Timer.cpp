#include "Timer.h"

Timer::Timer(float maxTime)
	: m_maxTime(maxTime)
{
	ResetTime();
}

void Timer::Update(float deltaTime)
{
	if (!m_stopped)
		m_time -= deltaTime;
}

void Timer::ResetTime()
{
	m_time = m_maxTime;
}