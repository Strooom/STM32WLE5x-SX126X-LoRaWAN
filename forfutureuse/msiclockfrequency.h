// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class msiClockFrequency : uint32_t {
    f100kHz = 100'000U,
    f200kHz = 200'000U,
    f400kHz = 400'000U,
    f800kHz = 800'000U,
    f1MHz   = 1'000'000U,
    f2MHz   = 2'000'000U,
    f4MHz   = 4'000'000U,
    f8MHz   = 8'000'000U,
    f16MHz  = 16'000'000U,
    f24MHz  = 24'000'000U,
    f32MHz  = 32'000'000U,
    f48MHz  = 48'000'000U
};

const char* toString(msiClockFrequency theMsiClockFrequency);
