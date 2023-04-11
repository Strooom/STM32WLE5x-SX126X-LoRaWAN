// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class cli {
  public:
    void handleRxEvent();
    void handleEvents();

  private:
    static constexpr uint32_t commandBufferLength{65};
    uint8_t commandBuffer[commandBufferLength]{0};
};
