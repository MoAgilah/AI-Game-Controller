#include "Timer.h"

Timer* Timer::m_self = nullptr;

Timer::~Timer()
{
	if (m_self)
	{
		delete m_self;
		m_self = nullptr;
	}
}

 Timer* Timer::Get()
{
	if (m_self == nullptr)
	{
		m_self = new Timer();
	}

	return m_self;
}

void Timer::UpdateTime(float deltaTime)
{
	m_time -= deltaTime;
}

void Timer::ResetTime()
{
	m_time = 300.0f;
}

void Timer::Stop()
{
	m_stopped = true;
}

Timer::Timer()
	: m_time(300.f), m_stopped(false)
{}
