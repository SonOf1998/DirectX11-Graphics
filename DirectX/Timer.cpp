#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
	LARGE_INTEGER timestamp;
	LARGE_INTEGER freq;
	QueryPerformanceCounter(&timestamp);
	QueryPerformanceFrequency(&freq);

	referencePoint = timestamp;
	frequency = freq;
	
	elapsedSinceStart = referencePoint;
	oldElapsedSinceStart = LARGE_INTEGER{0};
}

void Timer::Update()
{
	LARGE_INTEGER timestamp;
	QueryPerformanceCounter(&timestamp);

	elapsedSinceStart = timestamp;
	deltaT = elapsedSinceStart.QuadPart - oldElapsedSinceStart.QuadPart;
	oldElapsedSinceStart = elapsedSinceStart;
}

float Timer::GetT() const noexcept
{
	float t = static_cast<float>(elapsedSinceStart.QuadPart - referencePoint.QuadPart);
	t /= frequency.QuadPart;

	return t;
}

float Timer::GetDt() const noexcept
{
	float dt = static_cast<float>(deltaT);
	dt /= frequency.QuadPart;

	dt = std::min(dt, 0.1f);
	return dt;
	//return dt > 1 ? dt : 0f;		// huge dt values are signaled with stutters in objects' movement
}
