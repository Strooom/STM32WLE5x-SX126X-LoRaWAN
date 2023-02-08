// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class mode : uint32_t {
    BW500 = 0x06,
    BW250 = 0x05,
    BW125 = 0x04,
    BW62  = 0x03,
    BW41  = 0x0A,
    BW31  = 0x02,
    BW20  = 0x09,
    BW15  = 0x01,
    BW10  = 0x08,
    BW7   = 0x00
};

const char* toString(mode aMode);
float getBandwidth(mode aMode);
uint32_t getIntermediateFRequency(mode aMode);