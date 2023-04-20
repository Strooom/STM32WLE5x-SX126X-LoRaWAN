// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <stdint.h>

class power {
  public:
    static void detectUsbConnectOrRemove();        // sample vUSB, then push an event if changed
    static bool isUsbConnected();                  //
    static void measureBatteryLevel();             // samples vBat with ADC, then calculate voltage and % charged
    static uint32_t getBatteryVoltage();           // [mV]
    static uint32_t getBatteryPercent();           // [0-100]

  private:
    static uint32_t batteryPercentCharged;
    static uint32_t voltageInMillivolt;
    static uint32_t calculateVoltageFromRaw(uint32_t adcRaw);
    static uint32_t calculatePercentFromVoltage(uint32_t voltage);
    static bool usbPower;                          // does the hardware senses 5V (VBUS) present
    static uint32_t percentCharged;                // battery level in % [0-100]
    static uint8_t percentFromVoltage[256];        // lookup table for % charged from voltage : 0-255 -> 0-100%
};
