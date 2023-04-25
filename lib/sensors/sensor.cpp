#include "sensor.h"
#include "power.h"

// List of all sensors we know about

#include "bme680.h"
#include "tsl2591.h"

// #define sensorLogging
// #include "logging.h"

void sensor::run() {
#ifdef sensorLogging
    logging::snprintf("Run Sensor [%d] %s : ", static_cast<uint8_t>(type), toString(type));
#endif
    uint32_t oversampling;
    uint32_t prescaler;

    if (active) {
        if (power::hasUsbPower()) {
            oversampling = oversamplingHighPower;
            prescaler    = prescalerHighPower;
        } else {
            oversampling = oversamplingLowPower;
            prescaler    = prescalerLowPower;
        }

        if (prescaleCounter > prescaler) {        // when switching between low power and high power mode, the prescaleCounter could need to be reset in the appropriate range
            prescaleCounter = prescaler;
        }

        if (oversamplingCounter > oversampling) {        // when switching between low power and high power mode, the oversamplingCounter could need to be reset in the appropriate range
            oversamplingCounter = oversampling;
        }

        if (prescaleCounter == 0) {
            prescaleCounter             = prescaler;
            sample[oversamplingCounter] = read();        // take a new sample for this sensor and store it in the array of samples
#ifdef sensorLogging
            logging::snprintf("sampled, value = %f", sample[oversamplingCounter]);
#endif
            if (oversamplingCounter == 0) {
                store();        // average all samples & output this measurement to NVS
#ifdef sensorLogging
                logging::snprintf("measurement stored\n");
#endif
                oversamplingCounter = oversampling;
            } else {
#ifdef sensorLogging
                logging::snprintf("\n");
#endif
                oversamplingCounter--;
                prescaleCounter = prescaler;
            }
        } else {
#ifdef sensorLogging
            logging::snprintf("skipped\n");
#endif
            prescaleCounter--;
        }
    } else {
#ifdef sensorLogging
        logging::snprintf("inactive\n");
#endif
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
        case measurementChannel::BME680SensorRelativeHumidity:
        case measurementChannel::BME680SensorBarometricPressure:
            if (bme680::isAwake()) {
                bme680::goSleep();
            }
            break;

        case measurementChannel::TSL25911Infrared:
        case measurementChannel::TSL25911VisibleLight:
            tsl2591::goSleep();
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