// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "measurementchannel.h"

class sensor {
  public:
    void initalize();        // initializes the sensor after booting, reads/performs calibration data, etc.
    float read();            // reads the sensor and store the value into to sample[] array
    void goSleep();          // puts the sensor in sleep mode

    enum class runResult : uint32_t {
        inactive,
        prescaled,
        sampled,
        measured
    };

    runResult run();                    // TODO : clarify this // checks if this sensor needs to be sampled, and if so, samples it
    float lastSample{0.0F};             // TODO : make private and add getter
    float lastMeasurement{0.0F};        // TODO : make private and add getter

    static constexpr uint32_t maxPrescaler{4095};         // take a sample every x times of the 30 second RTC tick
    static constexpr uint32_t maxOversampling{15};        // average x samples before storing it in the sample collection

#ifndef unitTesting
  private:
#endif
    measurementChannel type{measurementChannel::none};        // defines the type of sensor

    // Note : prescaler runs from 1 to 4095, meaning that you sample every 1 to 4095 times the 30 second RTC tick
    // prescaler == 0 indicates you don't want to sample this sensor at all
    // oversampling runs from 0 to 15, meaning that you average 1 to 16 samples before storing it in the sample collection

    bool active{false};                       // indicates if this sensor is active or not
    uint32_t oversamplingLowPower{0};         // controls oversampling for low power mode : on battery
    uint32_t prescalerLowPower{0};            // controls prescaling for low power mode : on battery
    uint32_t oversamplingHighPower{0};        // high power mode : on USB power
    uint32_t prescalerHighPower{0};           //

    float samples[maxOversampling + 1]{};
    uint32_t oversamplingCounter{0};
    uint32_t prescaleCounter{0};

    float average(uint32_t nmbrOfSamples);        // calculates a measurement from samples

    friend class sensorCollection;        // collection is allowed access to the internals of each sensor in its collection
};