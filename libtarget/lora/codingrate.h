// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// It seems LoRaWAN fixes the coding rate to 4/5 (for the payload) and 4/8 (for the header), so we don't really need all the others

enum class codingRate : uint32_t {
    cr4_4 = 0x00,
    cr4_5 = 0x01,
    cr4_6 = 0x02,
    cr4_7 = 0x03,
    cr4_8 = 0x04,
};
