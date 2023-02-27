// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class messageIntegrityCode {
  public:
    uint32_t calculate(const uint8_t* buffer, uint32_t length, const uint8_t* key) const;        // calculates the MIC over the buffer, using the key
    bool validate(const uint8_t* buffer, uint32_t length, const uint8_t* key) const;             // validates the MIC over the buffer, using the key
    static constexpr uint32_t length{4};                                                         // [bytes]
  private:
};
