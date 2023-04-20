// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "measurementchannel.h"

const char* toString(measurementChannel type) {
    switch (type) {
        case measurementChannel::batteryLevel:
            return "batteryLevel";
            break;

        case measurementChannel::BME680SensorTemperature:
            return "BME680 - Temperature";
            break;

        case measurementChannel::BME680SensorRelativeHumidity:
            return "BME680 - Relative Humidity";
            break;

        case measurementChannel::BME680SensorBarometricPressure:
            return "BME680 - Barometer";
            break;

        case measurementChannel::TSL25911VisibleLight:
            return "TSL25911 - Visible Light";
            break;

        case measurementChannel::TSL25911Infrared:
            return "TSL25911 - Infrared Light";
            break;

        default:
            return "unknown infoChannel";
            break;
    }
}