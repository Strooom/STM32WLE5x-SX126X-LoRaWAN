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
    void set(uint32_t theFrameCount);                                                    // set the frameCount from a uint32_t
    void set(uint8_t theFrameCount[4]);                                                  // set the frameCount from an array of 4 bytes
    void increment();                                                                    //
    static uint32_t guessFromUint16(uint32_t frameCount32, uint16_t frameCount16Lsb);        // guess the 32 bit framecount from a 16-bit lsb value
    
    union {
        uint32_t asUint32{0};
        uint8_t asUint8[4];
    };

  private:
};
