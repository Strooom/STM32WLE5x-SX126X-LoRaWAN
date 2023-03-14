// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "region.h"
#include "spreadingfactor.h"
#include "bandwidth.h"

class data_rate {
  public:
  private:
    spreadingFactor theSpreadingFactor;
    bandwidth theBandwidth;
    uint32_t maximumPayloadLength;
    friend class dataRates;
};

class dataRates {
  public:
    void initialize();
    void add(spreadingFactor aSpreadingFactor, bandwidth someBandWidth, uint32_t maxPayloadLength);
    uint32_t getMaximumPayloadLength(uint32_t dataRateIndex);
    static constexpr uint32_t maxNmbrDataRates{8};
    uint32_t nmbrUsedDataRates{0};

  private:
    data_rate theDataRates[8];
};

enum class dataRate : uint32_t {
    DR0 = 0,
    DR1 = 1,
    DR2 = 2,
    DR3 = 3,
    DR4 = 4,
    DR5 = 5,
    DR6 = 6,
    DR7 = 7
};

const char* toString(dataRate aDataRate);

uint32_t getMaximumPayloadLength(dataRate aDataRate);

dataRate getDownlinkDataRate(dataRate uplinkDataRate, uint8_t Rx1DataRateOffset);