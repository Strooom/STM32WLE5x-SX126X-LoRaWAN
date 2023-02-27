// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "linkdirection.h"

class frameControl {
  public:
    frameControl(linkDirection theLinkDirection);
    void set(bool ADR, bool ADRACKReq, bool ACK, bool ClassB, uint8_t FOptsLen);
    uint8_t asByte() const;                     // return the frameControl as a byte
    void fromByte(uint8_t theByte);             // decode the frameControl from a byte
    static constexpr uint32_t length{1};        // [bytes]
  private:
    linkDirection theLinkDirection;
    bool ADR;                // bit [7]
    bool ADRACKReq;          // bit [6]
    bool ACK;                // bit [5]
    bool ClassB;             // bit [4]
    bool FPending;           // bit [4]
    uint8_t FOptsLen;        // bits [3:0]
};