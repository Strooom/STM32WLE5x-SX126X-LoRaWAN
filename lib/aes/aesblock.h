// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "aeskey.h"

class aesBlock {
  public:
    void set(uint8_t bytes[16]);        // load the block with data
    void set(uint32_t words[4]);        // load the block with data
    void encrypt(aesKey &key);          // encrypt the block
    uint8_t *getAsBytes();              // return the encrypted block as bytes
    uint8_t *getAsWords();              // return the encrypted block as words

#ifndef unitTesting
  private:
#endif
    union {
        uint8_t asByte[16];        //
        uint32_t asWord[4];        //
    } state;                       // fancy name for data in the block
};