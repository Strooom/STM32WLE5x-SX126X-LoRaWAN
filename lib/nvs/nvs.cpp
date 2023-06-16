// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"
#include "logging.h"

bool nonVolatileStorage::blockIndexIsValid(uint32_t theBlockIndex) {
    return (theBlockIndex < static_cast<uint32_t>(nvsMap::blockIndex::numberOfBlocks));
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
    // writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::unusedMeasurementDataManagement), data);

    logging::snprintf("Write Default / Initial LoRaWAN Settings & State...\n");        //
    writeBlock32(static_cast<uint32_t>(nvsMap::blockIndex::DevAddr), 0x27FE980D);

    uint8_t tmpKey1[] = {0x14, 0x22, 0xE5, 0x99, 0x1B, 0xF6, 0x60, 0x58, 0x02, 0x99, 0xA7, 0xE3, 0x2F, 0xFE, 0x13, 0xE5};
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::applicationSessionKey), tmpKey1);

    uint8_t tmpKey2[] = {0x5C, 0xB6, 0xF7, 0x99, 0x26, 0x6E, 0x02, 0x84, 0xAB, 0xC4, 0x95, 0x39, 0x01, 0x18, 0x07, 0xFE};
    writeBlock(static_cast<uint32_t>(nvsMap::blockIndex::networkSessionKey), tmpKey2);

    writeBlock32(static_cast<uint32_t>(nvsMap::blockIndex::uplinkFrameCounter), 0U);
    writeBlock32(static_cast<uint32_t>(nvsMap::blockIndex::downlinkFrameCounter), 0U);
    writeBlock8(static_cast<uint32_t>(nvsMap::blockIndex::rx1Delay), 1U);
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
    // TODO : make this simpler with a union
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
    // TODO : make this simpler with a union
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
    destination.timestamp.asBytes[0] = rawData[0];
    destination.timestamp.asBytes[1] = rawData[1];
    destination.timestamp.asBytes[2] = rawData[2];
    destination.timestamp.asBytes[3] = rawData[3];
    destination.value.asBytes[0]     = rawData[4];
    destination.value.asBytes[1]     = rawData[5];
    destination.value.asBytes[2]     = rawData[6];
    destination.value.asBytes[3]     = rawData[7];

    destination.type  = static_cast<measurementChannel>(rawData[8]);
    destination.flags = rawData[9];
}

void nonVolatileStorage::writeMeasurement(uint32_t measurementIndex, measurement& source) {
    measurementIndex      = measurementIndex % nmbrMeasurementBlocks;
    uint32_t startAddress = (measurementIndex * measurementBlockLength) + measurementsAddressOffset;
    uint8_t rawData[measurementBlockLength];

    rawData[0] = source.timestamp.asBytes[0];
    rawData[1] = source.timestamp.asBytes[1];
    rawData[2] = source.timestamp.asBytes[2];
    rawData[3] = source.timestamp.asBytes[3];

    rawData[4] = source.value.asBytes[0];
    rawData[5] = source.value.asBytes[1];
    rawData[6] = source.value.asBytes[2];
    rawData[7] = source.value.asBytes[3];

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
    HAL_Delay(writeCycleTime);                                         // the EEPROM needs 3.5 ms to internally write the data, if WriteProtect goes HIGH too early, the data is not written
    HAL_GPIO_WritePin(GPIOB, writeProtect_Pin, GPIO_PIN_SET);          // disable write
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
