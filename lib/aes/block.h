// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class block {
  public:
    void setFromBinaryData(const uint8_t* binarySourceData);
    void setFromHexString(const char* hexStringSourceData);
    void toString(char* destinationString) const;
    static constexpr uint32_t length{16};

#ifndef unitTesting
  private:
#endif
    void xorBlock(block data);
    void shiftLeft();
    uint8_t rawData[length]{0};
    friend class aes;
};