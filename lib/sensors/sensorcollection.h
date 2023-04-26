// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "sensor.h"
#include "measurement.h"

class sensorCollection {
  public:
    void discover();        // this will scan all hardware ports (I2C, UART, ...) for known sensors, and if found add them to the collection
    enum class runResult : uint32_t {
        none,
        newMeasurements,
    };
    runResult run();        // this will check all sensors in the collection if they need to be sampled, and will measurement them if needed. Sample is stored in measurement collection

    static constexpr uint32_t maxNumberOfSensors{8};
    measurement latestMeasurements[maxNumberOfSensors];        // collects the measurements of all sensors during a run
    uint32_t actualNumberOfMeasurements{0};                    // counts the number of measurements in the latestMeasurements array

#ifndef unitTesting
  private:
#endif
    uint32_t actualNumberOfSensors{0};
    sensor theSensorCollection[maxNumberOfSensors];


    void addSensor(measurementChannel aType, uint32_t oversamplingLowPower, uint32_t prescalerLowPower, uint32_t oversamplingHighPower, uint32_t prescalerHighPower);
    void addMeasurement(measurementChannel aType, float aValue);
};
