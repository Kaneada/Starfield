#ifndef _TIMER_H
#define _TIMER_H

#include <SDL.h>

class Timer
{
private:

	Uint32 mStartTicks;
	Uint32 mElaspedTicks;
	float mDeltaTime;
	float mTimeScale;

public:

	//Construct Destruct
	Timer();
	~Timer();

	void Reset();
	void Update();

	void setTimeScale(float t);
	float getTimeScale();

	float getDeltaTime();
	



};


#endif
