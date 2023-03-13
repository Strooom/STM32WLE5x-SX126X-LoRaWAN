// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ### 
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ### 
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ### 
// ######################################################################################

#pragma once
#include <stdint.h>

enum class infoChannelType : uint32_t {
    batteryLevel = 0x00,

    mcuTemperature          = 0x08,
    displayTemperature      = 0x09,
    BME680SensorTemperature = 0x0A,

    BME680SensorRelativeHumidity = 0x10,

    BME680SensorBarometricPressure = 0x18,

    TSL25911SensorLightIntensity = 0x20,

    status = 0xE0,
    events = 0xF0
};


const char* toString(const infoChannelType aChannel);