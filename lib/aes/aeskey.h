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
    aesKey();                                            // initialize the key to all zeros
    void setFromASCII(const char* asciiKey);             // initialize the key from an ASCII string, returns true if successful
    void setFromBinary(const uint8_t* binaryKey);        // initialize the key from an array of 16 bytes

    // TODO : check how I can make this safer by returning a pointer to a const array..
    uint8_t* asBinary();                    // returns a pointer to the key as a binary array
    unsigned char* asUnsignedChar();        // returns a pointer to the key as a binary array but typed as unsigned char
    uint8_t* asASCII();                     // returns a pointer to the key as an ASCII string

    static constexpr uint32_t binaryKeyLength{16};        // 128 bits = 16 bytes
    static constexpr uint32_t asciiKeyLength{33};         // 16 bytes * 2 characters per byte + 1 for the terminating zero

  private:
    uint8_t key[binaryKeyLength];
    uint8_t keyAsASCII[asciiKeyLength];

    // TODO : the key could be expanded upfront to save time during encryption - as the key doesn't change, the expanded keys should be constant as well
};

// NOTES :
// * I think implementation could be faster if keys are defined as uint32_t[4] instead of uint8_t[16]
// * We need an additional 10 keys for the 10 rounds of the AES algorithm - they can be precomputed from the original key
// * As a single bit-change in the keys, has dramatic effects, it's maybe a good idea to have some kind of checksum and integrity check on the aesKEy object

// static constexpr uint32_t binaryKeyLength{4};        // 128 bits = 4 words of 32 bits
// uint32_t key[binaryKeyLength];
// static constexpr uint32_t nmbrExpandedKeys{10};
// uint32_t expandedKey[binaryKeyLength][nmbrExpandedKeys];
// uint32_t static constexpr roundConstant[nmbrExpandedKeys];