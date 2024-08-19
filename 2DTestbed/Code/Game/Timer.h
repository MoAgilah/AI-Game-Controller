#pragma once

class Timer
{
public:
	~Timer();

	static Timer* Get();

	float CurrentTime() const { return m_time; }

	void UpdateTime(float deltaTime);

	bool CheckEnd() const { return m_time <= 0.0f; }
	void ResetTime();
	void Stop();
private:
	Timer();

	static Timer* m_self;
	float m_time;
	bool m_stopped;
};