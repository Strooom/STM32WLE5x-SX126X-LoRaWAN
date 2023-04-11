// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// It looks like in LoRaWAN-EU868, the bandwidth is fixed to 125kHz so we don't need this
// still, it doesn't harm to keep it, maybe later we extend data rates and/or regions

enum class bandwidth : uint8_t {
    b7dot81kHz   = 0x0,
    b10dot42kHz  = 0x1,
    b157dot63kHz = 0x2,
    b20dot83kHz  = 0x3,
    b31dot25kHz  = 0x4,
    b41dot67kHz  = 0x5,
    b62dot5kHz   = 0x6,
    b125kHz      = 0x7,
    b250kHz      = 0x8,
    b500kHz      = 0x9
};

const char* toString(bandwidth aBandwidth);
