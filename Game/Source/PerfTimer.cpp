// ----------------------------------------------------
// Slow timer with microsecond precision
// ----------------------------------------------------


#include "PerfTimer.h"
//#include "SDL\include\SDL_timer.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

// L07: DONE 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
// they are simple, one line each!

PerfTimer::PerfTimer()
{
	if (frequency == 0) frequency = SDL_GetPerformanceFrequency();

	Start();
}

PerfTimer::~PerfTimer() {};

bool PerfTimer::Start()
{
	startTime = SDL_GetPerformanceCounter();

	return true;
}

double PerfTimer::ReadMs() const
{
	return (1000.0 * (double(SDL_GetPerformanceCounter() - startTime) / double(frequency)));
}

uint64 PerfTimer::ReadTicks() const
{
	return (SDL_GetPerformanceCounter() - startTime);
}
