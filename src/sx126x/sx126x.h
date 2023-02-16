// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

// Notes :
// This driver intends to drive the SX126X chip from Semtech, embedded in the STM32WLE from ST
// As the (current) scope for my application is only LoRa in the EU868 region, all functions outside this scope are not implemented (FSK, long packets, etc..)
// As the SX126X is embedded inside the STM32WEL, some of the IOs are 'connected' to registers in the STM32WLE
// * RESET
// * BUSY
// * interrupt lines..

#pragma once
#include <stdint.h>
#include "stm32wle_peripheral.h"
#include "sx126xregisters.h"
#include "sx126xstate.h"
#include "sx126xerrors.h"
#include "sx126xcommands.h"
#include "sx126xstandbymodes.h"
#include "sx126packettypes.h"

#include "../lorawan/bandwidth.h"
#include "../lorawan/spreadingfactor.h"
#include "../lorawan/codingrate.h"

#define RCC_CSR (*(volatile uint32_t *)0x5800 0094)         // RCC control/status register - datasheet RM0461 page 288
#define RFRST 0x01 << 15;                                   // RF reset bit in RCC_CSR
#define RCC_CFGR (*(volatile uint32_t *)0x5800 0008)        // RCC clock configuration register - datasheet RM0461 page 254

#define SX126X_SPI_CR1 (*(volatile uint32_t *)0x5801 0000)        // SPI control register 1 - datasheet RM0461 page 1197
#define SX126X_SPI_CR2 (*(volatile uint32_t *)0x5801 0004)        // SPI control register 2 - datasheet RM0461 page 1199
#define SX126X_SPI_SR (*(volatile uint32_t *)0x5801 0008)         // SPI status register - datasheet RM0461 page 1201
#define SX126X_SPI_DR (*(volatile uint32_t *)0x5801 000C)         // SPI data register - datasheet RM0461 page 1203

#define PWR_SR2 (*(volatile uint32_t *)0x5801 000C)        // Power status register 2 - datasheet RM0461 page 219
#define RFBUSYS 0x01 << 1;                                 // Radio busy signal status

class sx126x : stm32wlePeripheral {
  public:
    void reset();
    void initialize();

    bool detectChannelActivity();
    void prepareForSleep();
    void restoreAfterSleep();
    bool isInitialized();

    void setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth, codingRate theCodingRate);
    void setPacketParameters(uint8_t payloadLength);
    void setRfFrequency(uint32_t frequency);
    void setTxParameters(int8_t transmitPowerdBm);

    // void setTxParameters(uint8_t paSelect, int8_t power, RadioRampTimes_t rampTime);

    // void setRxConfig(RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint32_t bandwidthAfc, uint16_t preambleLen, uint16_t symbTimeout, bool fixLen, uint8_t payloadLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod, bool iqInverted, bool rxContinuous);
    // void setTxConfig(RadioModems_t modem, int8_t power, uint32_t fdev, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod, bool iqInverted, uint32_t timeout);
    sx126xState getState() const;
    // uint32_t getTimeOnAir(RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, uint8_t payloadLen, bool crcOn);

    void goSleep();
    void goStandby(sx126xStandbyMode aStandbyMode);
    bool isStandby();

    void writeRegister(sx126xRegister theRegister, uint32_t newValue);
    uint32_t readRegister(sx126xRegister theRegister);

    void writeBuffer(sx126xRegister theRegister, uint32_t newValue);        // accesses the payload data buffer
    uint32_t readBuffer(sx126xRegister theRegister);                        // accesses the payload data buffer

    //    void writeMultipleRegisters(sx126xRegister theRegister, uint32_t newValue); // Dont know if they will be useful, looks like we barely need to write registers, and probably never a RANGE of adjacent registers
    //    uint32_t readMultipleRegisters(sx126xRegister theRegister);

    uint32_t getWakeupTime();        // in [ms]

    static constexpr uint32_t rxTxBufferLength{256};              // length of the RAM buffer inside the SX126X // TODO : ST firmware mentions 255 io 256.. Why ?k??
    static constexpr uint32_t crystalFrequency{32000000U};        // frequency of the TCXO
    static constexpr uint32_t maxWaitCounter{10000U};             // nmbr of loops in waitOnBusy

    sx126xError getLastError();        // reads and clears the last error that occured

  private:
    void setPacketType(packetType thePacketType);

    void executeCommand(sx126xCommand opcode, uint8_t *parametersIn, uint8_t *dataOut, uint8_t nmbrExtraBytes, bool waitOnBusy = false);

    sx126xError lastError{sx126xError::none};        // remembers the last error that occured
    sx126xState theState{sx126xState::boot};
    sx126xState getStateFromHW();
    bool isBusy();
    void waitOnBusy();
    void setRegulatorMode();
    void setPowerAmplifierConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut);

    void writeSPI();
    uint8_t readSPI();
};

// void SUBGRF_SetPayload(uint8_t *payload, uint8_t size) {
//     SUBGRF_WriteBuffer(0x00, payload, size);
// }

// void SUBGRF_SetTx(uint32_t timeout) {
//     uint8_t buf[3];

//     OperatingMode = MODE_TX;

//     buf[0] = (uint8_t)((timeout >> 16) & 0xFF);
//     buf[1] = (uint8_t)((timeout >> 8) & 0xFF);
//     buf[2] = (uint8_t)(timeout & 0xFF);
//     SUBGRF_WriteCommand(RADIO_SET_TX, buf, 3);
// }

// void SUBGRF_SendPayload(uint8_t *payload, uint8_t size, uint32_t timeout) {
//     SUBGRF_SetPayload(payload, size);
//     SUBGRF_SetTx(timeout);
// }