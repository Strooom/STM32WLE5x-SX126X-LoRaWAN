
#include "aeskey.h"

aesKey::aesKey() {
    for (uint32_t index = 0; index < binaryKeyLength; index++) {
        key[index] = 0;
    }
    for (uint32_t index = 0; index < asciiKeyLength; index++) {
        keyAsASCII[index] = 0;
    }
}

bool aesKey::fromASCII(const char* asciiKey) {
    // TODO : iterate over the string, take two characters at a time, convert to a byte, store in the key
    uint8_t leftCharacter{0};
    uint8_t rightCharacter{0};
    leftCharacter  = toUpperCase(leftCharacter);
    rightCharacter = toUpperCase(rightCharacter);
    if ((isHexCharacter(leftCharacter)) && (isHexCharacter(rightCharacter))) {
        uint8_t hexValue = (valueFromHexCharacter(leftCharacter) << 4) + valueFromHexCharacter(rightCharacter);
    }
    return false;
}

const uint8_t* aesKey::toASCII() {
    uint8_t leftCharacter{0};
    uint8_t rightCharacter{0};
    uint8_t aValue{0};

    // TODO : iterate over the key, convert each byte to two characters, store in the string
    leftCharacter  = hexCharacterFromValue((aValue >> 4) & 0x0F);
    rightCharacter = hexCharacterFromValue(aValue & 0x0F);
}

uint8_t aesKey::toUpperCase(uint8_t aCharacter) {
    if (aCharacter >= 'a' && aCharacter <= 'z') {
        aCharacter = aCharacter - 'a' + 'A';
    }
    return aCharacter;
}

bool aesKey::isHexCharacter(uint8_t aCharacter) {
    return ((aCharacter >= 'A' && aCharacter <= 'F') || (aCharacter >= '0' && aCharacter <= '9'));
}

uint8_t aesKey::valueFromHexCharacter(uint8_t aCharacter) {
    if (aCharacter >= '0' && aCharacter <= '9') {
        return aCharacter - '0';
    }
    if (aCharacter >= 'A' && aCharacter <= 'F') {
        return aCharacter - 'A' + 10;
    }
}

uint8_t aesKey::hexCharacterFromValue(uint8_t aValue) {
    if (aValue <= 9) {
        return aValue + '0';
    } else if (aValue <= 15) {
        return aValue - 10 + 'A';
    } else
        return '?';
}
