// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class rampTime : uint8_t {
    rt10us   = 0x00,
    rt20us   = 0x01,
    rt40us   = 0x02,
    rt80us   = 0x03,
    rt200us  = 0x04,
    rt800us  = 0x05,
    rt1700us = 0x06,
    rt3400us = 0x07
};

const char* toString(rampTime theRampTime);
