// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "macstate.h"

class LoRaWAN {
  public:
    void initialize();
    macState getState() const;
    // bool joinNetwork(); // TODO : using ABP for the time being to keep things simple for now
    bool isReadyToTRansmit();
    void sendMessage(const uint8_t* payload);
    

    // TODO : we need a reference to NVS so we can store keys etc in EEPROM

  private:
    macState theState{macState::idle};
    static constexpr uint32_t rxBufferLength{256};
    static constexpr uint32_t txBufferLength{256};
    uint8_t rxBuffer[rxBufferLength]{};
    uint8_t txBuffer[txBufferLength]{};
};
