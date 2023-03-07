// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

class logging {
  public:
    static constexpr uint32_t bufferLength = 1024;        //
	void snprintf(const char *format, ...);

  private:
    char buffer[bufferLength]{};        // Transmit buffer
};
