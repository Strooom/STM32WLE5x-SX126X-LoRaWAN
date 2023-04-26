// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

enum class measurementChannel : uint32_t {
    none = 0x00,

    batteryLevel                   = 0x01,
    BME680SensorTemperature        = 0x10,
    BME680SensorRelativeHumidity   = 0x11,
    BME680SensorBarometricPressure = 0x12,
    TSL25911VisibleLight           = 0x20,
    TSL25911Infrared               = 0x21,
    status                         = 0xE0,
    events                         = 0xF0
};

const char* toString(measurementChannel aChannel);


// °C