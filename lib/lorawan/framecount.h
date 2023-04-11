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
    frameCount();
    frameCount(uint32_t theFrameCount);
    uint32_t asUint32() const;                      // return the frameCount as a uint32_t
    uint8_t asUint8(uint32_t index) const;          // return the deviceAddress as 4 bytes
    void fromUint32(uint32_t theFrameCount);        // set the frameCount
    void increment();
    static constexpr uint32_t length{4};            // [bytes]
  private:
    uint32_t theFrameCount;
    //const uint32_t nvsBlockIndex;
};
