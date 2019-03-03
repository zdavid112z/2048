#include "pch.h"
#include "timer.h"

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	m_Delta = 0;
	m_Clock = std::chrono::high_resolution_clock::now();
	m_Start = m_Clock;
}

void Timer::Update()
{
	m_Aux = std::chrono::high_resolution_clock::now();
	m_Duration = m_Aux - m_Clock;
	m_Delta = m_Duration.count();
	m_Clock = m_Aux;
}

void Timer::SetTimeFactor(float f)
{
	m_TimeFactor = f;
}

float Timer::GetTimeFactor()
{
	return m_TimeFactor;
}

float Timer::GetDeltaTimeRaw()
{
	return m_Delta;
}

float Timer::GetDeltaTime()
{
	return m_Delta * m_TimeFactor;
}
