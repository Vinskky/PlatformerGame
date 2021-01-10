// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

//#include "App.h"
#include "Timer.h"
//#include "SDL\include\SDL_timer.h"
#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"


// L07: DONE 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!
	
Timer::Timer()
{
	name.Create("timer");
}

Timer::~Timer()
{
}

bool Timer::Start()
{
	startTime = SDL_GetTicks();
	return true;
}

uint32 Timer::Read() const
{
	return (SDL_GetTicks() - startTime);
}

float Timer::ReadSec() const
{
	return float(Read() / 1000.0f);
}