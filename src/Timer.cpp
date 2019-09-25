#include "Timer.h"

Timer::Timer()
{
	Reset();
	mTimeScale = 1.0f;
}

Timer::~Timer()
{
}

void Timer::Reset()
{
	mStartTicks = SDL_GetTicks();
	mElaspedTicks = 0;
	mDeltaTime = 0.0f;

}

void Timer::Update()
{
	mElaspedTicks = SDL_GetTicks() - mStartTicks;
	mDeltaTime = mElaspedTicks * 0.001f;
}

void Timer::setTimeScale(float t)
{
	mTimeScale = t;
}

float Timer::getTimeScale()
{
	return mTimeScale;
}

float Timer::getDeltaTime()
{
	return mDeltaTime;
}
