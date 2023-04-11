// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

#include "msiclockfrequency.h"
#include "sleepmode.h"
#include "voltagescale.h"

class stm32wleCpu {
  public:
    void initialize();
    void initializePower();
    void initializeClock();
    void goSleep(sleepMode theSleepMode);        // set CPU in STOP2 (or other mode)
    uint64_t getUniqueId() const;
    void reset();
    void waitForStableClock(); // delay a just a few clock cycles, needed afte enabling clocks towards peripherals

  private:
    // Power
    voltageScale getVoltageScaling();                        //
    void setVoltageScaling(voltageScale theNewScale);        // set the voltage scaling to 1.0V or 1.2V to choose between high performance and low power

    // Clocks
    msiClockFrequency getClockSpeed();                              // returns the current MSI clock speed
    void setClockSpeed(msiClockFrequency newClockFrequency);        // control the speed of the MSI
    uint32_t clockFrequencyToMsiRange(msiClockFrequency theClockFrequency);
    msiClockFrequency msiRangeToClockFrequency(uint32_t msiRange);
    msiClockFrequency currentClockSpeed{msiClockFrequency::f4MHz};        //

    // Memory Access
    void setFlashWaitstates(msiClockFrequency theClockFrequency);

    // SysTick timer
    void setSystickFrequency(msiClockFrequency theClockFrequency);
    void enableSystick(bool onOff);
};
