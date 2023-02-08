// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "loramacstate.h"

class LoRaWAN {
  public:
    void initialize();
    loraMacState getState() const;
    bool joinNetwork();
    void sendMessage();

    // TODO : we need a reference to NVS so we can store keys etc in EEPROM
    

  private:
    loraMacState theState{loraMacState::boot};
    constexpr uint32_t rxBufferLength{256};
    constexpr uint32_t txBufferLength{256};
    uint8_t rxBuffer[rxBufferLength]{};
    uint8_t txBuffer[txBufferLength]{};
};


.DevEui = { 0 },  // Automatically filed from secure-element
    .JoinEui = { 0 }, // Automatically filed from secure-element
    .NetworkId = LORAWAN_NETWORK_ID,
    .DevAddr = LORAWAN_DEVICE_ADDRESS,
       .AdrEnabled =       LORAMAC_HANDLER_ADR_ON,
    .DutyCycleEnabled = false,