// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "sensorcollection.h"
#include "bme680.h"
#include "tsl2591.h"
#include "logging.h"

extern logging theLog;

void sensorCollection::measure() {
    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        theSensorCollection[sensorIndex].run();
    }
}

void sensorCollection::discover() {
    addSensor(infoChannelType::batteryLevel, 7, 359, 7, 14);        // one measurement per day on battery, one per hour on USB power

    if (bme680::isPresent()) {
        addSensor(infoChannelType::BME680SensorTemperature, 3, 9, 3, 9);        // one measurement per 20 minutes
        // addSensor(infoChannelType::BME680SensorTemperature, 3, 9, 3, 9);        // one measurement per 20 minutes
        // addSensor(infoChannelType::BME680SensorTemperature, 3, 9, 3, 9);        // one measurement per 20 minutes
    }

    if (tsl2591::isPresent()) {
        addSensor(infoChannelType::TSL25911VisibleLight, 3, 9, 3, 9);        //
        // addSensor(infoChannelType::TSL25911Infrared, 3, 9, 3, 9);        //
    }
}

void sensorCollection::addSensor(infoChannelType aType, uint32_t oversamplingLowPower, uint32_t prescalerLowPower, uint32_t oversamplingHighPower, uint32_t prescalerHighPower) {
    if (actualNumberOfSensors < maxNumberOfSensors) {
        theLog.snprintf("%u : Added Sensor [%s]\n", (actualNumberOfSensors + 1), toString(aType));
        theSensorCollection[actualNumberOfSensors].type                  = aType;
        theSensorCollection[actualNumberOfSensors].oversamplingLowPower  = oversamplingLowPower;
        theSensorCollection[actualNumberOfSensors].prescalerLowPower     = prescalerLowPower;
        theSensorCollection[actualNumberOfSensors].oversamplingHighPower = oversamplingHighPower;
        theSensorCollection[actualNumberOfSensors].prescalerHighPower    = prescalerHighPower;
        actualNumberOfSensors++;
    } else {
        // TODO : log the error
    }
}
