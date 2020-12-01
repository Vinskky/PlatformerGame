#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__

#include "Defs.h"
#include "Module.h"

class PerfTimer : public Module
{
public:

	// Constructor
	PerfTimer();

	bool Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64 startTime;
	static uint64 frequency;
};

#endif //__PERFTIMER_H__
