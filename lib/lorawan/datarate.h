// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

#include "bandwidth.h"
#include "region.h"
#include "spreadingfactor.h"

class dataRate {
  public:
    spreadingFactor theSpreadingFactor;
    bandwidth theBandwidth;
    uint32_t maximumPayloadLength;
  private:
    friend class dataRates;
};

class dataRates {
  public:
    void initialize();
    void add(spreadingFactor aSpreadingFactor, bandwidth someBandWidth, uint32_t maxPayloadLength);
    uint32_t getMaximumPayloadLength(uint32_t dataRateIndex);
    static constexpr uint32_t maxNmbrDataRates{8};
    dataRate get(uint32_t index);
    static dataRate getDownlinkDataRate(uint8_t Rx1DataRateOffset);

  private:
    dataRate theDataRates[8];
    uint32_t nmbrUsedDataRates{0};
};
