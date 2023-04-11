#include "block.h"
#include "hextools.h"

void block::toString(char* destinationString) const {
    binaryArrayToHexString(rawData, block::length, destinationString);
}

void block::setFromBinaryData(const uint8_t* binarySourceData) {
    for (uint32_t index = 0; index < length; index++) {
        rawData[index] = binarySourceData[index];
    }
}

void block::setFromHexString(const char* hexStringSourceData) {
    hexStringToBinaryArray(hexStringSourceData, rawData);
}

void block::xorBlock(block secondBlock) {
    for (uint32_t index = 0; index < length; index++) {
        rawData[index] = rawData[index] ^ secondBlock.rawData[index];
    }
}

void block::shiftLeft() {
    uint8_t Overflow{0};

    for (uint32_t index = 0; index < length; index++) {
        if (index < 15) {                                     // Check for overflow on next byte except for the last byte
            if ((rawData[index + 1] & 0x80) == 0x80) {        // Check if upper bit is one
                Overflow = 1;
            } else {
                Overflow = 0;
            }
        } else {
            Overflow = 0;
        }
        rawData[index] = (rawData[index] << 1) + Overflow;        // Shift one left
    }
}