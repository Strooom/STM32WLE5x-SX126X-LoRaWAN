
// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class macCommand : uint32_t {
    linkCheck                      = 0x02,
    linkAdaptiveDataRate           = 0x03,
    endDeviceTransmitDutyCycle     = 0x04,
    receiveWindowTiming            = 0x05,
    deviceStatus                   = 0x06,
    newChannel                     = 0x07,
    endDeviceTransmitParameters    = 0x09,
    modifyDownlinkChannelFrequency = 0x0A,
    deviceTime                     = 0x0D
};


const char* toString(macCommand aCommand);
