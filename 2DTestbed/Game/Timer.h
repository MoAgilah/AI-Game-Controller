#ifndef TimerH
#define TimerH

class Timer
{
public:
	~Timer();

	static Timer* Get();

	float CurrentTime();

	void UpdateTime(float deltaTime);

	bool CheckEnd();
	void ResetTime();
	void Stop();
private:
	Timer();

	static Timer* self;
	float m_time;
	bool stopped;
};

#endif	//TimerH