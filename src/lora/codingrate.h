// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// TODO : check coding values with LoRaWAN spec

enum class codingRate : uint32_t {
    cr4_4 = 0x00,
    cr4_5 = 0x01,
    cr4_6 = 0x02,
    cr4_7 = 0x03,
    cr4_8 = 0x04,
};
