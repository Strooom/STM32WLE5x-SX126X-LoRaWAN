#pragma once
#include <stdint.h>

#include "timer.h"

class timers {
public:
	timers();
	virtual ~timers();
	void allocate();
	void release();

private:
	static constexpr uint32_t maxNmbrOfTimers{16};
	timer theTimers[maxNmbrOfTimers];
};
