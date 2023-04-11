// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

#include "stm32wle5.h"

class realTimeClock {
  public:
    void initialize();
    bool isInitialized() const;
    bool hasValidTime() const;
    void getTime() const;
    void setTime();

  private:
    void setRootSamplePeriod(uint32_t newPeriod);
    void setWriteProtection(bool writeProtectOn);
    void setInitializationMode(bool initializationModeOn);
};

