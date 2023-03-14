// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class aesKey {
  public:
    aesKey();                                       // initialize the key to all zeros
    bool setFromASCII(const char* asciiKey);        // initialize the key from an ASCII string, returns true if successful
    // TODO : check how I can make this safer by returning a pointer to a const array..
    const uint8_t* asBinary() const;        // returns a pointer to the key as a binary array
    unsigned char* asUnsignedChar();        // returns a pointer to the key as a binary array but typed as unsigned char
    const uint8_t* asASCII() const;         // returns a pointer to the key as an ASCII string

    static constexpr uint32_t binaryKeyLength{16};        // 128 bits = 16 bytes
    static constexpr uint32_t asciiKeyLength{33};         // 16 bytes * 2 characters per byte + 1 for the terminating zero

  private:
    uint8_t key[binaryKeyLength];
    uint8_t keyAsASCII[asciiKeyLength];

    // TODO : the key could be expanded upfront to save time during encryption - as the key doesn't change, the expanded key should be constant as well
};
