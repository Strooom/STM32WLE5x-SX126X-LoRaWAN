#include "logging.h"
#include "sensor.h"
#include "power.h"

// List of all sensors we know about

#include "bme680.h"
#include "tsl2591.h"

void sensor::run() {
    logging::snprintf("Run Sensor [%d] %s : ", static_cast<uint8_t>(type), toString(type));

    uint32_t oversampling;
    uint32_t prescaler;

    if (power::hasUsbPower()) {
        oversampling = oversamplingHighPower;
        prescaler    = prescalerHighPower;
    } else {
        oversampling = oversamplingLowPower;
        prescaler    = prescalerLowPower;
    }
    if (prescaler > 0) {                          // a value of 0 means we don't want to sample this sensor at all
        if (prescaleCounter > prescaler) {        // when switching between low power and high power mode, the prescaleCounter could need to be reset in the appropriate range
            prescaleCounter = prescaler;
        }

        if (oversamplingCounter > oversampling) {        // when switching between low power and high power mode, the oversamplingCounter could need to be reset in the appropriate range
            oversamplingCounter = oversampling;
        }

        if (prescaleCounter <= 1) {
            prescaleCounter             = prescaler;
            sample[oversamplingCounter] = read();        // take a new sample for this sensor and store it in the array of samples
            logging::snprintf("sampled, value = %f", sample[oversamplingCounter]);

            if (oversamplingCounter == 0) {
                store();        // average all samples & output this measurement to NVS
                logging::snprintf("measurement stored\n");
                oversamplingCounter = oversampling;
            } else {
                logging::snprintf("\n");
                oversamplingCounter--;
                prescaleCounter = prescaler;
            }
        } else {
            logging::snprintf("skipped\n");
            prescaleCounter--;
        }
    } else {
        logging::snprintf("inactive\n");
    }
}

float sensor::read() {
    switch (type) {
        case measurementChannel::batteryLevel:
            return 3.3F;
            break;

        case measurementChannel::BME680SensorTemperature:
            if (!bme680::isAwake()) {
                bme680::run();
            }
            return bme680::getTemperature();
            break;

        case measurementChannel::BME680SensorRelativeHumidity:
            if (!bme680::isAwake()) {
                bme680::run();
            }
            return bme680::getRelativeHumidity();
            break;

        case measurementChannel::BME680SensorBarometricPressure:
            if (!bme680::isAwake()) {
                bme680::run();
            }
            return bme680::getBarometricPressure();
            break;

        default:
            return 0.0F;
            break;
    }
}

void sensor::goSleep() {
    switch (type) {
        case measurementChannel::batteryLevel:
            break;

        case measurementChannel::BME680SensorTemperature:
            bme680::goSleep();
            break;

        default:
            break;
    }
}

void sensor::store() {
}

void sensor::initalize() {
    switch (type) {
        case measurementChannel::batteryLevel:
            break;

        case measurementChannel::BME680SensorTemperature:
        case measurementChannel::BME680SensorRelativeHumidity:
        case measurementChannel::BME680SensorBarometricPressure:
            bme680::initialize();
            break;

        default:
            break;
    }
}