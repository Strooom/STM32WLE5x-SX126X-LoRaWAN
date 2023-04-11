// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

enum class applicationEvent : uint8_t {
    usbConnected,
    usbRemoved,

    realTimeClockTick,
    lowPowerTimerExpired,

    downlinkMessageReceived,
};

const char* toString(applicationEvent anEvent);