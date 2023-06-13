// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include "stdint.h"

enum class batteryType : uint8_t {
    none             = 0x00,        // no battery, device powered by USB
    liFePO4_700mAh   = 0x01,        // AA LiFePO4 battery
    liFePO4_1500mAh  = 0x02,        // 18650 LiFePO4 battery
    alkaline_1200mAh = 0x03,        // 2 AAA Alkaline batteries
};
