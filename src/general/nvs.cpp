// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"

void nonVolatileStorage::read(uint32_t blockIndex, uint8_t* destinationDataBuffer) {
    if (blockIndex < theNvsMap.numberOfBlocksUsed) {
        uint32_t startAddress = theNvsMap.block[blockIndex].startAddress;
        uint32_t length       = theNvsMap.block[blockIndex].length;
        for (uint32_t i = 0; i < length; i++) {
            // destinationDataBuffer[i] = EEPROM.read(startAddress + i);
        }
    } else {
        // error : blockIndex out of range
    }
}

void nonVolatileStorage::write(uint32_t blockIndex, uint8_t* sourceDataBuffer) {
    if (blockIndex < theNvsMap.numberOfBlocksUsed) {
    } else {
    }
}

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
