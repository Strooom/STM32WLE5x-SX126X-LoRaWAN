// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "datarate.h"

class AdaptiveDataRate {
  public:
    void setDataRate(dataRate newDataRate);
    void decreaseDataRate();
    static constexpr uint32_t unAcknowledgedUplinksLimit{64}; // after this amount of uplink messages, we set a flag to request an acknoledge
    static constexpr uint32_t unAcknowledgedUplinksDelay{32}; // after this amount of uplink messages with request for acknowledge flag set, and no acknowledge received, we decrease the data rate.

  private:
    dataRate currentDataRate{dataRate::DR0};
    uint32_t unAcknowledgedUplinksCount{0};
};