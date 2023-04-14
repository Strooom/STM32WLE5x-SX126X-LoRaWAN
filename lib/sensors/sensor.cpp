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

    if (prescaleCounter > prescaler) {        // when switching between low power and high power mode, the prescaleCounter could need to be reset in the appropriate range
        prescaleCounter = prescaler;
    }

    if (oversamplingCounter > oversampling) {        // when switching between low power and high power mode, the oversamplingCounter could need to be reset in the appropriate range
        oversamplingCounter = oversampling;
    }

    if (prescaleCounter == 0) {
        theLog.snprintf("skipped\n");
    } else {
        sample[oversamplingCounter] = read();        // take a new sample for this sensor and store it in the array of samples
        if (oversamplingCounter == 0) {
            theLog.snprintf("measurement\n");
            // average all samples & ouput this measurement to NVS
            oversamplingCounter = oversampling;
        } else {
            theLog.snprintf("sampled\n");
            oversamplingCounter--;
            prescaleCounter = prescaler;
        }
    }
}

float sensor::read() {
    switch (type) {
        case measurementChannel::batteryLevel:
            break;

        case measurementChannel::BME680SensorTemperature:
            // bme680::initialize();
            // bme680::readTemperature();
            // bme680::goSleep();
            break;

        default:
            break;
    }
    return 0.123F;        // TODO : remove, test only
}
