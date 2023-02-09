// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class codingRate : uint32_t {
    CR4_4 = 0x00,
    CR4_5 = 0x01,
    CR4_6 = 0x02,
    CR4_7 = 0x03,
    CR4_8 = 0x04,
};
