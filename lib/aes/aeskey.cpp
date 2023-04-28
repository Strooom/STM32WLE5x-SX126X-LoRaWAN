
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

void aesKey::setFromASCII(const char* asciiKey) {
    for (uint32_t index = 0; index < binaryKeyLength; index++) {
        uint8_t leftCharacter  = asciiKey[index * 2];
        uint8_t rightCharacter = asciiKey[(index * 2) + 1];
        leftCharacter          = toUpperCase(leftCharacter);
        rightCharacter         = toUpperCase(rightCharacter);
        if ((isHexCharacter(leftCharacter)) && (isHexCharacter(rightCharacter))) {
            key[index]                  = (valueFromHexCharacter(leftCharacter) << 4) + valueFromHexCharacter(rightCharacter);
            keyAsASCII[index * 2]       = leftCharacter;
            keyAsASCII[(index * 2) + 1] = rightCharacter;
        }
    }
    keyAsASCII[asciiKeyLength - 1] = 0x00;        // terminate the string
}

void aesKey::setFromBinary(const uint8_t* binaryKey) {
    for (uint32_t index = 0; index < binaryKeyLength; index++) {
        key[index] = binaryKey[index];
    }
    binaryArrayToHexString(key, binaryKeyLength, (char*)keyAsASCII);
}

 uint8_t* aesKey::asASCII()  {
    return keyAsASCII;
}

 uint8_t* aesKey::asBinary()  {
    return key;
}

unsigned char* aesKey::asUnsignedChar() {
    return (unsigned char*)key;
}
