// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "infochanneltype.h"

const char* toString(infoChannelType type) {
    switch (type) {
        case infoChannelType::batteryLevel:
            return "batteryLevel";
            break;

        case infoChannelType::BME680SensorTemperature:
            return "BME680 - Temperature";
            break;

        case infoChannelType::TSL25911VisibleLight:
            return "TSL25911 - Visible Light";
            break;

        default:
            return "unknown infoChannel";
            break;
    }
}