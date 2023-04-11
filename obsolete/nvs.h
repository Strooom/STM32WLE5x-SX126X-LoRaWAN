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

    // DevEUI : this we can get from the device UID64 - hardcoded in ROM
    // DevAddr [4 bytes], genereated by the network server / TTN
    // SessionKeys : NwkSKey and AppSKey : both keys are 128 bits = 16 bytes
    // Frame Counters (both uplink and downlink)  : both are 32 bit counters

    // Channel List (frequencies and enabled channels)
    // Data rate
    // TXPower : FTTB we just transmit at 14 dBm / 25 mW
    // NbTrans : TODO
    // MaxDutyCycle : ideally we should store this in NVS, so after a reset we can continue where we left off, but as resets are quite rare, I postpone this for now

    // RX2Frequency : for TTN this is currently fixed at 869.525 MHz, so we can hardcode it..
    // RX1DROffset :
    // RX2DataRate : fixed at SF9 for TTN
    // RXTimingDelay : 1 second up to 15 seconds - I think it can only be changed with OTAA Join, which we don't do FTTB

    // MaxEIRP ?? What is the difference with TxPower ?
    // DownlinkDwellTime / not for EU region
    // UplinkDwellTime / not for EU region

    // Note for OTAA there is some delta
    // not stored : DevAddr & Session Keys
    // To be stored :
    // JoinEUI
    // Root Keys AppKey / NwkKey
    // JoinNonce
};

class nonVolatileStorage {
  public:
    void initialize();
    bool isReady();

    uint8_t read8(uint32_t blockIndex);
    uint16_t read16(uint32_t blockIndex);
    uint32_t read32(uint32_t blockIndex);
    uint64_t read64(uint32_t blockIndex);
    void readAesKey(uint32_t blockIndex, aesKey &theAesKey);

    void write(uint32_t blockIndex, uint8_t sourceData);
    void write(uint32_t blockIndex, uint16_t sourceData);
    void write(uint32_t blockIndex, uint32_t sourceData);
    void write(uint32_t blockIndex, uint64_t sourceData);
    void write(uint32_t blockIndex, aesKey sourceData);

    uint32_t addBlock(uint32_t length);        // adds a block with size [length] bytes to the nvsMap, returns the blockIndex
    uint32_t getNumberOfBlocksUsed() const;
    uint32_t getNumberOfBytesUsed() const;

    const uint32_t deviceAddressBlockIndex{addBlock(4)};
    const uint32_t networkSessionKeyBlockIndex{addBlock(16)};
    const uint32_t applicationSessionKeyBlockIndex{addBlock(16)};
    const uint32_t uplinkFrameCounterBlockIndex{addBlock(4)};
    const uint32_t downlinkFrameCounterBlockIndex{addBlock(4)};

  private:
    nvsMap theNvsMap;
    bool isValidBlockIndex(uint32_t blockIndex) const;
};
