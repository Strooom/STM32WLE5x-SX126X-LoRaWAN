#include "logging.h"
extern logging theLog;
#include "sensor.h"

// List of all sensors we know about
#include "power.h"
extern power thePowerControl;
#include "bme680.h"
#include "tsl2591.h"

void sensor::run() {
    theLog.snprintf("Run Sensor [%d] %s : ", static_cast<uint8_t>(type), toString(type));

    uint32_t oversampling;
    uint32_t prescaler;

    if (thePowerControl.isUsbConnected()) {
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
            theLog.snprintf("sampled ");

            if (oversamplingCounter == 0) {
                store();        // average all samples & output this measurement to NVS
                theLog.snprintf("measurement stored\n");
                oversamplingCounter = oversampling;
            } else {
                theLog.snprintf("\n");
                oversamplingCounter--;
                prescaleCounter = prescaler;
            }
        } else {
            theLog.snprintf("skipped\n");
            prescaleCounter--;
        }
    } else {
        theLog.snprintf("inactive\n");
    }
}

float sensor::read() {
    switch (type) {
        case measurementChannel::batteryLevel:
            break;

        case measurementChannel::BME680SensorTemperature:
            bme680::initialize();
            bme680::readTemperature();
            break;

        default:
            break;
    }
    return static_cast<float>(type);        // TODO : remove, test only
}

void sensor::sleep() {
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