#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"
#include "Module.h"

class Timer : public Module
{
public:

	// Constructor
	Timer();
	~Timer();

	bool Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32 startTime;
};

#endif //__TIMER_H__