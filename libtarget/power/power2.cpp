#include "power.h"
#include "eventbuffer.h"
#include "applicationevent.h"
#include "main.h"

//#include "stm32wle5.h"
//extern stm32wleAdc adc;
extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;
//extern peripheralRegister PORTB_IDR;

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

void power::measureBatteryLevel(){
// 1. Sample the internal voltage reference
// 2. Calculate the raw value into a voltage
// 3. Calculate the % charged

//     ADC_ChannelConfTypeDef channelConfig = {0};

// 	// sample the internal bandgap reference
//     channelConfig.Channel =  ADC_CHANNEL_VREFINT;
//     channelConfig.Rank = 1;
//     channelConfig.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
//     if (HAL_OK != HAL_ADC_ConfigChannel(&hadc, &channelConfig))     {    	Error_Handler();    }
//     // Hard hack to set channel correct...
//     #define mADC_CHSELR ((uint32_t*)(0x40012400 + 0x28))
//     *mADC_CHSELR = 0xFFFFFFFD;


//     uint16_t results[16];

//     for (uint32_t i=0; i<16; i++)
//     {
//         if (HAL_OK != HAL_ADC_Start(&hadc)){
//         	Error_Handler();
//         }
// //#define mADC_CR ((uint32_t*)(0x40012400 + 0x08))
//         //uint32_t mCR = *mADC_CR;

//     	if (HAL_OK != HAL_ADC_PollForConversion(&hadc, 10))
//     	{
//     		Error_Handler();
//     	}

//   results[i] =   		HAL_ADC_GetValue(&hadc);


//     }


// 	uint32_t CAL = (uint32_t)(*VREFINT_CAL_ADDR);

// 	uint32_t vrefint = (3300 * CAL) / 4096; 		// VREFINT_CAL_VREF; = 3300

// //uint32_t tensOfMillivolt = (330 * MEAS * CAL) / (REFINT * 4096);



// 	//voltageInMillivolt = calculateVoltageFromRaw(MEAS);
// 	percentCharged = calculatePercentFromVoltage(vrefint);

}


uint32_t power::calculateVoltageFromRaw(uint32_t adcRaw){
		//return (VREFINT_CAL_VREF * 1510) / adcRaw;
	return 0;
}
uint32_t power::calculatePercentFromVoltage(uint32_t voltage){
	return 0;
}


uint32_t power::getBatteryVoltage()
{
	return voltageInMillivolt;
}

uint32_t power::getBatteryPercent(){
	return percentCharged;
}
