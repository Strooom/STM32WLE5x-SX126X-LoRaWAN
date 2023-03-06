// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class frameCount {
  public:
    frameCount(uint32_t theFrameCount);
    uint16_t asUint32() const;                         // return the frameCount as a uint16_t
    void fromUint32(uint16_t theFrameCount);           // decode the frameCount from a uint16_t
    static constexpr uint32_t length{4};               // [bytes]
  private:
    uint32_t theFrameCount;
    const uint32_t nvsBlockIndex;
};