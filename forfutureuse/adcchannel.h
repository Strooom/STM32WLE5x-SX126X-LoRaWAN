// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class adcChannel : uint32_t {
	none = 0,
    internalVoltageReference = 13,
	internalTemperatureSensor = 14
};

const char* toString(adcChannel aChannel);
