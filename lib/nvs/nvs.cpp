// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"
#include "logging.h"

bool nonVolatileStorage::blockIndexIsValid(uint32_t blockIndex) {
    return (blockIndex < static_cast<uint32_t>(nvsMap::blockIndex::numberOfBlocks));
}

bool nonVolatileStorage::isInitialized() {
    return (0xFF != readBlock8(static_cast<uint32_t>(nvsMap::blockIndex::nvsMapVersion)));
}

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

uint32_t nonVolatileStorage::readBlock32(uint32_t blockIndex) {
    uint8_t dataAsBytes[4];
    uint32_t result;
    readBlock(blockIndex, dataAsBytes);
    result = (static_cast<uint32_t>(dataAsBytes[0]) << 24) | (static_cast<uint32_t>(dataAsBytes[1]) << 16) | (static_cast<uint32_t>(dataAsBytes[2]) << 8) | static_cast<uint32_t>(dataAsBytes[3]);
    return result;
}

void nonVolatileStorage::writeBlock8(uint32_t blockIndex, uint8_t sourceData) {
    writeBlock(blockIndex, &sourceData);
}

uint8_t nonVolatileStorage::readBlock8(uint32_t blockIndex) {
    uint8_t result;
    readBlock(blockIndex, &result);
    return result;
}

void nonVolatileStorage::writeBlock32(uint32_t blockIndex, uint32_t sourceData) {
    uint8_t dataAsBytes[4];
    dataAsBytes[0] = static_cast<uint8_t>((sourceData & 0xFF000000) >> 24);
    dataAsBytes[1] = static_cast<uint8_t>((sourceData & 0x00FF0000) >> 16);
    dataAsBytes[2] = static_cast<uint8_t>((sourceData & 0x0000FF00) >> 8);
    dataAsBytes[3] = static_cast<uint8_t>((sourceData & 0x000000FF));
    writeBlock(blockIndex, dataAsBytes);
}

void nonVolatileStorage::readMeasurement(uint32_t measurementIndex, measurement& destination) {
    measurementIndex      = measurementIndex % nmbrMeasurementBlocks;
    uint32_t startAddress = (measurementIndex * measurementBlockLength) + measurementsAddressOffset;
    uint8_t rawData[measurementBlockLength]{};
    read(startAddress, rawData, measurementBlockLength);
    destination.timestampAsBytes[0] = rawData[0];
    destination.timestampAsBytes[1] = rawData[1];
    destination.timestampAsBytes[2] = rawData[2];
    destination.timestampAsBytes[3] = rawData[3];
    destination.valueAsBytes[0]     = rawData[4];
    destination.valueAsBytes[1]     = rawData[5];
    destination.valueAsBytes[2]     = rawData[6];
    destination.valueAsBytes[3]     = rawData[7];

    // destination.timestamp = (static_cast<uint32_t>(rawData[0]) << 24) | (static_cast<uint32_t>(rawData[1]) << 16) | (static_cast<uint32_t>(rawData[2]) << 8) | static_cast<uint32_t>(rawData[3]);
    // destination.valueAsFloat     = (static_cast<uint32_t>(rawData[4]) << 24) | (static_cast<uint32_t>(rawData[5]) << 16) | (static_cast<uint32_t>(rawData[6]) << 8) | static_cast<uint32_t>(rawData[7]);

    destination.type  = static_cast<measurementChannel>(rawData[8]);
    destination.flags = rawData[9];
}

void nonVolatileStorage::writeMeasurement(uint32_t measurementIndex, measurement& source) {
    measurementIndex      = measurementIndex % nmbrMeasurementBlocks;
    uint32_t startAddress = (measurementIndex * measurementBlockLength) + measurementsAddressOffset;
    uint8_t rawData[measurementBlockLength];

    rawData[0] = source.timestampAsBytes[0];
    rawData[1] = source.timestampAsBytes[1];
    rawData[2] = source.timestampAsBytes[2];
    rawData[3] = source.timestampAsBytes[3];

    rawData[4] = source.valueAsBytes[0];
    rawData[5] = source.valueAsBytes[1];
    rawData[6] = source.valueAsBytes[2];
    rawData[7] = source.valueAsBytes[3];

    // rawData[0] = static_cast<uint8_t>((source.timestamp >> 24) & 0xFF);
    // rawData[1] = static_cast<uint8_t>((source.timestamp >> 16) & 0xFF);
    // rawData[2] = static_cast<uint8_t>((source.timestamp >> 8) & 0xFF);
    // rawData[3] = static_cast<uint8_t>(source.timestamp & 0xFF);

    // rawData[4] = static_cast<uint8_t>((source.valueAsFloat >> 24) & 0xFF);
    // rawData[5] = static_cast<uint8_t>((source.valueAsFloat >> 16)) & 0xFF;
    // rawData[6] = static_cast<uint8_t>((source.valueAsFloat >> 8) & 0xFF);
    // rawData[7] = static_cast<uint8_t>(source.valueAsFloat & 0xFF);

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

#ifndef environment_desktop

#include "main.h"

extern I2C_HandleTypeDef hi2c2;

bool nonVolatileStorage::isReady() {
    if (HAL_OK != HAL_I2C_IsDeviceReady(&hi2c2, i2cAddress << 1, halTrials, halTimeout)) {        // testing presence of the first bank of 64K (U7)
        return false;
    }
    if (HAL_OK != HAL_I2C_IsDeviceReady(&hi2c2, (i2cAddress << 1) + 1, halTrials, halTimeout)) {        // testing presence of the second bank of 64K (U8)
        return false;
    }
    return true;
}

void nonVolatileStorage::read(uint32_t startAddress, uint8_t* data, uint32_t dataLength) {
    HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, startAddress, I2C_MEMADD_SIZE_16BIT, data, dataLength, halTimeout);        //
}
void nonVolatileStorage::write(uint32_t startAddress, uint8_t* data, uint32_t dataLength) {
    HAL_GPIO_WritePin(GPIOB, writeProtect_Pin, GPIO_PIN_RESET);        // Drive writeProtect LOW = enable write
    HAL_I2C_Mem_Write(&hi2c2, i2cAddress << 1, startAddress, I2C_MEMADD_SIZE_16BIT, data, dataLength, halTimeout);
    HAL_Delay(writeCycleTime);                                       // the EEPROM needs 3.5 ms to internally write the data, if WriteProtect goes HIGH too early, the data is not written
    HAL_GPIO_WritePin(GPIOB, writeProtect_Pin, GPIO_PIN_SET);        // disable write
}

#else

#include <cstring>

static uint8_t memory[60 * 1024];

bool nonVolatileStorage::isReady() {
    return true;
}

void nonVolatileStorage::read(uint32_t startAddress, uint8_t* data, uint32_t dataLength) {
    memcpy(data, memory + startAddress, dataLength);
}
void nonVolatileStorage::write(uint32_t startAddress, uint8_t* data, uint32_t dataLength) {
    memcpy(memory + startAddress, data, dataLength);
}

// TODO : a desktop console version goes here

#endif
