// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class gpioGroup : uint32_t {
    none = 0,
    rfControl,
    i2c,
    writeProtect,
    i2cSensors,
    i2cEeprom,
    spiDisplay,
    debugPort,
    uart1,
    uart2,
    usbPresent
};


void enableGpio(gpioGroup aGroup);
void disableGpio(gpioGroup aGroup);
