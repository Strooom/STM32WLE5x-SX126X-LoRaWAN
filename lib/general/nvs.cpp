// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"

void nonVolatileStorage::initialize() {
    addBlock(4);
    addBlock(16);
    addBlock(16);
    addBlock(4);
    addBlock(4);
}


uint16_t nonVolatileStorage::read16(uint32_t blockIndex) { return 0;}
uint32_t nonVolatileStorage::read32(uint32_t blockIndex) {return 0;}
uint64_t nonVolatileStorage::read64(uint32_t blockIndex) {return 0;}
void nonVolatileStorage::readAesKey(uint32_t blockIndex, aesKey &theAesKey) {}



void nonVolatileStorage::write(uint32_t blockIndex, uint16_t sourceData) {}
void nonVolatileStorage::write(uint32_t blockIndex, uint32_t sourceData) {}
void nonVolatileStorage::write(uint32_t blockIndex, uint64_t sourceData) {}
void nonVolatileStorage::write(uint32_t blockIndex, aesKey sourceData) {}

uint32_t nvsMap::addBlock(uint32_t length) {
    // TODO : check maximimum size of map 1K
    if ((numberOfBlocksUsed < maxNumberOfBlocks) && (nextStartAddress + length < maxSizeInBytes)) {
        uint32_t newBlockIndex                 = numberOfBlocksUsed;
        block[numberOfBlocksUsed].startAddress = nextStartAddress;
        block[numberOfBlocksUsed].length       = length;
        numberOfBlocksUsed++;
        nextStartAddress += length;
        return newBlockIndex;
    } else {
        return 0;
        // TODO : error - allocating too much NVS blocks
    }
}

uint32_t nonVolatileStorage::getNumberOfBlocksUsed() const {
    return theNvsMap.numberOfBlocksUsed;
}

uint32_t nonVolatileStorage::getNumberOfBytesUsed() const {
    return theNvsMap.nextStartAddress;
}

bool nonVolatileStorage::isValidBlockIndex(uint32_t blockIndex) const {
    return (blockIndex < theNvsMap.numberOfBlocksUsed);
}