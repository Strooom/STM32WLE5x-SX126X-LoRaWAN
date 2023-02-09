// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class smpsDriveSetting : uint32_t {
drive20 = 0x00,
drive40 = 0x02,
drive60 = 0x04,
drive100 = 0x06

// #define SMPS_DRV_20  ((uint8_t) ((0x0)<<1))
// #define SMPS_DRV_40  ((uint8_t) ((0x1)<<1))
// #define SMPS_DRV_60  ((uint8_t) ((0x2)<<1))
// #define SMPS_DRV_100 ((uint8_t) ((0x3)<<1))
// #define SMPS_DRV_MASK ((uint8_t) ((0x3)<<1))

};

