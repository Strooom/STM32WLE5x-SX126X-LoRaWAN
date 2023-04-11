// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126x.h"

// for a certain radio frequency rfFrequency, the SX126x needs a different value to be passed in setRfFrequency()
// RM0461 datasheet, page 138, Set_RfFrequency() command
// RF-PLL frequency = 32e6 x RFfreq / 2^25
// RF-PLL frequency = the real frequency, eg. 868'100'000 Hz
// RFFreq = the value to be passed to the SX126x, eg. 0x36419999
// 32e6 = the TCXO crystal frequency, eg. 32'000'000 Hz

uint32_t sx126x::calculateFrequencyRegisterValue(uint32_t rfFrequency) {
    uint64_t tmpResult;
    tmpResult = (uint64_t)rfFrequency << 25;
    tmpResult = tmpResult / crystalFrequency;
    return static_cast<uint32_t>(tmpResult);
}



    void sx126x::initialize() {}
