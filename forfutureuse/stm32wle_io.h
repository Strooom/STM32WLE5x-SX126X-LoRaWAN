// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

#include "stm32wle_peripheral.h"

class stm32wleIo : public stm32wlePeripheral {
  public:
	void initialize();
	void prepareForSleep();
	void restoreAfterSleep();

  private:
};
