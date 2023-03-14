#include "bytebuffer.h"
#include "hextools.h"
#include "string2.h"

void byteBuffer::clear() {
    length = 0;
}

// void byteBuffer::append(macHeader& aMacHeader) {
//     buffer[length] = aMacHeader.asByte();
//     length += aMacHeader.length;
// }

// void byteBuffer::append(macPayload& aPayload) {
//     // buffer[length] = aPayload.asByte();
//     length += aPayload.length();
// }

// void byteBuffer::append(messageIntegrityCode& aMic) {
//     buffer[length] = aMic[0];
//     buffer[length] = aMic[1];
//     buffer[length] = aMic[2];
//     buffer[length] = aMic[3];
//     length += aMic.length;
// }

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

void byteBuffer::setFromASCII(const char* asciiKey) {
    uint32_t asciiKeyLength = strnlen(asciiKey, 2 * maxLength);

    for (uint32_t index = 0; index < (asciiKeyLength / 2); index++) {
        uint8_t leftCharacter  = asciiKey[index * 2];
        uint8_t rightCharacter = asciiKey[(index * 2) + 1];
        leftCharacter          = toUpperCase(leftCharacter);
        rightCharacter         = toUpperCase(rightCharacter);
        if ((isHexCharacter(leftCharacter)) && (isHexCharacter(rightCharacter))) {
            buffer[index] = (valueFromHexCharacter(leftCharacter) << 4) + valueFromHexCharacter(rightCharacter);
        }
        length++;
    }
}
