// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <stdint.h>

class power {
  public:
    static bool isUsbConnected();        // detect insertion of USB cable event
    static bool isUsbRemoved();          // detect removal of USB cable event
    static bool hasUsbPower();
    static void measureBatteryLevel();          // samples vRefint with ADC, then calculate voltage and % charged
    static uint32_t getBatteryVoltage();        // [mV]
    static uint32_t getBatteryPercent();        // [0-100]

  private:
    static uint32_t batteryPercentCharged;
    static uint32_t voltageInMillivolt;
    static uint32_t calculatePercentFromVoltage(uint32_t voltage);        //
    static bool usbPower;                                                 // remembers previous usbPresent state
    static uint8_t percentFromVoltage[256];                               // lookup table for % charged from voltage : 0-255 -> 0-100%
};
