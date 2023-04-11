// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

#include "aeskey.h"

class messageIntegrityCode {
  public:
    uint32_t calculate(aesKey aKey);                                           // calculates the MIC over the buffer, using the key
    bool validate(const uint8_t* buffer, uint32_t length, aesKey aKey);        // validates the MIC over the buffer, using the key
    static constexpr uint32_t length{4};                                       // [bytes]
    uint8_t asUint8(uint32_t index) const;

  private:
    uint32_t micValue{0};
};
