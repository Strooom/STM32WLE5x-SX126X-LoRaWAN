// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class region {
    public:
        uint8_t loRaSyncword; // EU868 = 0x34
        uint8_t preambleLength; // EU868 = 8 symbols
        channels ch; // object holding the avaialble / active channels in this region

};