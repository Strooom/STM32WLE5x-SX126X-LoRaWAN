#include "power.h"

bool power::usbPower{false};
uint32_t power::batteryPercentCharged{0};
uint32_t power::voltageInMillivolt{0};

bool power::hasUsbPower() {
    return usbPower;
}

uint32_t power::calculatePercentFromVoltage(uint32_t voltage) {
    return 0;
}

uint32_t power::getBatteryVoltage() {
    return voltageInMillivolt;
}

uint32_t power::getBatteryPercent() {
    return batteryPercentCharged;
}

#ifndef environment_desktop

#include "main.h"

extern ADC_HandleTypeDef hadc;

bool power::isUsbConnected() {
    bool newUsbPower = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, usbPowerPresent_Pin));
    if (newUsbPower && !usbPower) {
        usbPower = true;
        return true;
    } else {
        return false;
    }
}

bool power::isUsbRemoved() {
    bool newUsbPower = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, usbPowerPresent_Pin));
    if (!newUsbPower && usbPower) {
        usbPower = false;
        return true;
    } else {
        return false;
    }
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

#else

bool power::isUsbConnected() {
    return false;
}

bool power::isUsbRemoved() {
    return false;
}

void power::measureBatteryLevel() {
}

#endif