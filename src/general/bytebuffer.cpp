#include "bytebuffer.h"

void byteBuffer::set(const uint8_t* newData, uint32_t newDataLength) {
    if (newDataLength <= maxLength) {
        for (uint32_t byteIndex = 0; byteIndex < newDataLength; byteIndex++) {
            buffer[byteIndex] = newData[byteIndex];
        }
        length = newDataLength;
    } else {
        // TODO : error trying to send too much data
    }
}