// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class txRxCycleState : uint32_t {
    idle,                                   // radio is standby - initialized
    waitForCadEnd,                          // waiting for the radio to finish the channel activity detection
    waitForRandomTimeBeforeTransmit,        // waiting for a random time before starting to transmit in order to avoid collisions - ALOHA
    waitForTxComplete,                      // transmitting, waiting for the radio to finish transmitting
    waitForRx1Start,                        //
    waitForRx1CompleteOrTimeout,            // listening / Rx : either we receive a msg or we timeout
    waitForRx2Start,                        //
    waitForRx2CompleteOrTimeout,            // listening / Rx : either we receive a msg or we timeout
};

const char* toString(txRxCycleState aState);

enum class loRaWanEvent : uint8_t {
    none = 0x00,
    sx126xCadEnd,            //
    sx126xTxComplete,        // transmit complete interrupt from the SX126x
    sx126xRxComplete,        // receive complete interrupt from the SX126x
    sx126xTimeout,           // interrupt from the SX126x timeout
    timeOut,                 // interrupt from the LPTIM1
};

const char* toString(loRaWanEvent anEvent);