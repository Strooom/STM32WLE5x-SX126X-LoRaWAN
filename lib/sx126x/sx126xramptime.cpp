// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126xramptime.h"

const char* toString(rampTime theRampTime) {
    switch (theRampTime) {
        case rampTime::rt10us:
            return "10us";
        case rampTime::rt20us:
            return "20us";
        case rampTime::rt40us:
            return "40us";
        case rampTime::rt80us:
            return "80us";
        case rampTime::rt200us:
            return "200us";
        case rampTime::rt800us:
            return "800us";
        case rampTime::rt1700us:
            return "1700us";
        case rampTime::rt3400us:
            return "3400us";
        default:
            return "unknown";
    }
}



