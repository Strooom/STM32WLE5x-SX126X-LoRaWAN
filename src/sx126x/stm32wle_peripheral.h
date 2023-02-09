// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once

#include <stdint.h>

#define peripheralBaseAddress	0x4000000

class stm32wlePeripheral {
public:
	virtual void initialize() = 0;
	virtual void prepareForSleep() = 0;
	virtual void restoreAfterSleep() = 0;
	bool isInitialized();

private:
	bool initialized{false};
};


