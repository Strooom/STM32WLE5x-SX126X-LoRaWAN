// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"

bool nonVolatileStorage::blockIndexIsValid(uint32_t blockIndex) {
    return (blockIndex < static_cast<uint32_t>(nvsMap::blockIndex::numberOfBlocks));
}

bool nonVolatileStorage::isInitialized() {
    uint8_t nvsMapVersion;
    read(static_cast<uint32_t>(nvsMap::blockIndex::nvsMapVersion), &nvsMapVersion);        // read the first block in NVS
    return (nvsMapVersion != 0xFF);                                                        // if it's still 0xFF, the EEPROM has not been initialized yet
};

void nonVolatileStorage::reset() {
    uint8_t data[16]{0};

    data[0] = 0x01;
    write(static_cast<uint32_t>(nvsMap::blockIndex::nvsMapVersion), data);        // set nvsMapVersion to 0x01

    data[0] = 0;
    write(static_cast<uint32_t>(nvsMap::blockIndex::measurementWriteIndex), data);        // set all other to 0
    write(static_cast<uint32_t>(nvsMap::blockIndex::oldestUnsentMeasurementIndex), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::oldestUnconfirmedMeasurementindex), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::unusedMeasurementDataManagement), data);

    write(static_cast<uint32_t>(nvsMap::blockIndex::DevEUI), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::DevAddr), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::uplinkFrameCounter), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::downlinkFrameCounter), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::applicationSessionKey), data);
    write(static_cast<uint32_t>(nvsMap::blockIndex::networkSessionKey), data);

    data[0] = 0x01;
    write(static_cast<uint32_t>(nvsMap::blockIndex::rx1Delay), data);        // this defaults to 1 second
}

void nonVolatileStorage::initialize() {
}
