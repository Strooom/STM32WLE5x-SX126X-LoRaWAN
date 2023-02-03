// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class spreadingFactor : uint32_t {
    SF7,
    SF7,
    SF9,
    SF10,
    SF11,
    SF12
};

const char* toString(spreadingFactor aSpreadingFactor);