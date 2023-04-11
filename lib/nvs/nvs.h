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

// Collection of all nvsBlocks

class nvsMap {
  public:
    static constexpr uint32_t numberOfBlocks{7};

  private:
    friend class nonVolatileStorage;
    nvsBlock block[numberOfBlocks] = {
        // Note : if you change this list, then the new firmware will not be able to read the old non-volatile storage
        {0, 4},         // uplinkFrameCounter : 32 bits
        {4, 4},         // downlinkFrameCounter : 32 bits
        {8, 2},         // joinDevNonce : 16 bits
        {10, 8},        // applicationSessionKey : 128 bits
        {18, 8},        // networkSessionKey : 128 bits
        {26, 4},        // joinEUI : 64 bits
        {30, 8},        // AppKey  : 128 bits

        // Required Persistent Values for ABP Devcies In the event of a reset or loss of power, the following values must be persisted so that an ABP end device can return to the network connection session where it left off before the loss of power:
        // DevEUI
        // DevAddr
        // SessionKeys
        // Frame Counters (both uplink and downlink)
        // Channel List (frequencies and enabled channels)
        // Data rate
        // TXPower
        // NbTrans
        // MaxDutyCycle
        // RX2Frequency
        // RX1DROffset
        // RX2DataRate
        // RXTimingDelay
        // MaxEIRP
        // DownlinkDwellTime
        // UplinkDwellTime

        // Note for OTAA there is some delta
        // not stored : DevAddr & Session Keys
        // To be stored :
        // JoinEUI
        // Root Keys AppKey / NwkKey
        // JoinNonce

    };
    bool hasOverlap();        // checks all blocks to ensure there is no overlap
};

class nonVolatileStorage {
  public:
    bool isReady();                                                        // testing if the EEPROM is found correctly on the I2C bus
    void read(uint32_t blockIndex, uint8_t* destinationDataBuffer);        // reads the block from EEPROM and stores it in the destinationDataBuffer
    void write(uint32_t blockIndex, uint8_t* sourceDataBuffer);            // write data from sourceDataBuffer to the block in EEPROM

    static constexpr uint8_t i2cAddress{0x50};        // default I2C address of the first EEPROM, the second one has an address i2cAddress+1, set by its A0 pin being ties to VDD
    static constexpr uint8_t halTrials{0x03};         // ST HAL requires a 'retry' parameters
    static constexpr uint8_t halTimeout{0x10};        // ST HAL requires a 'timeout' in ms

  private:
    nvsMap theNvsMap;
    bool blockIndexIsValid(uint32_t blockIndex);        // checks if the blockIndex is valid
};
