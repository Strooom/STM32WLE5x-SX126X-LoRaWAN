// #############################################################################
// ### This file is part of the source code for the Moovr CNC Controller     ###
// ### https://github.com/Strooom/Moovr                                      ###
// ### Author : Pascal Roobrouck - @strooom                                  ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once

#include <stdint.h>
#include "mainstate.h"

class mainController {
  public:
    void initialize();
    void handleEvents();

  private:
    // ### Properties ###
    // ### StateMachines ###
    mainState theMainState = mainState::boot;

    // uint32_t rootSamplingPeriod{1};        // value between 1 and 32, controlling the basic rate at which the application wakes up, and takes samples from the sensors.
};
