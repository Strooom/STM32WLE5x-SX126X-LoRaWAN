#pragma once

#include "timertype.h"


class timer {
public:
	timer();
	virtual ~timer();

	void set();
	void get();

private:
	timerType type{timerType::oneShot};
	bool running{false};

};


