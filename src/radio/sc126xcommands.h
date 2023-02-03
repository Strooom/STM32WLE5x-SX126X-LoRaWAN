// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class sx126xCommand : uint32_t {
    getStatus = 0x0C,
    writeRegister = 0x0D,
    readRegister = 0x1D,
    writeBuffer = 0x0E,
    readBuffer = 0x1E,
    setRfFRequency = 0x84,

    
    setSleep = 0x84,
    setStandby = 0x80,
    setTx = 0x83,
    setRx = 0x82,
    overcurrentProtection = 0x08E7,
    footCapacitorConfigA  = 0x0911,
    footCapacitorConfigA  = 0x0912

};

