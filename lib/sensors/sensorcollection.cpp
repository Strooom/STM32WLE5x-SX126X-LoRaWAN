// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "sensorcollection.h"
#include "bme680.h"
#include "tsl2591.h"
#include "logging.h"

void sensorCollection::measure() {
    // Keep the sensor awake as more measurements could be needed from the same sensor, eg BME68X
    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        theSensorCollection[sensorIndex].run();
    }
    // Then after all measurements are done, check them all and put them to sleep if needed.
    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        theSensorCollection[sensorIndex].goSleep();
    }
}

void sensorCollection::discover() {
    addSensor(measurementChannel::batteryLevel, 8, 360, 8, 15);        // one measurement per day on battery, one per hour on USB power

    if (bme680::isPresent()) {
        addSensor(measurementChannel::BME680SensorTemperature, 4, 10, 4, 10);               // one measurement per 20 minutes
        addSensor(measurementChannel::BME680SensorRelativeHumidity, 4, 30, 4, 30);          // one measurement per 60 minutes
        addSensor(measurementChannel::BME680SensorBarometricPressure, 4, 30, 4, 30);        // one measurement per 60 minutes
    }

    if (tsl2591::isPresent()) {
        addSensor(measurementChannel::TSL25911VisibleLight, 4, 10, 4, 10);        //
        addSensor(measurementChannel::TSL25911Infrared, 0, 0, 4, 10);             //
    }
}

void sensorCollection::addSensor(measurementChannel aType, uint32_t oversamplingLowPower, uint32_t prescalerLowPower, uint32_t oversamplingHighPower, uint32_t prescalerHighPower) {
    if (actualNumberOfSensors < maxNumberOfSensors) {
        logging::snprintf("%u : Added Sensor [%s]\n", (actualNumberOfSensors + 1), toString(aType));
        if (oversamplingLowPower > sensor::maxOversampling) {
            oversamplingLowPower = sensor::maxOversampling;
        }
        if (oversamplingHighPower > sensor::maxOversampling) {
            oversamplingHighPower = sensor::maxOversampling;
        }
        if (prescalerLowPower > sensor::maxPrescaler) {
            prescalerLowPower = sensor::maxPrescaler;
        }
        if (prescalerHighPower > sensor::maxPrescaler) {
            prescalerHighPower = sensor::maxPrescaler;
        }
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
