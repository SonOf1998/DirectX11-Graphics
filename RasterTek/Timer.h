#pragma once

class Timer
{
	LARGE_INTEGER referencePoint;
	LARGE_INTEGER frequency;

	LARGE_INTEGER elapsedSinceStart;
	LARGE_INTEGER oldElapsedSinceStart;
	long long deltaT;

public:

	Timer();

	void Update();

	float GetT  () const noexcept;
	float GetDt () const noexcept;
};