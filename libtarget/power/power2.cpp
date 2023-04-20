#include "power.h"
#include "eventbuffer.h"
#include "applicationevent.h"
#include "main.h"


extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;
extern ADC_HandleTypeDef hadc;

bool power::usbPower{false};

bool power::isUsbConnected() {
    return usbPower;
}

void power::detectUsbConnectOrRemove() {
    bool currentUsbPower = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, usbPowerPresent_Pin));
    if (currentUsbPower) {
        if (!usbPower) {
            applicationEventBuffer.push(applicationEvent::usbConnected);
        }
    } else {
        if (usbPower) {
            applicationEventBuffer.push(applicationEvent::usbRemoved);
        }
    }
    usbPower = currentUsbPower;
}

void power::measureBatteryLevel() {
    // Now some code to test the ADC conversion..
    HAL_ADCEx_Calibration_Start(&hadc);        // calibrate the ADC after power in

    ADC_ChannelConfTypeDef theAdcConfig;
    theAdcConfig.Channel      = ADC_CHANNEL_VREFINT;
    theAdcConfig.Rank         = ADC_REGULAR_RANK_1;
    theAdcConfig.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc, &theAdcConfig);
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);
    uint32_t adcRawResult = HAL_ADC_GetValue(&hadc);
    voltageInMillivolt    = __HAL_ADC_CALC_VREFANALOG_VOLTAGE((adcRawResult), ADC_RESOLUTION_12B);
}

uint32_t power::calculateVoltageFromRaw(uint32_t adcRaw) {
    // return (VREFINT_CAL_VREF * 1510) / adcRaw;
    return 0;
}
uint32_t power::calculatePercentFromVoltage(uint32_t voltage) {
    return 0;
}

uint32_t power::getBatteryVoltage() {
    return voltageInMillivolt;
}

uint32_t power::getBatteryPercent() {
    return percentCharged;
}
