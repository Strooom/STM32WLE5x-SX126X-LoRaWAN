#include "sensor.h"
#include "power.h"

// List of all sensors we know about

#include "bme680.h"
#include "tsl2591.h"
#include "main.h"
static void MX_ADC_Init(void);
extern ADC_HandleTypeDef hadc;

sensor::runResult sensor::run() {
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
            prescaleCounter              = prescaler;
            lastSample                   = read();            // take a new sample for this sensor and ...
            samples[oversamplingCounter] = lastSample;        // ... store it in the array of samples
            if (oversamplingCounter == 0) {
                lastMeasurement     = average(oversampling + 1);        // average all samples & output this measurement to NVS
                oversamplingCounter = oversampling;
                return runResult::measured;
            } else {
                oversamplingCounter--;
                prescaleCounter = prescaler;
                return runResult::sampled;
            }
        } else {
            prescaleCounter--;
            return runResult::prescaled;
        }
    } else {
        return runResult::inactive;
    }
}

float sensor::read() {
    switch (type) {
        case measurementChannel::batteryLevel:
        	MX_ADC_Init();
            power::measureBatteryLevel();
            return power::getBatteryVoltage();
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

float sensor::average(uint32_t nmbrOfSamples) {
    float sum{0.0F};
    for (uint32_t i = 0; i < nmbrOfSamples; i++) {
        sum += samples[i];
    }
    return (sum / nmbrOfSamples);
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


static void MX_ADC_Init(void) {
	/* USER CODE BEGIN ADC_Init 0 */

	/* USER CODE END ADC_Init 0 */

	/* USER CODE BEGIN ADC_Init 1 */

	/* USER CODE END ADC_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc.Instance = ADC;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = ENABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_79CYCLES_5;
	hadc.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_79CYCLES_5;
	hadc.Init.OversamplingMode = DISABLE;
	hadc.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_8;
	hadc.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_NONE;
	hadc.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	hadc.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
	if (HAL_ADC_Init(&hadc) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC_Init 2 */

	/* USER CODE END ADC_Init 2 */
}
