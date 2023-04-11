// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

// Notes :
// This driver intends to drive the SX126X chip from Semtech, embedded in the STM32WLE from ST
// As the (current) scope for my application is only LoRa in the EU868 region, all functions outside this scope are not implemented (FSK, long packets, etc..)
// As the SX126X is embedded inside the STM32WLE5, some of the IOs are 'connected' to registers in the STM32WLE
// * RESET
// * BUSY
// * interrupt lines..

#pragma once
#include <stdint.h>


#include "bandwidth.h"
#include "bytebuffer.h"

#include "codingrate.h"
#include "crctype.h"
#include "headertype.h"
#include "invertiq.h"
#include "lowdatarateoptimize.h"
#include "spreadingfactor.h"
#include "sx126packettypes.h"
#include "sx126xcommands.h"
#include "sx126xerrors.h"
#include "sx126xramptime.h"
#include "sx126xregisters.h"
#include "sx126xparameters.h"
#include "sx126xstate.h"
#include "rfswitch.h"

class sx126x {
  public:
    void initialize();
    void reset();                           // reset the SX126 from its HW reset pin
    void initializeRadioInterface();        // configures the SPI between the MCU and the SX126x
    void initializeRadio();                 //
    void initializeModem();                 //

    void goSleep(sleepMode aSleepMode = sleepMode::warmStart);
    void goStandby(standbyMode aStandbyMode = standbyMode::rc);
    void executeCommand(sx126xCommand command, uint8_t *commandData, uint8_t commandDataLength);
    void writeRegisters(sx126xRegister theRegister, uint8_t *data, uint8_t dataLength);


    void setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth, codingRate theCodingRate);
    void setRfFrequency(uint32_t frequency);
    void setPacketType(packetType thePacketType);
    // void setPacketParameters(uint16_t preambleLength, headerType theHeaderType, uint8_t payloadLength, CRCType theCRCType, InvertIQ theInvertIQ);
    void setPacketParameters(uint8_t payloadLength);
    void setTxParameters(int8_t transmitPowerdBm, rampTime theRampTime);

    // void setTxParameters(uint8_t paSelect, int8_t power, RadioRampTimes_t rampTime);

    // void setRxConfig(RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint32_t bandwidthAfc, uint16_t preambleLen, uint16_t symbTimeout, bool fixLen, uint8_t payloadLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod, bool iqInverted, bool rxContinuous);
    // void setTxConfig(RadioModems_t modem, int8_t power, uint32_t fdev, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod, bool iqInverted, uint32_t timeout);
    sx126xState getState() const;
    // uint32_t getTimeOnAir(RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, uint8_t payloadLen, bool crcOn);

    void writeRegister(sx126xRegister theRegister, uint32_t newValue);
    uint32_t readRegister(sx126xRegister theRegister);

    // void writeBuffer(sx126xRegister theRegister, uint32_t newValue);        // accesses the payload data buffer
    void writeBuffer(byteBuffer rawMessage);

    // uint32_t readBuffer(sx126xRegister theRegister);        // accesses the payload data buffer
    void readBuffer(byteBuffer rawMessage);

    //    void writeMultipleRegisters(sx126xRegister theRegister, uint32_t newValue); // Dont know if they will be useful, looks like we barely need to write registers, and probably never a RANGE of adjacent registers
    //    uint32_t readMultipleRegisters(sx126xRegister theRegister);

    // uint32_t getWakeupTime();        // in [ms]

    static constexpr uint32_t rxTxBufferLength{256};              // length of the RAM buffer inside the SX126X // TODO : ST firmware mentions 255 io 256.. Why ?k??
    static constexpr uint32_t crystalFrequency{32000000U};        // frequency of the TCXO
    static constexpr uint32_t maxWaitCounter{10000U};             // nmbr of loops in waitOnBusy

    // sx126xError getLastError();        // reads and clears the last error that occured
#ifndef unitTesting
  private:
#endif
    void setRfSwitch(rfSwitchState newState);

    void executeCommand(sx126xCommand opcode, uint8_t *parametersIn, uint8_t arametersInLength, uint8_t *dataOut, uint8_t dataOutLength, bool waitOnBusy = false);

    sx126xError lastError{sx126xError::none};        // remembers the last error that occured
    sx126xState theState{sx126xState::boot};
    sx126xState getStateFromHW();
    bool isBusy();
    void waitOnBusy();
    void setRegulatorMode();
    void setPowerAmplifierConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut);
    static uint32_t calculateFrequencyRegisterValue(uint32_t rfFrequency);

    // void writeSPI(); TODO : replaces ST-HAL some day
    // uint8_t readSPI(); // TODO : replaces ST-HAL some day

    // obsolete or under construction stuff
    // bool detectChannelActivity();
};