// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

// See SX126x datasheet, section 13.5.1 GetStatus

enum class chipMode : uint8_t {
    standbyRc            = 0x02,
    standbyHseOscillator = 0x03,
    frequencySynthesis   = 0x04,
    receiving            = 0x05,
    transmitting         = 0x06,
};

const char* toString(chipMode aMode);

enum class commandStatus : uint8_t {

    dataAvailableForHost    = 0x02,
    commandTimeOut          = 0x03,
    commandProcessingError  = 0x04,
    commandExecutionFailure = 0x05,
    commandTransmitDone     = 0x06,
};

const char* toString(commandStatus aStatus);