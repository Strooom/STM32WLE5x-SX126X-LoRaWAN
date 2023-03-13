// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class radioState : uint32_t {
    idle,                                    // waiting for the application to start a transmission / SX126x is powered-down
    WaitForRandomDelayBeforeTransmit,        // waiting for a random time before starting to transmit in order to avoid collisions - ALOHA
    waitForTxReady,                          // configured the radio for transmit, waiting until it is powered-up and ready to transmit
    waitForTxComplete,                       // transmitting, waiting for the radio to finish transmitting
    waitForRx1Start,                         //
    waitForRx1CTimeout,                      // listening / Rx : either we receive a msg or we timeout
    waitForRx2Start,                         //
    waitForRx2Timeout,                       // listening / Rx : either we receive a msg or we timeout
};

const char* toString(radioState aState);