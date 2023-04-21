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

#include "spreadingfactor.h"
#include "sx126xcommands.h"
#include "sx126xramptime.h"
#include "sx126xregisters.h"
#include "sx126xparameters.h"
#include "rfswitch.h"

class sx126x {
  public:
    // void reset();             // reset the SX126 from its HW reset pin - in the STMWLE5 this is done from a register in RCC
    void initialize();        // initialize the the SPI interface to the SX126, then the SX126x chip itself

    void goSleep(sleepMode aSleepMode = sleepMode::warmStart);
    void goStandby(standbyMode aStandbyMode = standbyMode::rc);

    void configForTransmit(spreadingFactor theSpreadingFactor, uint32_t frequency, uint8_t *payload, uint32_t payloadLength);
    void configForReceive(spreadingFactor theSpreadingFactor, uint32_t frequency);

    void startTransmit(uint32_t timeOut);
    void startReceive(uint32_t timeOut);

    void getReceivedMessage();

    static constexpr uint32_t rxTxBufferLength{256};              // length of the RAM buffer inside the SX126X // TODO : ST firmware mentions 255 io 256.. Why ???
    static constexpr uint32_t crystalFrequency{32000000U};        // frequency of the TCXO

#ifndef unitTesting
  private:
#endif
    void initializeInterface();        // configures the SPI between the MCU and the SX126x
    void initializeRadio();            //

    void setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth);
    void setRfFrequency(uint32_t frequency);

    void setPacketParametersTransmit(uint8_t payloadLength);
    void setPacketParametersReceive();

    void setTxParameters(int8_t transmitPowerdBm, rampTime theRampTime);

    void setRegulatorMode();
    static uint32_t calculateFrequencyRegisterValue(uint32_t rfFrequency);
    void setRfSwitch(rfSwitchState newState);

    void executeCommand(sx126xCommand command, uint8_t *commandData, uint8_t commandDataLength);

    void writeRegister(sx126xRegister theRegister, uint32_t newValue);
    void writeRegisters(sx126xRegister theRegister, uint8_t *data, uint8_t dataLength);
    uint32_t readRegister(sx126xRegister theRegister);

    void readBuffer(uint8_t *payload, uint32_t payloadLength);
    void writeBuffer(uint8_t *payload, uint32_t payloadLength);
};
