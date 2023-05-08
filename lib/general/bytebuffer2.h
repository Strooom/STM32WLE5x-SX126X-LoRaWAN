// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include <string.h>

// template class for a byte buffer.
// In LoRaWAN data is usually binary io null terminated strings, so I use this class to easily pass data around between application, Lorawan and the LoRa modem

template <uint32_t bufferLength>
class byteBuffer2 {
  public:
    static constexpr uint32_t length = bufferLength;

    byteBuffer2(){};

    void initialize() { level = 0; };
    bool isEmpty() const { return level == 0; };
    uint32_t getLevel() const { return level; };

    void shiftLeft(uint32_t shiftAmount) {
        if (shiftAmount < level) {
            (void)memcpy(theBuffer, theBuffer + shiftAmount, level - shiftAmount);
            level -= shiftAmount;
        } else {
            level = 0;
        }
    }

    void shiftRight(uint32_t shiftAmount) {
        if (level + shiftAmount <= bufferLength) {
            (void)memcpy(theBuffer + shiftAmount, theBuffer, level);
            level += shiftAmount;
        } else {
            level = bufferLength;
        }
    }

    void append(const uint8_t* newData, uint32_t newDataLength) {
        if (level + newDataLength <= bufferLength) {
            (void)memcpy(theBuffer + level, newData, newDataLength);
            level += newDataLength;
        } else {
            (void)memcpy(theBuffer + level, newData, bufferLength - level);
            level = bufferLength;
        }
    }

    void prefix(const uint8_t* newData, uint32_t newDataLength) {
        if (level + newDataLength <= bufferLength) {
            (void)memcpy(theBuffer + newDataLength, theBuffer, level);
            (void)memcpy(theBuffer, newData, newDataLength);
            level += newDataLength;
        }
    }

    void append(byteBuffer2& otherBuffer, uint32_t offset = 0) {
        if (offset + level + otherBuffer.level <= bufferLength) {
            (void)memcpy(theBuffer + level, otherBuffer.get(), otherBuffer.level);
            level += otherBuffer.level;        // TODO : get correct level after this manipulation
        }
    }

    void set(const uint8_t* newData, uint32_t newDataLength, uint32_t offset = 0) {
        if (offset + newDataLength <= bufferLength) {
            (void)memcpy(theBuffer + offset, newData, newDataLength);
        }
    }

    const uint8_t* get(uint32_t offset = 0) const { return (theBuffer + offset); }

#ifndef unitTesting
//  private:
#endif
    uint32_t level{0};
    uint8_t theBuffer[bufferLength]{};
};
