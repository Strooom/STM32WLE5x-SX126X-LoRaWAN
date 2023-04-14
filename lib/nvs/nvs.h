// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

// an nvsBlock is a region of non-volatile storage in external EEPROM
// each object which needs non-volatile storage gets an nvsBlock

class nvsBlock {
  public:
    const uint32_t startAddress;
    const uint32_t length;
};

class nvsMap {
    // friend class nonVolatileStorage;

  public:
    enum class blockIndex : uint32_t {
        nvsMapVersion = 0,
        unusedGeneral,

        measurementWriteIndex,
        oldestUnsentMeasurementIndex,
        oldestUnconfirmedMeasurementindex,
        unusedMeasurementDataManagement,

        DevEUI,
        DevAddr,
        uplinkFrameCounter,
        downlinkFrameCounter,
        applicationSessionKey,
        networkSessionKey,
        rx1Delay,
        unusedLoRaWAN,

        numberOfBlocks
    };

    // private:
    const nvsBlock blocks[static_cast<uint32_t>(blockIndex::numberOfBlocks)] = {
        {0, 1},          // nvsMapVersion : 1 byte
        {1, 127},        // unusedGeneral : extra blocks can be inserted hereafter for a maximum of 127 bytes

        {128, 4},          // measurementWriteIndex : 32 bits
        {132, 4},          // oldestUnsentMeasurementIndex : 32 bits
        {136, 4},          // oldestUnconfirmedMeasurementindex : 32 bits
        {140, 116},        // unusedMeasurementDataManagement : extra blocks can be inserted hereafter for a maximum of 116 bytes

        {256, 8},          // DevEUI : 64 bits
        {264, 4},          // DevAddr : 32 bits
        {268, 4},          // uplinkFrameCounter : 32 bits
        {272, 4},          // downlinkFrameCounter : 32 bits
        {276, 16},         // applicationSessionKey : 128 bits
        {292, 16},         // networkSessionKey : 128 bits
        {308, 1},          // rx1Delay : 1 byte
        {309, 203},        // unusedLoRaWAN : extra blocks can be inserted hereafter
    };
};

class nonVolatileStorage {
  public:
    bool isReady();                                                        // testing if the EEPROM is found correctly on the I2C bus
    bool isInitialized();                                                  // testing if the EEPROM is initialized or still has factory reset values of 0xFF
    void reset();                                                          // reset the EEPROM to meaningfull values at our very first boot
    void initialize();                                                     // initialize the nvsMap for subsequent boots
    void read(uint32_t blockIndex, uint8_t* destinationDataBuffer);        // reads the block from EEPROM and stores it in the destinationDataBuffer
    void write(uint32_t blockIndex, uint8_t* sourceDataBuffer);            // write data from sourceDataBuffer to the block in EEPROM

    static constexpr uint8_t i2cAddress{0x50};          // default I2C address of the first EEPROM, the second one has an address i2cAddress+1, set by its A0 pin being ties to VDD
    static constexpr uint8_t halTrials{0x03};           // ST HAL requires a 'retry' parameters
    static constexpr uint8_t halTimeout{0x10};          // ST HAL requires a 'timeout' in ms
    bool blockIndexIsValid(uint32_t blockIndex);        // checks if the blockIndex is valid

  private:
    nvsMap theNvsMap;
    uint32_t measurementWriteIndex{0};                    // measurement block index where we will write the next measurement
    uint32_t oldestUnsentMeasurementIndex{0};             // measurement block index of the oldest unconfirmed measurement
    uint32_t oldestUnconfirmedMeasurementindex{0};        // measurement block index of the oldest unsent measurement
    uint32_t nmbrMeasurementBlocks{12288};                // (120*1024)/100 = 12288
};
