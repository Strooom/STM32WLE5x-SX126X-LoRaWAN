// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <stdint.h>

class power {
  public:
    void detectUsbConnectOrRemove();        // sample vUSB, then push an event if changed
    bool isUsbConnected();                  //
    void measureBatteryLevel();             // samples vBat with ADC, then calculate voltage and % charged
    uint32_t getBatteryVoltage();           // [mV]
    uint32_t getBatteryPercent();           // [0-100]

  private:
    uint32_t batteryPercentCharged{0};
    uint32_t voltageInMillivolt{0};
    uint32_t calculateVoltageFromRaw(uint32_t adcRaw);
    uint32_t calculatePercentFromVoltage(uint32_t voltage);
    bool usbPower{false};        // does the hardware senses 5V (VBUS) present
    uint32_t percentCharged{0}; // battery level in % [0-100]

    uint8_t percentFromVoltage[256]{}; // lookup table for % charged from voltage : 0-255 -> 0-100%
};
