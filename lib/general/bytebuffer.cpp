#include "bytebuffer.h"

void byteBuffer::clear() {
    length = 0;
}

void byteBuffer::append(macHeader& aMacHeader) {
    buffer[length] = aMacHeader.asByte();
    length += aMacHeader.length;
}

void byteBuffer::append(macPayload& aPayload) {
    //buffer[length] = aPayload.asByte();
    length += aPayload.length();
}

void byteBuffer::append(messageIntegrityCode& aMic) {
    buffer[length] = aMic[0];
    buffer[length] = aMic[1];
    buffer[length] = aMic[2];
    buffer[length] = aMic[3];
    length += aMic.length;

}

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

void byteBuffer::set(const char* newDataAsString) {
    uint32_t newDataLength = strnlen(newDataAsString, maxLength);
    if (newDataLength <= maxLength) {
        strncpy((char*)buffer, newDataAsString, newDataLength);
        length = newDataLength;
    } else {
        // TODO : error trying to send too much data
    }
}
