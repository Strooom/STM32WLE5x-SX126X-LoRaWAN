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
    actualNumberOfMeasurements = 0;        // reset the array of measurements

    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        sensor::runResult theResult = theSensorCollection[sensorIndex].run();
        switch (theResult) {
            case sensor::runResult::inactive:
                break;
            case sensor::runResult::prescaled:
                break;
            case sensor::runResult::sampled:
                break;
            case sensor::runResult::measured:
                addMeasurement(theSensorCollection[sensorIndex].type, theSensorCollection[sensorIndex].lastMeasurement);
                break;
        }
    }

    // After all measurements are done, check them all and put them to sleep if needed.
    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        theSensorCollection[sensorIndex].goSleep();
    }
}

void sensorCollection::discover() {
    addSensor(measurementChannel::batteryLevel, 7, 359, 7, 15);        // one measurement per day on battery, one per hour on USB power

    if (bme680::isPresent()) {
        addSensor(measurementChannel::BME680SensorTemperature, 3, 9, 3, 9);                 // one measurement per 20 minutes
        addSensor(measurementChannel::BME680SensorRelativeHumidity, 3, 29, 3, 29);          // one measurement per 60 minutes
        addSensor(measurementChannel::BME680SensorBarometricPressure, 3, 29, 3, 29);        // one measurement per 60 minutes
    }

    if (tsl2591::isPresent()) {
        addSensor(measurementChannel::TSL25911VisibleLight, 3, 9, 3, 9);        //
        addSensor(measurementChannel::TSL25911Infrared, 3, 9, 3, 9);            //
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

        theSensorCollection[actualNumberOfSensors].oversamplingCounter = sensor::maxOversampling;        // will be reduced to either oversamplingLowPower or oversamplingHighPower on the first run. If it would be initialized to 0, it would output a measurement on first run without having done all the samples

        theSensorCollection[actualNumberOfSensors].active = true;
        actualNumberOfSensors++;
    } else {
        // TODO : log the error
    }
}

void sensorCollection::addMeasurement(measurementChannel aType, float aValue) {
    if (actualNumberOfMeasurements < maxNumberOfSensors) {
        latestMeasurements[actualNumberOfMeasurements].timestamp = measurement::getTimeStamp();
        latestMeasurements[actualNumberOfMeasurements].type      = aType;
        latestMeasurements[actualNumberOfMeasurements].value     = aValue;
        latestMeasurements[actualNumberOfMeasurements].flags     = 0;
        actualNumberOfMeasurements++;
    }
}