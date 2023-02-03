// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "stm32wle_peripheral.h"

class sx126x : stm32wlePeripheral {
    void initialize();
    void prepareForSleep();
    void restoreAfterSleep();
    bool isInitialized();

  public:
  private:
};
