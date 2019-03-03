#pragma once

#include "pch.h"
#include <chrono>

class Timer
{
public:
	Timer();
	void Reset();
	void Update();
	void SetTimeFactor(float);
	float GetTimeFactor();
	float GetDeltaTimeRaw();
	float GetDeltaTime();
private:
	float m_TimeFactor = 1;
	float m_Delta = 0;
	std::chrono::duration<float> m_Duration;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Clock, m_Aux, m_Start;
};