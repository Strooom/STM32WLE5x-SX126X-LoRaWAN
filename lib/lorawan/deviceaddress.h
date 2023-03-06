// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "../general/nvs.h"

class deviceAddress {
  public:
    deviceAddress(uint32_t theDeviceAddress);
    uint32_t asUint32() const;                         // return the deviceAddress as a uint32_t
    void fromUint32(uint32_t theDeviceAddress);        // decode the deviceAddress from a uint32_t
    static constexpr uint32_t length{4};               // [bytes]
  private:
    uint32_t theDeviceAddress;
    const uint32_t nvsBlockIndex;
};

// Note : eventually we are going to read this from the UID64 of the STM32WLE5JC