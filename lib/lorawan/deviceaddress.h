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
    static constexpr uint32_t length{4};                    // length of the deviceAddress in bytes
    deviceAddress();                                        //
    deviceAddress(uint32_t theDeviceAddress);               //
    deviceAddress(uint8_t theDeviceAddress[length]);        //
    void set(uint32_t theDeviceAddress);                    // decode the deviceAddress from a uint32_t
    void set(uint8_t theDeviceAddress[length]);             // set the frameCount from an array of 4 bytes

    union {
        uint32_t asUint32{0};
        uint8_t asUint8[length];
    };

  private:
};

