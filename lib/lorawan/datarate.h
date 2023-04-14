// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

#include "spreadingfactor.h"
#include "bandwidth.h"

class dataRate {
  public:
    spreadingFactor theSpreadingFactor;
    bandwidth theBandwidth;
    uint32_t maximumPayloadLength;

  private:
};

class dataRates {
  public:
    static constexpr uint32_t nmbrUsedDataRates{6};
    const dataRate theDataRates[nmbrUsedDataRates]{
        // for some C++ language reason, I cannot make this constexpr..
        {spreadingFactor::SF12, bandwidth::b125kHz, 51},
        {spreadingFactor::SF11, bandwidth::b125kHz, 51},
        {spreadingFactor::SF10, bandwidth::b125kHz, 51},
        {spreadingFactor::SF9, bandwidth::b125kHz, 115},
        {spreadingFactor::SF8, bandwidth::b125kHz, 242},
        {spreadingFactor::SF7, bandwidth::b125kHz, 242},
    };
    static uint32_t getDownlinkDataRateIndex(uint32_t uplinkDataRateIndex, uint32_t Rx1DataRateOffset);

  private:
};
