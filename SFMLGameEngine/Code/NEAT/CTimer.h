#pragma once
//-----------------------------------------------------------------------
//
//  Name: CTimer.h
//
//  Author: Mat Buckland 2002
//  Edited: Mo Agilah 2024
//
//	Desc: Windows timer class
//
//-----------------------------------------------------------------------
#define NOMINMAX
#include <windows.h>
#include <iostream>

class CTimer
{
public:

	//ctors
	CTimer();
	CTimer(float fps);

	//whatdayaknow, this starts the timer
	void Start();

	//determines if enough time has passed to move onto next frame
	bool ReadyForNextFrame();

	//only use this after a call to the above.
	double GetTimeElapsed() { return m_TimeElapsed; }

	double TimeElapsed();

private:

	LONGLONG	m_CurrentTime;
	LONGLONG	m_LastTime;
	LONGLONG	m_NextTime;
	LONGLONG	m_FrameTime;
	LONGLONG	m_PerfCountFreq;

	double		m_TimeElapsed;
	double		m_TimeScale;

	float		m_FPS;
};