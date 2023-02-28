// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "aeskey.h"

// an nvsBlock is a region of non-volatile storage in external EEPROM
// this region is defined by a start address and a length
// each object which needs non-volatile storage gets an nvsBlock, so it can read and write its own data to NVS if needed

class nvsBlock {
  public:
    uint32_t startAddress;
    uint32_t length;
};

// Collection of all nvsBlocks

class nvsMap {
  public:
    static constexpr uint32_t maxSizeInBytes{1024};         // 1K is reserved for storing application settings. All the rest is reserved for measurement data
    static constexpr uint32_t maxNumberOfBlocks{64};        //
    uint32_t addBlock(uint32_t length);                     // adds a block to the nvsMap, returns the blockIndex

  private:
    friend class nonVolatileStorage;
    nvsBlock block[maxNumberOfBlocks];

    uint32_t numberOfBlocksUsed{0};
    uint32_t nextStartAddress{0};

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

class nonVolatileStorage {
  public:
    bool isReady();

    uint8_t read8(uint32_t blockIndex);
    uint16_t read16(uint32_t blockIndex);
    uint32_t read32(uint32_t blockIndex);
    uint64_t read64(uint32_t blockIndex);
    aesKey readAesKey(uint32_t blockIndex);

    void write(uint32_t blockIndex, uint8_t sourceData);
    void write(uint32_t blockIndex, uint16_t sourceData);
    void write(uint32_t blockIndex, uint32_t sourceData);
    void write(uint32_t blockIndex, uint64_t sourceData);
    void write(uint32_t blockIndex, aesKey sourceData);

    uint32_t addBlock(uint32_t length);        // adds a block to the nvsMap, returns the blockIndex
    uint32_t getNumberOfBlocksUsed() const;
    uint32_t getNumberOfBytesUsed() const;

  private:
    nvsMap theNvsMap;
    bool isValidBlockIndex(uint32_t blockIndex) const;
};
