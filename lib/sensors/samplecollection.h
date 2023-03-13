// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "sample.h"
// This implements a large circular buffer to store samples.
// Samples are always written to EEPROM, but the head and level are kept in RAM, after reset, we can recover the head and level from scanning the EEPROM



class sampleCollection {
  public:
    void addSample(sample newSample);
    void transmitSamples();
  private:
    static constexpr uint32_t maxNumberOfSamples{8};
    uint32_t actualNumberOfSamples{0};
    sample allSamples[maxNumberOfSamples];
};