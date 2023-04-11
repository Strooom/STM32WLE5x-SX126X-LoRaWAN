

void start() {
    // 1. Configure Clock and central MCU
    peripheralRegister _PWR_CR1(reinterpret_cast<volatile uint32_t *const>(0x58000094U));
    peripheralRegister _RCC_BDCR(reinterpret_cast<volatile uint32_t *const>(0x58000094U));

    // 1.1 If not yet enabled, enable external XTAL for RTC - This should normally only be done on boot after power on, as this setting is retained after reset.
    if (!_RCC_BDCR.readBit(15)) {
        _PWR_CR1.setBit(8);          // Enable access to backup domain
        _RCC_BDCR.setBit(0);         // Enable LSE
        _RCC_BDCR.setBit(3);         // Set LSE drive to low
        _RCC_BDCR.setBit(8);         // Set LSE as RTC clock source
        _RCC_BDCR.setBit(15);        // Enable RTC

        // remember that time/date are still invalid
    }
    // 1.2 Select voltage scaling - as we are keeping things simple, we run on 16 MHz MSI, but we still set Core Voltage to 1.2V (= higher value)
    // Later we can experiment with consuming less power and running on 1.0V
    _PWR_CR1.setBit(9);        // Set Voltage scaling to 1.2V

    // 1.3 Now configure the Oscillator
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_8;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;

    // 1.4 Now configure all clocks
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

    // Also set ADC clock
    MODIFY_REG(RCC->CCIPR, RCC_CCIPR_ADCSEL, ADCxSource);


    // Configure the RTC
    __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);

  /* Clear WUTE in RTC_CR to disable the wakeup timer */
  CLEAR_BIT(RTC->CR, RTC_CR_WUTE);

  /* Clear flag Wake-Up */
  WRITE_REG(RTC->SCR, RTC_SCR_CWUTF);

  /* Poll WUTWF until it is set in RTC_ICSR to make sure the access to wakeup autoreload
     counter and to WUCKSEL[2:0] bits is allowed. This step must be skipped in
     calendar initialization mode. */
  if (READ_BIT(RTC->ICSR, RTC_ICSR_INITF) == 0U)
  {
    tickstart = HAL_GetTick();
    while (READ_BIT(RTC->ICSR, RTC_ICSR_WUTWF) == 0U)
    {
      if ((HAL_GetTick() - tickstart) > RTC_TIMEOUT_VALUE)
      {
        /* Enable the write protection for RTC registers */
        __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);

        hrtc->State = HAL_RTC_STATE_TIMEOUT;

        /* Process Unlocked */
        __HAL_UNLOCK(hrtc);

        return HAL_TIMEOUT;
      }
    }
  }

  /* Configure the Wakeup Timer counter and auto clear value */
  WRITE_REG(RTC->WUTR, (uint32_t)(WakeUpCounter | (WakeUpAutoClr << RTC_WUTR_WUTOCLR_Pos)));

  /* Configure the clock source */
  MODIFY_REG(RTC->CR, RTC_CR_WUCKSEL, (uint32_t)WakeUpClock);

  /* In case of WUT autoclr, the IRQ handler should not be called */
  if (WakeUpAutoClr != 0U)
  {
    /* RTC WakeUpTimer EXTI Configuration: Event configuration */
    __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_EVENT();
  }
  else
  {
    /* RTC WakeUpTimer EXTI Configuration: Interrupt configuration */
    __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT();
  }

  /* Configure the Interrupt in the RTC_CR register and Enable the Wakeup Timer*/
  SET_BIT(RTC->CR, (RTC_CR_WUTIE | RTC_CR_WUTE));

  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);


// Enabling / Disabling clocks to GPIO ports A and B (C is currently not used)
#define __HAL_RCC_GPIOA_CLK_ENABLE()                LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define __HAL_RCC_GPIOB_CLK_ENABLE()                LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define __HAL_RCC_GPIOA_CLK_DISABLE()               LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define __HAL_RCC_GPIOB_CLK_DISABLE()               LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOB)

peripheralRegister _RCC_AHB2ENR(reinterpret_cast<volatile uint32_t *const>(0x58000094U));
_RCC_AHB2ENR.setBit(0); // Enable GPIOA
_RCC_AHB2ENR.setBit(1); // Enable GPIOB
// Wait some delay here, until the clock is running stable, and the further configuration can be done

// Pins to be configured :
// vBus : input : sensing of presence of USB power
// writeProtect : output - for external EEPROM
// displayReset : output
// displayDataCommand : output
// displayChipSelect : ouput
// displayBusy : input

// Then some more IOs need to be configured with their Alternate functions
// SPI2 for display
// UART1 and UART2
// I2C2 for sensors and memory..


    HAL_GPIO_WritePin(writeProtect_GPIO_Port, writeProtect_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : vBus_Pin */
    GPIO_InitStruct.Pin  = vBus_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(vBus_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : writeProtect_Pin */
    GPIO_InitStruct.Pin   = writeProtect_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(writeProtect_GPIO_Port, &GPIO_InitStruct);


}