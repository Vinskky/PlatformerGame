// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "App.h"
#include "Timer.h"
#include "SDL\include\SDL_timer.h"
#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"


// L07: DONE 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!
	
Timer::Timer()
{
	Start();
}

bool Timer::Start()
{
	return Read();
}

uint32 Timer::Read() const
{
	return SDL_GetTicks();
}

float Timer::ReadSec() const
{
	return 0.0f;
}