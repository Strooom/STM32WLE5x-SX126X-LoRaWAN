// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class sleepMode : uint8_t {
    coldStart = 0x0,
    warmStart = 0x1
};


enum class standbyMode : uint8_t {
    rc   = 0x00,
    xosc = 0x01

};

