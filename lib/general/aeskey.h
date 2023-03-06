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
    bool fromASCII(const char* asciiKey);        // initialize the key from an ASCII string, returns true if successful
    const uint8_t* toASCII();                    // returns a pointer to the key as an ASCII string
  private:
    uint8_t key[16];                                                 // 128 bits = 16 bytes
    uint8_t keyAsASCII[33];                                          // 16 bytes * 2 characters per byte + 1 for the terminating zero
    static uint8_t toUpperCase(uint8_t aCharacter);                  // convert a character to upper case
    static bool isHexCharacter(uint8_t aCharacter);                  // check if a character is a valid hex character, ie. 0-9 or A-F - assuming upper case
    static uint8_t valueFromHexCharacter(uint8_t aCharacter);        // convert a hex character to a value, ie. 'A' -> 10
    static uint8_t hexCharacterFromValue(uint8_t aValue);            // convert a value to a hex character, ie. 10 -> 'A'
};
