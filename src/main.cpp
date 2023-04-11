#include "main.h"
#include <stdint.h>
#include <stdio.h>        // for snprintf()...
#include "logging.h"
#include "eventbuffer.h"
#include "applicationevent.h"
#include "lorawanevent.h"
#include "radioevent.h"
#include "lorawan.h"
#include "string.h"
#include "sx126x.h"
#include "maincontroller.h"
#include "power.h"
#include "cli.h"
#include "sensorcollection.h"
#include "nvs.h"

ADC_HandleTypeDef hadc;
I2C_HandleTypeDef hi2c2;
LPTIM_HandleTypeDef hlptim1;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi2;
SUBGHZ_HandleTypeDef hsubghz;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

nonVolatileStorage nvs;
logging theLog;
eventBuffer<radioEvent, 16U> sx126xEventBuffer;
eventBuffer<loRaWanEvent, 16U> loraWanEventBuffer;
eventBuffer<applicationEvent, 16U> applicationEventBuffer;
sx126x theRadio;
LoRaWAN loraNetwork;
mainController theMainController;
power thePowerControl;
cli theCli;
sensorCollection theSensors;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C2_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_LPTIM1_Init(void);
void initializePeripherals(void);

int main(void) {
    HAL_Init();
    initializePeripherals();
    theLog.snprintf("MuMo v2\n");
    theSensors.discover();
    loraNetwork.initialize();              // LoRaWAN layer + the LoRa radio
    theMainController.initialize();        // application

    while (1) {
        thePowerControl.detectUsbConnectOrRemove();
        loraNetwork.handleEvents();
        theMainController.handleEvents();
        if (thePowerControl.isUsbConnected()) {
            theCli.handleRxEvent();
            theCli.handleEvents();
        } else {
            // Here I need a racefree solution to put the MCU into sleep when there are no more events to process..
            // It's not so much a problem that there would be an interrupt pending and the MCU refuses to go into sleep..
            // The problem would be that there are messages waiting in the eventBuffers which would only be served after waking up..
        }
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure LSE Drive Capability
     */
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_8;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief ADC Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC_Init(void) {
    /* USER CODE BEGIN ADC_Init 0 */

    /* USER CODE END ADC_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC_Init 1 */

    /* USER CODE END ADC_Init 1 */

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc.Instance                   = ADC;
    hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
    hadc.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    hadc.Init.LowPowerAutoWait      = DISABLE;
    hadc.Init.LowPowerAutoPowerOff  = DISABLE;
    hadc.Init.ContinuousConvMode    = DISABLE;
    hadc.Init.NbrOfConversion       = 1;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
    hadc.Init.SamplingTimeCommon1   = ADC_SAMPLETIME_160CYCLES_5;
    hadc.Init.SamplingTimeCommon2   = ADC_SAMPLETIME_160CYCLES_5;
    hadc.Init.OversamplingMode      = DISABLE;
    hadc.Init.TriggerFrequencyMode  = ADC_TRIGGER_FREQ_HIGH;
    if (HAL_ADC_Init(&hadc) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
     */
    sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC_Init 2 */

    /* USER CODE END ADC_Init 2 */
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {
    hi2c2.Instance              = I2C2;
    hi2c2.Init.Timing           = 0x00303D5B;
    hi2c2.Init.OwnAddress1      = 0;
    hi2c2.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2      = 0;
    hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c2.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief LPTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPTIM1_Init(void) {
    /* USER CODE BEGIN LPTIM1_Init 0 */

    /* USER CODE END LPTIM1_Init 0 */

    /* USER CODE BEGIN LPTIM1_Init 1 */

    /* USER CODE END LPTIM1_Init 1 */
    hlptim1.Instance             = LPTIM1;
    hlptim1.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    hlptim1.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
    hlptim1.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
    hlptim1.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
    hlptim1.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
    hlptim1.Init.Input1Source    = LPTIM_INPUT1SOURCE_GPIO;
    hlptim1.Init.Input2Source    = LPTIM_INPUT2SOURCE_GPIO;
    if (HAL_LPTIM_Init(&hlptim1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN LPTIM1_Init 2 */

    /* USER CODE END LPTIM1_Init 2 */
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {
    hrtc.Instance            = RTC;
    hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv   = 127;
    hrtc.Init.SynchPrediv    = 255;
    hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
    hrtc.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;
    hrtc.Init.BinMode        = RTC_BINARY_NONE;
    if (HAL_RTC_Init(&hrtc) != HAL_OK) {
        Error_Handler();
    }

    /** Enable the WakeUp
     */
    //	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 61440, RTC_WAKEUPCLOCK_RTCCLK_DIV16,
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 4096, RTC_WAKEUPCLOCK_RTCCLK_DIV16,
                                    0) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {
    /* USER CODE BEGIN SPI2_Init 0 */

    /* USER CODE END SPI2_Init 0 */

    /* USER CODE BEGIN SPI2_Init 1 */

    /* USER CODE END SPI2_Init 1 */
    /* SPI2 parameter configuration*/
    hspi2.Instance               = SPI2;
    hspi2.Init.Mode              = SPI_MODE_MASTER;
    hspi2.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize          = SPI_DATASIZE_4BIT;
    hspi2.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial     = 7;
    hspi2.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi2.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&hspi2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI2_Init 2 */

    /* USER CODE END SPI2_Init 2 */
}

/**
 * @brief SUBGHZ Initialization Function
 * @param None
 * @retval None
 */
void MX_SUBGHZ_Init(void) {
    /* USER CODE BEGIN SUBGHZ_Init 0 */

    /* USER CODE END SUBGHZ_Init 0 */

    /* USER CODE BEGIN SUBGHZ_Init 1 */

    /* USER CODE END SUBGHZ_Init 1 */
    hsubghz.Init.BaudratePrescaler = SUBGHZSPI_BAUDRATEPRESCALER_2;
    if (HAL_SUBGHZ_Init(&hsubghz) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SUBGHZ_Init 2 */

    /* USER CODE END SUBGHZ_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {
    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance                    = USART1;
    huart1.Init.BaudRate               = 115200;
    huart1.Init.WordLength             = UART_WORDLENGTH_8B;
    huart1.Init.StopBits               = UART_STOPBITS_1;
    huart1.Init.Parity                 = UART_PARITY_NONE;
    huart1.Init.Mode                   = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {
    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance                    = USART2;
    huart2.Init.BaudRate               = 115200;
    huart2.Init.WordLength             = UART_WORDLENGTH_8B;
    huart2.Init.StopBits               = UART_STOPBITS_1;
    huart2.Init.Parity                 = UART_PARITY_NONE;
    huart2.Init.Mode                   = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

    LL_GPIO_ResetOutputPin(GPIOB, writeProtect_Pin | displayDataCommand_Pin | displayChipSelect_Pin);
    LL_GPIO_ResetOutputPin(GPIOA, displayReset_Pin | rfControl1_Pin | rfControl2_Pin);

    GPIO_InitStruct.Pin  = usbPowerPresent_Pin | displayBusy_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin        = writeProtect_Pin | displayDataCommand_Pin | displayChipSelect_Pin;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin        = displayReset_Pin | rfControl1_Pin | rfControl2_Pin;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    //
    //	/* GPIO Ports Clock Enable */
    //	__HAL_RCC_GPIOA_CLK_ENABLE();
    //	__HAL_RCC_GPIOB_CLK_ENABLE();
    //	__HAL_RCC_GPIOC_CLK_ENABLE();
    //
    //	/*Configure GPIO pin Output Level */
    //	HAL_GPIO_WritePin(writeProtect_GPIO_Port, writeProtect_Pin, GPIO_PIN_SET); // High = writes protected, Low = writes enabled
    //
    //	/*Configure GPIO pin : vBus_Pin */
    //	GPIO_InitStruct.Pin = vBus_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	HAL_GPIO_Init(vBus_GPIO_Port, &GPIO_InitStruct);
    //
    //	/*Configure GPIO pin : writeProtect_Pin */
    //	GPIO_InitStruct.Pin = writeProtect_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    //	HAL_GPIO_Init(writeProtect_GPIO_Port, &GPIO_InitStruct);
    //
    //	HAL_GPIO_WritePin(RF_SW_CTRL1_GPIO_Port, RF_SW_CTRL1_Pin, GPIO_PIN_RESET);
    //	HAL_GPIO_WritePin(RF_SW_CTRL2_GPIO_Port, RF_SW_CTRL2_Pin, GPIO_PIN_RESET);
    //
    //	/* Configure the Radio Switch pin PA4 */
    //	GPIO_InitStruct.Pin = RF_SW_CTRL1_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    //
    //	HAL_GPIO_Init(RF_SW_CTRL1_GPIO_Port, &GPIO_InitStruct);
    //
    //	/* Configure the Radio Switch pin PA5 */
    //	GPIO_InitStruct.Pin = RF_SW_CTRL2_Pin;
    //	HAL_GPIO_Init(RF_SW_CTRL2_GPIO_Port, &GPIO_InitStruct);
    //
    //	/*Configure GPIO pin Output Level */
    //	HAL_GPIO_WritePin(displayDataCommand_GPIO_Port, displayDataCommand_Pin,
    //			GPIO_PIN_RESET);
    //
    //	/*Configure GPIO pins : displayReset_Pin displayChipSelect_Pin TP3_Pin */
    //	GPIO_InitStruct.Pin = displayReset_Pin | displayChipSelect_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    //	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    //
    //	/*Configure GPIO pin : usbPower_Pin */
    //	GPIO_InitStruct.Pin = usbPower_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	HAL_GPIO_Init(usbPower_GPIO_Port, &GPIO_InitStruct);
    //
    //	/*Configure GPIO pin : displayDataCommand_Pin */
    //	GPIO_InitStruct.Pin = displayDataCommand_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    //	HAL_GPIO_Init(displayDataCommand_GPIO_Port, &GPIO_InitStruct);
    //
    //	/*Configure GPIO pin : displayBusy_Pin */
    //	GPIO_InitStruct.Pin = displayBusy_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	HAL_GPIO_Init(displayBusy_GPIO_Port, &GPIO_InitStruct);
    //
}

void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

void initializePeripherals(void) {
    SystemClock_Config();
    HAL_Delay(1U);
    MX_GPIO_Init();
    HAL_Delay(1U);
    MX_ADC_Init();
    HAL_Delay(1U);
    MX_I2C2_Init();
    HAL_Delay(1U);
    MX_RTC_Init();
    HAL_Delay(1U);
    MX_SPI2_Init();
    HAL_Delay(1U);
    MX_USART1_UART_Init();
    HAL_Delay(1U);
    MX_USART2_UART_Init();
    HAL_Delay(1U);
    MX_LPTIM1_Init();
    HAL_Delay(1U);
    MX_SUBGHZ_Init();
    HAL_Delay(1U);
}
