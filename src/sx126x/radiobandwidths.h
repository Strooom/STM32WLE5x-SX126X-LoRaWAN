// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class sx126xRadioBandwidth : uint32_t {
    RX_BW_4800   = 0x1F,
    RX_BW_5800   = 0x17,
    RX_BW_7300   = 0x0F,
    RX_BW_9700   = 0x1E,
    RX_BW_11700  = 0x16,
    RX_BW_14600  = 0x0E,
    RX_BW_19500  = 0x1D,
    RX_BW_23400  = 0x15,
    RX_BW_29300  = 0x0D,
    RX_BW_39000  = 0x1C,
    RX_BW_46900  = 0x14,
    RX_BW_58600  = 0x0C,
    RX_BW_78200  = 0x1B,
    RX_BW_93800  = 0x13,
    RX_BW_117300 = 0x0B,
    RX_BW_156200 = 0x1A,
    RX_BW_187200 = 0x12,
    RX_BW_234300 = 0x0A,
    RX_BW_312000 = 0x19,
    RX_BW_373600 = 0x11,
    RX_BW_467000 = 0x09,
};
