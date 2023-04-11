// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "stm32wle5.h"
#include "stm32wle_adc.h"
#include <stdint.h>


// #############################
// ### ADC                   ###
// ### RM0461 Page 427       ###
// #############################

peripheralRegister ADC_ISR(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x00));
peripheralRegister ADC_IER(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x04));
peripheralRegister ADC_CR(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x08));
peripheralRegister ADC_CFGR1(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x0C));
peripheralRegister ADC_CFGR2(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x10));
peripheralRegister ADC_SMPR(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x14));

peripheralRegister ADC_CHSELR(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x28));
peripheralRegister ADC_DR(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x40));
peripheralRegister ADC_CCR(reinterpret_cast<volatile uint32_t *const>(ADC_BASE + 0x308));

extern peripheralRegister RCC_APB2ENR;

// #define ADC_TR1 (*(volatile uint32_t *)ADC_BASE + 0x20)            // ADC watchdog threshold register
// #define ADC_TR2 (*(volatile uint32_t *)ADC_BASE + 0x24)            // ADC watchdog threshold register
// #define ADC_TR3 (*(volatile uint32_t *)ADC_BASE + 0x2C)            // ADC watchdog threshold register
// #define ADC_AWD2CR (*(volatile uint32_t *)ADC_BASE + 0xA0)         // ADC analog watchdog 2 configuration register
// #define ADC_AWD3CR (*(volatile uint32_t *)ADC_BASE + 0xA4)         // ADC analog watchdog 3 configuration register
// #define ADC_CALFACT (*(volatile uint32_t *)ADC_BASE + 0xB4)        // ADC calibration factors

void stm32wleAdc::initialize() {
    RCC_APB2ENR.setBit(9);        // enable clock towards ADC peripheral
    // TODO : Need some wait here..

    ADC_CR.write(0x00);
    ADC_CR.setBit(ADVREGEN);        // enable voltage regulator
    ADC_CR.setBit(ADCAL);           // start calibration
    while (!ADC_ISR.readBit(ADCAL)) {
    }        // wait for calibration to complete

    ADC_CFGR1.clearBit(CHSELRMOD);             // CHSELRMOD = 0 : select channels by setting a single bit in ADC_CHSELR
    ADC_CHSELR.setBit(VREFINT_CHANNEL);        // select channel 13 = internal voltage reference
    while (!ADC_ISR.readBit(VREFINT_CHANNEL)) {
    };        // CCRDY : Wait for confirmation by Channel Configuration Ready flag

    // ADC_ISR : defaults are OK
    // ADC_IER : defaults are OK

    ADC_CFGR2.write(0x9U);        // Enable 8x oversampling mode TODO : check if clock is OK
    ADC_SMPR.write(0x07);         // Sampling time = 160.5 ADC Clock Cycles

    ADC_CR.write(ADEN);        // Only at the end of the initialization, enable the ADC

//ADC_CCR.write(); // ADC Prescaler
//ADC_CCR.setBit(VREFEN);

    // MODIFY_REG(ADCx->CR,
    //            ADC_CR_BITS_PROPERTY_RS,
    //            ADC_CR_ADVREGEN);

    // wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US / 10UL) * ((SystemCoreClock / (100000UL * 2UL)) + 1UL));
    // while (wait_loop_index != 0UL) {
    //     wait_loop_index--;
    // }

    // __STATIC_INLINE uint32_t LL_ADC_IsInternalRegulatorEnabled(ADC_TypeDef * ADCx) {
    //     return ((READ_BIT(ADCx->CR, ADC_CR_ADVREGEN) == (ADC_CR_ADVREGEN)) ? 1UL : 0UL);
    // }

    // ADC_STATE_CLR_SET(hadc->State,
    //                   HAL_ADC_STATE_REG_BUSY,
    //                   HAL_ADC_STATE_BUSY_INTERNAL);

    // tmpCFGR1 |= (hadc->Init.Resolution |
    //              ADC_CFGR1_AUTOWAIT((uint32_t)hadc->Init.LowPowerAutoWait) |
    //              ADC_CFGR1_AUTOOFF((uint32_t)hadc->Init.LowPowerAutoPowerOff) |
    //              ADC_CFGR1_CONTINUOUS((uint32_t)hadc->Init.ContinuousConvMode) |
    //              ADC_CFGR1_OVERRUN(hadc->Init.Overrun) |
    //              hadc->Init.DataAlign |
    //              ADC_SCAN_SEQ_MODE(hadc->Init.ScanConvMode) |
    //              ADC_CFGR1_DMACONTREQ((uint32_t)hadc->Init.DMAContinuousRequests));

    // ADC_CR.setBit(ADVREGEN);        // enable ADC voltage regulator
    // ADC_CR.setBit(ADEN);            // enable ADC

    //

    //     hadc.Instance                   = ADC;
    //     hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;        // ; //
    //     hadc.Init.Resolution            = ADC_RESOLUTION_12B;
    //     hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    //     hadc.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    //     hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    //     hadc.Init.LowPowerAutoWait      = DISABLE;
    //     hadc.Init.LowPowerAutoPowerOff  = DISABLE;
    //     hadc.Init.ContinuousConvMode    = DISABLE;
    //     hadc.Init.NbrOfConversion       = 1;
    //     hadc.Init.DiscontinuousConvMode = DISABLE;
    //     hadc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    //     hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //     hadc.Init.DMAContinuousRequests = DISABLE;
    //     hadc.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
    //     hadc.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_160CYCLES_5;
    //     hadc.Init.SamplingTimeCommon2   = ADC_SAMPLETIME_160CYCLES_5;
    //     hadc.Init.OversamplingMode      = DISABLE;
    //     hadc.Init.TriggerFrequencyMode  = ADC_TRIGGER_FREQ_HIGH;
    //     if (HAL_ADC_Init(&hadc) != HAL_OK) {
    //         Error_Handler();
    //     }

    //     /** Configure Regular Channel
    //      */
    //     ADC_ChannelConfTypeDef sConfig;

    //     sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;
    //     sConfig.Rank         = ADC_REGULAR_RANK_1;
    //     sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    //     if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    //         Error_Handler();
    //     }
}

void stm32wleAdc::prepareForSleep() {
    // ADC_CR.clearBit(ADC_CR_ADEN);        // disable ADC
    // ADC_CR.setBit(ADC_CR_ADEN);          // enable ADC
}
void stm32wleAdc::restoreAfterSleep() {
    initialize();
}

void stm32wleAdc::selectChannel(adcChannel aChannel) {
    // write the bits in the correct register..
    // activate the voltage reference / temp sensor
    // wait some uSeconds to have the source ready to be sampled...
    // when selecting channel 'none' -> power down all these sources..
}

uint32_t stm32wleAdc::convert() {
    // start conversion
    while (!ADC_ISR.readBit(2)) {
        // wait for ADC conversion
    }
}

bool stm32wleAdc::isReady() const {
    return ADC_ISR.readBit(0);
}
