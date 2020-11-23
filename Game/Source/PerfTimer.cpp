// ----------------------------------------------------
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfTimer::frequency = 0;

// L07: DONE 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
// they are simple, one line each!

PerfTimer::PerfTimer()
{
	//...

	Start();
}

void PerfTimer::Start()
{
	//...
}

double PerfTimer::ReadMs() const
{
	return 0.0;
}

uint64 PerfTimer::ReadTicks() const
{
	return 0;
}
