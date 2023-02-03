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

    sx126xState getState() const;
    uint32_t getTimeOnAir ( RadioModems_t modem, uint32_t bandwidth, uint32_t datarate, uint8_t coderate, uint16_t preambleLen, bool fixLen, uint8_t payloadLen, bool crcOn );

    void goSleep();
    void goStandby();

    void writeRegister(sx126xRegister theRegister, uint32_t newValue);
    uint32_t readRegister(sx126xRegister theRegister);

  private:
    sx126xState theState{sx126xState::boot};
};
