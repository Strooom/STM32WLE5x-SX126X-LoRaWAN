// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "adcchannel.h"

const char* toString(adcChannel aChannel) {
    switch (aChannel) {
        case adcChannel::none:
            return "none";
            break;

        case adcChannel::internalVoltageReference:
            return "internal VoltageReference";
            break;

        case adcChannel::internalTemperatureSensor:
            return "internal Temperature";
            break;

         default:
            return "unknown Channel";
            break;
    }
}
