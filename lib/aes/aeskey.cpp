
#include "aeskey.h"
#include "hextools.h"

aesKey::aesKey() {
    for (uint32_t index = 0; index < binaryKeyLength; index++) {        //
        key[index] = 0;                                                 // initialize the key to all zero bytes
    }
    for (uint32_t index = 0; index < asciiKeyLength - 1; index++) {        //
        keyAsASCII[index] = '0';                                           // initialize the key to all (ASCII) zero characters
    }
    keyAsASCII[asciiKeyLength - 1] = 0;        // terminate the string
}

bool aesKey::setFromASCII(const char* asciiKey) {
    // TODO : to be safe, check the strnlen of what we receive

    for (uint32_t index = 0; index < binaryKeyLength; index++) {
        uint8_t leftCharacter  = asciiKey[index * 2];
        uint8_t rightCharacter = asciiKey[(index * 2) + 1];
        leftCharacter          = toUpperCase(leftCharacter);
        rightCharacter         = toUpperCase(rightCharacter);
        if ((isHexCharacter(leftCharacter)) && (isHexCharacter(rightCharacter))) {
            key[index]                  = (valueFromHexCharacter(leftCharacter) << 4) + valueFromHexCharacter(rightCharacter);
            keyAsASCII[index * 2]       = leftCharacter;
            keyAsASCII[(index * 2) + 1] = rightCharacter;
        } else {
            return false;        // any non-hex character is an error, and makes us quit the conversion
        }
    }
    keyAsASCII[asciiKeyLength - 1] = 0x00;        // terminate the string
    return true;
}

const uint8_t* aesKey::asASCII() const {
    return keyAsASCII;
}

const uint8_t* aesKey::asBinary() const {
    return key;
}

unsigned char* aesKey::asUnsignedChar() {
    return (unsigned char*) key;
}

   

