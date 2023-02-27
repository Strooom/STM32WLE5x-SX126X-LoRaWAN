// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class macHeader {
  public:
    macHeader(frameType theFrameType);          // construct a macHeader with the given frameType
    uint8_t asByte() const;                     // return the macHeader as a byte
    static constexpr uint32_t length{1};        // [bytes]
  private:
    frameType theFrameType;                                    // bits [7:5]
                                                               // bits [4:2] are RFU
    static constexpr uint32_t majorDataFrameVersion{0};        // bits [1:0] are always 0 for LoRaWAN R1
};



