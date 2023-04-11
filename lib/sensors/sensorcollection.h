// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "sensor.h"

class sensorCollection {
  public:
    void discover();        // this will scan all hardware ports (I2C, UART, ...) for known sensors, and if found add them to the collection
    void measure();         // this will check all sensors in the collection if they need to be sampled, and will sample them if needed. Sample is stored in sample collection

  private:
    static constexpr uint32_t maxNumberOfSensors{8};
    uint32_t actualNumberOfSensors{0};
    sensor theSensorCollection[maxNumberOfSensors];

    void addSensor(infoChannelType aType, uint32_t oversamplingLowPower, uint32_t prescalerLowPower, uint32_t oversamplingHighPower, uint32_t prescalerHighPower);
    ;
};
