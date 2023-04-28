// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "sensorcollection.h"
#include "bme680.h"
#include "tsl2591.h"
#include "logging.h"

sensorCollection::runResult sensorCollection::run() {
    actualNumberOfMeasurements = 0;        // reset the array of measurements

    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        sensor::runResult theResult = theSensorCollection[sensorIndex].run();
        switch (theResult) {
            case sensor::runResult::inactive:
                break;
            case sensor::runResult::prescaled:
                logging::snprintf("Sensor %s : prescale tick\n", toString(theSensorCollection[sensorIndex].type));
                break;
            case sensor::runResult::sampled:
                logging::snprintf("Sensor %s : sampled %.2f\n", toString(theSensorCollection[sensorIndex].type), theSensorCollection[sensorIndex].lastSample);
                break;
            case sensor::runResult::measured:
                logging::snprintf("Sensor %s : measurement %.4f\n", toString(theSensorCollection[sensorIndex].type), theSensorCollection[sensorIndex].lastMeasurement);
                addMeasurement(theSensorCollection[sensorIndex].type, theSensorCollection[sensorIndex].lastMeasurement);
                break;
        }
    }

    // After all measurements are done, check them all and put them to sleep if needed.
    for (uint32_t sensorIndex = 0; sensorIndex < actualNumberOfSensors; sensorIndex++) {
        theSensorCollection[sensorIndex].goSleep();
    }

    if (actualNumberOfMeasurements > 0) {
        return runResult::newMeasurements;
    } else {
        return runResult::none;
    }
}

void sensorCollection::discover() {
    // addSensor(measurementChannel::batteryLevel, 7, 359, 7, 15);        // one measurement per day on battery, one per hour on USB power
    addSensor(measurementChannel::batteryLevel, 9, 0, 4, 0);        // test Version

    if (bme680::isPresent()) {
        bme680::initialize();        // this reads the calibration data from the sensor

        //        addSensor(measurementChannel::BME680SensorTemperature, 3, 9, 3, 9);                 // one measurement per 20 minutes
        //        addSensor(measurementChannel::BME680SensorRelativeHumidity, 3, 29, 3, 29);          // one measurement per 60 minutes
        //        addSensor(measurementChannel::BME680SensorBarometricPressure, 3, 59, 3, 59);        // one measurement per 120 minutes
        addSensor(measurementChannel::BME680SensorTemperature, 9, 0, 3, 0);               // test
        addSensor(measurementChannel::BME680SensorRelativeHumidity, 7, 0, 2, 0);          // test
        addSensor(measurementChannel::BME680SensorBarometricPressure, 8, 0, 1, 0);        // test
    }

    if (tsl2591::isPresent()) {
        //        addSensor(measurementChannel::TSL25911VisibleLight, 3, 9, 3, 9);        //
        //        addSensor(measurementChannel::TSL25911Infrared, 3, 9, 3, 9);            //
    }
}

void sensorCollection::addSensor(measurementChannel aType, uint32_t oversamplingLowPower, uint32_t prescalerLowPower, uint32_t oversamplingHighPower, uint32_t prescalerHighPower) {
    if (actualNumberOfSensors < maxNumberOfSensors) {
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
        latestMeasurements[actualNumberOfMeasurements].timestampAsUInt32 = measurement::getTimeStamp();
        latestMeasurements[actualNumberOfMeasurements].type              = aType;
        latestMeasurements[actualNumberOfMeasurements].valueAsFloat      = aValue;
        latestMeasurements[actualNumberOfMeasurements].flags             = 0;
        actualNumberOfMeasurements++;
    }
}
