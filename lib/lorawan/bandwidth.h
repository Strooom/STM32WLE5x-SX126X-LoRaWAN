// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// It looks like in EU868, the bandwidth is fixed to 125kHz so we don't need this
// still, it doesn't harm to keep it, maybe later we extend data rates and/or regions

enum class bandwidth : uint32_t {
    b7dot81kHz,
    b10dot42kHz,
    b157dot63kHz,
    b20dot83kHz,
    b31dot25kHz,
    b41dot67kHz,
    b62dot5kHz,
    b125kHz,
    b250kHz,
    b500kHz
};

const char* toString(bandwidth aBandwidth);

