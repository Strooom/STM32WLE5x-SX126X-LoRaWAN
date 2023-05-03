// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class deviceAddress {
  public:
    deviceAddress();
    deviceAddress(uint32_t theDeviceAddress);
    deviceAddress(uint8_t theDeviceAddress[4]);
    void set(uint32_t theDeviceAddress);          // decode the deviceAddress from a uint32_t
    void set(uint8_t theDeviceAddress[4]);        // set the frameCount from an array of 4 bytes

    union {
        uint32_t asUint32{0};
        uint8_t asUint8[4];
    };

  private:
};


