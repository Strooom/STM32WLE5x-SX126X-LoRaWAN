// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"
#include "logging.h"

extern logging theLog;

bool nonVolatileStorage::blockIndexIsValid(uint32_t blockIndex) {
    return (blockIndex < static_cast<uint32_t>(nvsMap::blockIndex::numberOfBlocks));
}

bool nonVolatileStorage::isInitialized() {
    uint8_t nvsMapVersion;
    readBlock(static_cast<uint32_t>(nvsMap::blockIndex::nvsMapVersion), &nvsMapVersion);        // read the first block in NVS
    if (nvsMapVersion == 0xFF) {                                                                // if it's 0xFF, the EEPROM has not been initialized yet
        theLog.snprintf("EEPROM blank\n");                                                      //
    }
    theLog.snprintf("nvsMap version [%u]\n", nvsMapVersion);        //
    return (nvsMapVersion != 0xFF);                                 // if it's still 0xFF, the EEPROM has not been initialized yet
};

void nonVolatileStorage::initializeOnce() {
    uint8_t data[16]{0};

    data[0] = 0x01;
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::nvsMapVersion), data);        // set nvsMapVersion to 0x01

    data[0] = 0;
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::measurementWriteIndex), data);        // set all other to 0
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::oldestUnsentMeasurementIndex), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::oldestUnconfirmedMeasurementindex), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::unusedMeasurementDataManagement), data);

    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::DevEUI), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::DevAddr), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::uplinkFrameCounter), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::downlinkFrameCounter), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::applicationSessionKey), data);
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::networkSessionKey), data);

    data[0] = 0x01;
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::rx1Delay), data);        // this defaults to 1 second
}

void nonVolatileStorage::initialize() {
}

void nonVolatileStorage::readBlock(uint32_t blockIndex, uint8_t* destinationDataBuffer) {
    if (blockIndexIsValid(blockIndex)) {
        uint32_t startAddress = theNvsMap.blocks[blockIndex].startAddress;
        uint32_t length       = theNvsMap.blocks[blockIndex].length;
        read(startAddress, destinationDataBuffer, length);
    }
}

void nonVolatileStorage::writeBlock(uint32_t blockIndex, uint8_t* sourceDataBuffer) {
    if (blockIndexIsValid(blockIndex)) {
        uint32_t startAddress = theNvsMap.blocks[blockIndex].startAddress;
        uint32_t length       = theNvsMap.blocks[blockIndex].length;
        write(startAddress, sourceDataBuffer, length);
    }
}

void nonVolatileStorage::readMeasurement(uint32_t measurementIndex, measurement& destination) {
    measurementIndex      = measurementIndex % nmbrMeasurementBlocks;
    uint32_t startAddress = (measurementIndex * measurementBlockLength) + measurementsAddressOffset;
    uint8_t rawData[measurementBlockLength]{};
    read(startAddress, rawData, measurementBlockLength);
    destination.timestamp = (static_cast<uint32_t>(rawData[0]) << 24) | (static_cast<uint32_t>(rawData[1]) << 16) | (static_cast<uint32_t>(rawData[2]) << 8) | static_cast<uint32_t>(rawData[3]);
    destination.value     = (static_cast<uint32_t>(rawData[4]) << 24) | (static_cast<uint32_t>(rawData[5]) << 16) | (static_cast<uint32_t>(rawData[6]) << 8) | static_cast<uint32_t>(rawData[7]);
    destination.type      = static_cast<measurementChannel>(rawData[8]);
    destination.flags     = rawData[9];
}

void nonVolatileStorage::writeMeasurement(uint32_t measurementIndex, measurement& source) {
    measurementIndex      = measurementIndex % nmbrMeasurementBlocks;
    uint32_t startAddress = (measurementIndex * measurementBlockLength) + measurementsAddressOffset;
    uint8_t rawData[measurementBlockLength];
    rawData[0] = static_cast<uint8_t>((source.timestamp >> 24) & 0xFF);
    rawData[1] = static_cast<uint8_t>((source.timestamp >> 16) & 0xFF);
    rawData[2] = static_cast<uint8_t>((source.timestamp >> 8) & 0xFF);
    rawData[3] = static_cast<uint8_t>(source.timestamp & 0xFF);

    rawData[4] = static_cast<uint8_t>((source.value >> 24) & 0xFF);
    rawData[5] = static_cast<uint8_t>((source.value >> 16)) & 0xFF;
    rawData[6] = static_cast<uint8_t>((source.value >> 8) & 0xFF);
    rawData[7] = static_cast<uint8_t>(source.value & 0xFF);

    rawData[8] = static_cast<uint8_t>(source.type);
    rawData[9] = source.flags;
    write(startAddress, rawData, measurementBlockLength);
}

void nonVolatileStorage::writeMeasurement(measurement& source) {
    writeMeasurement(measurementWriteIndex, source);
    measurementWriteIndex = (measurementWriteIndex + 1) % nmbrMeasurementBlocks;
    // TODO : write measurementWriteIndex to NVS
    // writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::measurementWriteIndex), data);

}
