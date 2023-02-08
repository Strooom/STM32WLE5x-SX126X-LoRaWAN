// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "stm32wle_peripheral.h"
#include "sx126xregisters.h"
#include "sx126xstate.h"

class sx126x : stm32wlePeripheral {
  public:
    void initialize();
    void prepareForSleep();
    void restoreAfterSleep();
    bool isInitialized();

    void setRxConfig(RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint32_t bandwidthAfc, uint16_t preambleLen, uint16_t symbTimeout, bool fixLen, uint8_t payloadLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod, bool iqInverted, bool rxContinuous);
    void setTxConfig(RadioModems_t modem, int8_t power, uint32_t fdev, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod, bool iqInverted, uint32_t timeout);
    sx126xState getState() const;
    uint32_t getTimeOnAir(RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, uint8_t payloadLen, bool crcOn);

    void goSleep();
    void goStandby();

    void writeRegister(sx126xRegister theRegister, uint32_t newValue);
    uint32_t readRegister(sx126xRegister theRegister);
    void writeMultipleRegisters(sx126xRegister theRegister, uint32_t newValue);
    uint32_t readMultipleRegisters(sx126xRegister theRegister);

    uint32_t getWakeupTime(); // in [ms]

    constexpr uint32_t rxTxBufferLength{256};              // length of the RAM buffer inside the SX126X // TODO : ST firmware mentions 255 io 256.. Why ???
    constexpr uint32_t crystalFrequency{30000000U};        // frequency of the TCXO

  private:
    sx126xState theState{sx126xState::boot};
};


void SUBGRF_SetPayload( uint8_t *payload, uint8_t size )
{
    SUBGRF_WriteBuffer( 0x00, payload, size );
}

void SUBGRF_SetTx( uint32_t timeout )
{
    uint8_t buf[3];

    OperatingMode = MODE_TX;

    buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( timeout & 0xFF );
    SUBGRF_WriteCommand( RADIO_SET_TX, buf, 3 );
} 

void SUBGRF_SendPayload( uint8_t *payload, uint8_t size, uint32_t timeout)
{
    SUBGRF_SetPayload( payload, size );
    SUBGRF_SetTx( timeout );
}