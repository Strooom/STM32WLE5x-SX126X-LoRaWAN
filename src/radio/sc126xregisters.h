// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class sx126xRegister : uint32_t {
    hoppingEnable = 0x0385,
    packetLength = 0x386,
    
    overcurrentProtection = 0x08E7,
    footCapacitorConfigA  = 0x0911,
    footCapacitorConfigA  = 0x0912

};

