#pragma once

class Timer
{
public:
	Timer(float maxTime);
	~Timer() = default;

	void Update(float deltaTime);

	float GetTime() const { return m_time; }
	bool CheckEnd() const { return m_time <= 0.0f; }
	void ForceEnd() { m_time = 0; }
	void SetTime(float time) { m_time = time; }

	void Pause() { m_stopped = true; }
	void Resume() { m_stopped = false; }
	void ResetTime();

private:

	bool m_stopped = false;
	float m_time = 0;
	float m_maxTime;
};