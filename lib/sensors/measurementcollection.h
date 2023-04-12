// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

#include "measurement.h"
// This implements a large circular buffer to store samples.
// Samples are always written to EEPROM, but the head and level are kept in RAM, after reset, we can recover the head and level from scanning the EEPROM


class measurementCollection {
  public:
    void add(measurement newSample);
    void get();

    uint32_t getNmbrToBeTransmitted(); // how many measurements are still to be transmitted
  private:
    uint32_t head{0};
    uint32_t level{0};
};