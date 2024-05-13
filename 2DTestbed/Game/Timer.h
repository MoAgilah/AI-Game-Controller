#pragma once

class Timer
{
public:
	~Timer();

	static Timer* Get();

	float CurrentTime() const;

	void UpdateTime(float deltaTime);

	bool CheckEnd() const;
	void ResetTime();
	void Stop();
private:
	Timer();

	static Timer* self;
	float m_time;
	bool stopped;
};