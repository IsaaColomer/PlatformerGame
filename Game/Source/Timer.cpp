// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"

// L07: DONE 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!
	
Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	//...
}

uint32 Timer::Read() const
{
	return 0;
}

float Timer::ReadSec() const
{
	return 0.0f;
}