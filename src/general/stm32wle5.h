// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

// This is a header file with definitions of all registers / peripherals / etc. of the STM32WLE5 MCU.
// ST defines this is stm32wle5xx.h, but in a style which is harder to read and use.
// So I include this stm32wle5xx.h file here, and derive some new definitions from it which are simpler to use.

#pragma once
#include <stdint.h>

// #########################
// ### Cortex M4 General ###
// #########################

enum class stm32wle5xxInterruptSource : int8_t {
    // Cortex-M4 Processor Exceptions Numbers
    NonMaskableInt_IRQn   = -14,        //  Non Maskable Interrupt                                            */
    HardFault_IRQn        = -13,        //  Cortex-M4 Hard Fault Interrupt                                    */
    MemoryManagement_IRQn = -12,        //  Cortex-M4 Memory Management Interrupt                             */
    BusFault_IRQn         = -11,        //  Cortex-M4 Bus Fault Interrupt                                     */
    UsageFault_IRQn       = -10,        //  Cortex-M4 Usage Fault Interrupt                                   */
    SVCall_IRQn           = -5,         //  Cortex-M4 SV Call Interrupt                                       */
    DebugMonitor_IRQn     = -4,         //  Cortex-M4 Debug Monitor Interrupt                                 */
    PendSV_IRQn           = -2,         //  Cortex-M4 Pend SV Interrupt                                       */
    SysTick_IRQn          = -1,         //  Cortex-M4 System Tick Interrupt                                   */

    //  STM32WLxx specific Interrupt Numbers on M4 core
    WWDG_IRQn                   = 0,         //  Window WatchDog Interrupt                                         */
    PVD_PVM_IRQn                = 1,         //  PVD and PVM detector                                              */
    TAMP_STAMP_LSECSS_SSRU_IRQn = 2,         //  RTC Tamper, RTC TimeStamp, LSECSS and RTC SSRU Interrupts         */
    RTC_WKUP_IRQn               = 3,         //  RTC Wakeup Interrupt                                              */
    FLASH_IRQn                  = 4,         //  FLASH (CFI)  global Interrupt                                     */
    RCC_IRQn                    = 5,         //  RCC Interrupt                                                     */
    EXTI0_IRQn                  = 6,         //  EXTI Line 0 Interrupt                                             */
    EXTI1_IRQn                  = 7,         //  EXTI Line 1 Interrupt                                             */
    EXTI2_IRQn                  = 8,         //  EXTI Line 2 Interrupt                                             */
    EXTI3_IRQn                  = 9,         //  EXTI Line 3 Interrupt                                             */
    EXTI4_IRQn                  = 10,        //  EXTI Line 4 Interrupt                                             */
    DMA1_Channel1_IRQn          = 11,        //  DMA1 Channel 1 Interrupt                                          */
    DMA1_Channel2_IRQn          = 12,        //  DMA1 Channel 2 Interrupt                                          */
    DMA1_Channel3_IRQn          = 13,        //  DMA1 Channel 3 Interrupt                                          */
    DMA1_Channel4_IRQn          = 14,        //  DMA1 Channel 4 Interrupt                                          */
    DMA1_Channel5_IRQn          = 15,        //  DMA1 Channel 5 Interrupt                                          */
    DMA1_Channel6_IRQn          = 16,        //  DMA1 Channel 6 Interrupt                                          */
    DMA1_Channel7_IRQn          = 17,        //  DMA1 Channel 7 Interrupt                                          */
    ADC_IRQn                    = 18,        //  ADC Interrupt                                                     */
    DAC_IRQn                    = 19,        //  DAC Interrupt                                                     */
    COMP_IRQn                   = 21,        //  COMP1 and COMP2 Interrupts                                        */
    EXTI9_5_IRQn                = 22,        //  EXTI Lines [9:5] Interrupt                                        */
    TIM1_BRK_IRQn               = 23,        //  TIM1 Break Interrupt                                              */
    TIM1_UP_IRQn                = 24,        //  TIM1 Update Interrupt                                             */
    TIM1_TRG_COM_IRQn           = 25,        //  TIM1 Trigger and Communication Interrupts                         */
    TIM1_CC_IRQn                = 26,        //  TIM1 Capture Compare Interrupt                                    */
    TIM2_IRQn                   = 27,        //  TIM2 Global Interrupt                                             */
    TIM16_IRQn                  = 28,        //  TIM16 Global Interrupt                                            */
    TIM17_IRQn                  = 29,        //  TIM17 Global Interrupt                                            */
    I2C1_EV_IRQn                = 30,        //  I2C1 Event Interrupt                                              */
    I2C1_ER_IRQn                = 31,        //  I2C1 Error Interrupt                                              */
    I2C2_EV_IRQn                = 32,        //  I2C2 Event Interrupt                                              */
    I2C2_ER_IRQn                = 33,        //  I2C2 Error Interrupt                                              */
    SPI1_IRQn                   = 34,        //  SPI1 Interrupt                                                    */
    SPI2_IRQn                   = 35,        //  SPI2 Interrupt                                                    */
    USART1_IRQn                 = 36,        //  USART1 Interrupt                                                  */
    USART2_IRQn                 = 37,        //  USART2 Interrupt                                                  */
    LPUART1_IRQn                = 38,        //  LPUART1 Interrupt                                                 */
    LPTIM1_IRQn                 = 39,        //  LPTIM1 Global Interrupt                                           */
    LPTIM2_IRQn                 = 40,        //  LPTIM2 Global Interrupt                                           */
    EXTI15_10_IRQn              = 41,        //  EXTI Lines [15:10] Interrupt                                      */
    RTC_Alarm_IRQn              = 42,        //  RTC Alarms (A and B) Interrupt                                    */
    LPTIM3_IRQn                 = 43,        //  LPTIM3 Global Interrupt                                           */
    SUBGHZSPI_IRQn              = 44,        //  SUBGHZSPI Interrupt                                               */
    HSEM_IRQn                   = 47,        //  HSEM Interrupt                                                    */
    I2C3_EV_IRQn                = 48,        //  I2C3 Event Interrupt                                              */
    I2C3_ER_IRQn                = 49,        //  I2C3 Error Interrupt                                              */
    SUBGHZ_Radio_IRQn           = 50,        //  SUBGHZ Radio Interrupt                                            */
    AES_IRQn                    = 51,        //  AES Interrupt                                                     */
    RNG_IRQn                    = 52,        //  RNG Interrupt                                                     */
    PKA_IRQn                    = 53,        //  PKA Interrupt                                                     */
    DMA2_Channel1_IRQn          = 54,        //  DMA2 Channel 1 Interrupt                                          */
    DMA2_Channel2_IRQn          = 55,        //  DMA2 Channel 2 Interrupt                                          */
    DMA2_Channel3_IRQn          = 56,        //  DMA2 Channel 3 Interrupt                                          */
    DMA2_Channel4_IRQn          = 57,        //  DMA2 Channel 4 Interrupt                                          */
    DMA2_Channel5_IRQn          = 58,        //  DMA2 Channel 5 Interrupt                                          */
    DMA2_Channel6_IRQn          = 59,        //  DMA2 Channel 6 Interrupt                                          */
    DMA2_Channel7_IRQn          = 60,        //  DMA2 Channel 7 Interrupt                                          */
    DMAMUX1_OVR_IRQn            = 61         //  DMAMUX1 overrun Interrupt                                         */
};

// #############################
// ### STM32WLE5 Peripherals ###
// #############################

#define FLASH_BASE        0x08000000UL        //  FLASH(up to 256 KB) base address */
#define SYSTEM_FLASH_BASE 0x1FFF0000UL        //  System FLASH(28Kb) base address */
#define SRAM1_BASE        0x20000000UL        //  SRAM1(up to 32 KB) base address */
#define SRAM2_BASE        0x20008000UL        //  SRAM2(up to 32 KB) base address */
#define PERIPH_BASE       0x40000000UL        //  Peripheral base address */

#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000UL)
#define AHB3PERIPH_BASE (PERIPH_BASE + 0x18000000UL)
#define APB3PERIPH_BASE (PERIPH_BASE + 0x18010000UL)

//  APB1 peripherals */
#define TIM2_BASE    (APB1PERIPH_BASE + 0x00000000UL)
#define RTC_BASE     (APB1PERIPH_BASE + 0x00002800UL)
#define WWDG_BASE    (APB1PERIPH_BASE + 0x00002C00UL)
#define IWDG_BASE    (APB1PERIPH_BASE + 0x00003000UL)
#define SPI2_BASE    (APB1PERIPH_BASE + 0x00003800UL)
#define USART2_BASE  (APB1PERIPH_BASE + 0x00004400UL)
#define I2C1_BASE    (APB1PERIPH_BASE + 0x00005400UL)
#define I2C2_BASE    (APB1PERIPH_BASE + 0x00005800UL)
#define I2C3_BASE    (APB1PERIPH_BASE + 0x00005C00UL)
#define DAC_BASE     (APB1PERIPH_BASE + 0x00007400UL)
#define LPTIM1_BASE  (APB1PERIPH_BASE + 0x00007C00UL)
#define LPUART1_BASE (APB1PERIPH_BASE + 0x00008000UL)
#define LPTIM2_BASE  (APB1PERIPH_BASE + 0x00009400UL)
#define LPTIM3_BASE  (APB1PERIPH_BASE + 0x00009800UL)
#define TAMP_BASE    (APB1PERIPH_BASE + 0x0000B000UL)

//  APB2 peripherals */
#define SYSCFG_BASE     (APB2PERIPH_BASE + 0x00000000UL)
#define VREFBUF_BASE    (APB2PERIPH_BASE + 0x00000030UL)
#define COMP1_BASE      (APB2PERIPH_BASE + 0x00000200UL)
#define COMP2_BASE      (APB2PERIPH_BASE + 0x00000204UL)
#define ADC_BASE        (APB2PERIPH_BASE + 0x00002400UL)
#define ADC_COMMON_BASE (APB2PERIPH_BASE + 0x00002708UL)
#define TIM1_BASE       (APB2PERIPH_BASE + 0x00002C00UL)
#define SPI1_BASE       (APB2PERIPH_BASE + 0x00003000UL)
#define USART1_BASE     (APB2PERIPH_BASE + 0x00003800UL)
#define TIM16_BASE      (APB2PERIPH_BASE + 0x00004400UL)
#define TIM17_BASE      (APB2PERIPH_BASE + 0x00004800UL)

//  AHB1 peripherals */
#define DMA1_BASE    (AHB1PERIPH_BASE + 0x00000000UL)
#define DMA2_BASE    (AHB1PERIPH_BASE + 0x00000400UL)
#define DMAMUX1_BASE (AHB1PERIPH_BASE + 0x00000800UL)
#define CRC_BASE     (AHB1PERIPH_BASE + 0x00003000UL)

//  AHB2 peripherals */
#define IOPORT_BASE (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOA_BASE  (IOPORT_BASE + 0x00000000UL)
#define GPIOB_BASE  (IOPORT_BASE + 0x00000400UL)
#define GPIOC_BASE  (IOPORT_BASE + 0x00000800UL)
#define GPIOH_BASE  (IOPORT_BASE + 0x00001C00UL)

//  AHB3 peripherals */
#define PWR_BASE       (AHB3PERIPH_BASE + 0x00000400UL)
#define EXTI_BASE      (AHB3PERIPH_BASE + 0x00000800UL)
#define RCC_BASE       (AHB3PERIPH_BASE + 0x00000000UL)
#define RNG_BASE       (AHB3PERIPH_BASE + 0x00001000UL)
#define HSEM_BASE      (AHB3PERIPH_BASE + 0x00001400UL)
#define AES_BASE       (AHB3PERIPH_BASE + 0x00001800UL)
#define PKA_BASE       (AHB3PERIPH_BASE + 0x00002000UL)
#define FLASH_REG_BASE (AHB3PERIPH_BASE + 0x00004000UL)

//  APB3 peripherals */
#define SUBGHZSPI_BASE (APB3PERIPH_BASE + 0x00000000UL)

//  Peripherals available on CPU1 external PPB bus */
#define DBGMCU_BASE (0xE0042000UL)

// #############################
// ### ADC                   ###
// ### RM0461 Page 427       ###
// #############################

#define ADC_ISR     (*(volatile uint32_t *)ADC_BASE + 0x00)        // ADC interrupt and status register
#define ADC_IER     (*(volatile uint32_t *)ADC_BASE + 0x04)        // ADC interrupt enable register
#define ADC_CR      (*(volatile uint32_t *)ADC_BASE + 0x08)        // ADC control register
#define ADC_CFGR1   (*(volatile uint32_t *)ADC_BASE + 0x0C)        // ADC configuration register 1
#define ADC_CFGR2   (*(volatile uint32_t *)ADC_BASE + 0x10)        // ADC configuration register 2
#define ADC_SMPR    (*(volatile uint32_t *)ADC_BASE + 0x14)        // ADC sampling time register
#define ADC_TR1     (*(volatile uint32_t *)ADC_BASE + 0x20)        // ADC watchdog threshold register
#define ADC_TR2     (*(volatile uint32_t *)ADC_BASE + 0x24)        // ADC watchdog threshold register
#define ADC_CHSELR  (*(volatile uint32_t *)ADC_BASE + 0x28)        // ADC group regular sequencer register
#define ADC_TR3     (*(volatile uint32_t *)ADC_BASE + 0x2C)        // ADC watchdog threshold register
#define ADC_DR      (*(volatile uint32_t *)ADC_BASE + 0x40)        // ADC Data register
#define ADC_AWD2CR  (*(volatile uint32_t *)ADC_BASE + 0xA0)        // ADC analog watchdog 2 configuration register
#define ADC_AWD3CR  (*(volatile uint32_t *)ADC_BASE + 0xA4)        // ADC analog watchdog 3 configuration register
#define ADC_CALFACT (*(volatile uint32_t *)ADC_BASE + 0xB4)        // ADC calibration factors

// #############################
// ### AES                   ###
// ### RM0461 Page 542       ###
// #############################

#define AES_CR    (*(volatile uint32_t *)AES_BASE + 0x00)        // AES control register
#define AES_SR    (*(volatile uint32_t *)AES_BASE + 0x04)        // AES status register
#define AES_DINR  (*(volatile uint32_t *)AES_BASE + 0x08)        // AES data input register
#define AES_DOUTR (*(volatile uint32_t *)AES_BASE + 0x0C)        // AES data output register
#define AES_KEYR0 (*(volatile uint32_t *)AES_BASE + 0x10)        // AES key registers
#define AES_KEYR1 (*(volatile uint32_t *)AES_BASE + 0x14)        //
#define AES_KEYR2 (*(volatile uint32_t *)AES_BASE + 0x18)        //
#define AES_KEYR3 (*(volatile uint32_t *)AES_BASE + 0x1C)        //
#define AES_KEYR4 (*(volatile uint32_t *)AES_BASE + 0x30)        //
#define AES_KEYR5 (*(volatile uint32_t *)AES_BASE + 0x34)        //
#define AES_KEYR6 (*(volatile uint32_t *)AES_BASE + 0x38)        //
#define AES_KEYR7 (*(volatile uint32_t *)AES_BASE + 0x3C)        //
#define AES_IVR0  (*(volatile uint32_t *)AES_BASE + 0x20)        // AES initialization vector registers
#define AES_IVR1  (*(volatile uint32_t *)AES_BASE + 0x24)        //
#define AES_IVR2  (*(volatile uint32_t *)AES_BASE + 0x28)        //
#define AES_IVR3  (*(volatile uint32_t *)AES_BASE + 0x2C)        //
#define AES_SUSP0 (*(volatile uint32_t *)AES_BASE + 0x40)        // AES suspend registers
#define AES_SUSP1 (*(volatile uint32_t *)AES_BASE + 0x44)        //
#define AES_SUSP2 (*(volatile uint32_t *)AES_BASE + 0x48)        //
#define AES_SUSP3 (*(volatile uint32_t *)AES_BASE + 0x4C)        //
#define AES_SUSP4 (*(volatile uint32_t *)AES_BASE + 0x50)        //
#define AES_SUSP5 (*(volatile uint32_t *)AES_BASE + 0x54)        //
#define AES_SUSP6 (*(volatile uint32_t *)AES_BASE + 0x58)        //
#define AES_SUSP7 (*(volatile uint32_t *)AES_BASE + 0x5C)        //

// #############################
// ### UART                  ###
// ### RM0461 Page 1011      ###
// #############################

#define UART1_CR1   (*(volatile uint32_t *)USART1_BASE + 0x00)        // UART control register 1
#define UART1_CR2   (*(volatile uint32_t *)USART1_BASE + 0x04)        // UART control register 2
#define UART1_CR3   (*(volatile uint32_t *)USART1_BASE + 0x08)        // UART control register 3
#define UART1_BRR   (*(volatile uint32_t *)USART1_BASE + 0x0C)        // UART baud rate register
#define UART1_GTPR  (*(volatile uint32_t *)USART1_BASE + 0x10)        // UART guard time and prescaler register
#define UART1_RTOR  (*(volatile uint32_t *)USART1_BASE + 0x14)        // UART receiver timeout register
#define UART1_RQR   (*(volatile uint32_t *)USART1_BASE + 0x18)        // UART request register
#define UART1_ISR   (*(volatile uint32_t *)USART1_BASE + 0x1C)        // UART interrupt and status register
#define UART1_ICR   (*(volatile uint32_t *)USART1_BASE + 0x20)        // UART interrupt flag clear register
#define UART1_RDR   (*(volatile uint32_t *)USART1_BASE + 0x24)        // UART receive data register
#define UART1_TDR   (*(volatile uint32_t *)USART1_BASE + 0x28)        // UART transmit data register
#define UART1_PRESC (*(volatile uint32_t *)USART1_BASE + 0x2C)        // UART prescaler register

#define UART2_CR1   (*(volatile uint32_t *)USART2_BASE + 0x00)        //
#define UART2_CR2   (*(volatile uint32_t *)USART2_BASE + 0x04)        //
#define UART2_CR3   (*(volatile uint32_t *)USART2_BASE + 0x08)        //
#define UART2_BRR   (*(volatile uint32_t *)USART2_BASE + 0x0C)        //
#define UART2_GTPR  (*(volatile uint32_t *)USART2_BASE + 0x10)        //
#define UART2_RTOR  (*(volatile uint32_t *)USART2_BASE + 0x14)        //
#define UART2_RQR   (*(volatile uint32_t *)USART2_BASE + 0x18)        //
#define UART2_ISR   (*(volatile uint32_t *)USART2_BASE + 0x1C)        //
#define UART2_ICR   (*(volatile uint32_t *)USART2_BASE + 0x20)        //
#define UART2_RDR   (*(volatile uint32_t *)USART2_BASE + 0x24)        //
#define UART2_TDR   (*(volatile uint32_t *)USART2_BASE + 0x28)        //
#define UART2_PRESC (*(volatile uint32_t *)USART2_BASE + 0x2C)        //

// #############################
// ### SPI                   ###
// ### RM0461 Page 1151      ###
// #############################

#define SPI2_CR1     (*(volatile uint32_t *)SPI2_BASE + 0x00)        // SPI control register 1
#define SPI2_CR2     (*(volatile uint32_t *)SPI2_BASE + 0x04)        // SPI control register 2
#define SPI2_SR      (*(volatile uint32_t *)SPI2_BASE + 0x08)        // SPI status register
#define SPI2_DR      (*(volatile uint32_t *)SPI2_BASE + 0x0C)        //  SPI data register
#define SPI2_CRCPR   (*(volatile uint32_t *)SPI2_BASE + 0x10)        // SPI CRC polynomial register
#define SPI2_RXCRCR  (*(volatile uint32_t *)SPI2_BASE + 0x14)        // SPI RX CRC register
#define SPI2_TXCRCR  (*(volatile uint32_t *)SPI2_BASE + 0x18)        // SPI TX CRC register
#define SPI2_I2SCFGR (*(volatile uint32_t *)SPI2_BASE + 0x1C)        // SPI_I2S configuration register
#define SPI2_I2SPR   (*(volatile uint32_t *)SPI2_BASE + 0x20)        // SPI_I2S prescaler register

#define SPI_SX126X__CR1     (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x00)        //
#define SPI_SX126X__CR2     (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x04)        //
#define SPI_SX126X__SR      (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x08)        //
#define SPI_SX126X__DR      (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x0C)        //
#define SPI_SX126X__CRCPR   (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x10)        //
#define SPI_SX126X__RXCRCR  (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x14)        //
#define SPI_SX126X__TXCRCR  (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x18)        //
#define SPI_SX126X__I2SCFGR (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x1C)        //
#define SPI_SX126X__I2SPR   (*(volatile uint32_t *)SUBGHZSPI_BASE + 0x20)        //

// #############################
// ### RTC                   ###
// ### RM0461 Page 884       ###
// #############################

#define RTC_TR       (*(volatile uint32_t *)RTC_BASE + 0x00)        // RTC time register
#define RTC_DR       (*(volatile uint32_t *)RTC_BASE + 0x04)        // RTC date register
#define RTC_SSR      (*(volatile uint32_t *)RTC_BASE + 0x08)        // RTC sub second register
#define RTC_ICSR     (*(volatile uint32_t *)RTC_BASE + 0x0C)        // RTC initialization and status register
#define RTC_PRER     (*(volatile uint32_t *)RTC_BASE + 0x10)        // RTC prescaler register
#define RTC_WUTR     (*(volatile uint32_t *)RTC_BASE + 0x14)        //  RTC wakeup timer register
#define RTC_CR       (*(volatile uint32_t *)RTC_BASE + 0x18)        // RTC control register
#define RTC_WPR      (*(volatile uint32_t *)RTC_BASE + 0x24)        // RTC write protection register
#define RTC_CALR     (*(volatile uint32_t *)RTC_BASE + 0x28)        // RTC calibration register
#define RTC_SHIFTR   (*(volatile uint32_t *)RTC_BASE + 0x2C)        // RTC shift control register
#define RTC_TSTR     (*(volatile uint32_t *)RTC_BASE + 0x30)        // RTC time stamp time register
#define RTC_TSDR     (*(volatile uint32_t *)RTC_BASE + 0x34)        // RTC time stamp date register
#define RTC_TSSSR    (*(volatile uint32_t *)RTC_BASE + 0x38)        // RTC time-stamp sub second register
#define RTC_ALRMAR   (*(volatile uint32_t *)RTC_BASE + 0x40)        // RTC alarm A register
#define RTC_ALRMASSR (*(volatile uint32_t *)RTC_BASE + 0x44)        // RTC alarm A sub second register
#define RTC_ALMRBR   (*(volatile uint32_t *)RTC_BASE + 0x48)        // RTC alarm B register
#define RTC_ALRMBSSR (*(volatile uint32_t *)RTC_BASE + 0x4C)        // RTC alarm B sub second register
#define RTC_SR       (*(volatile uint32_t *)RTC_BASE + 0x50)        // RTC status register
#define RTC_MISR     (*(volatile uint32_t *)RTC_BASE + 0x54)        // RTC masked interrupt status register
#define RTC_SCR      (*(volatile uint32_t *)RTC_BASE + 0x5C)        // RTC status clear register
#define RTC_ALRABINR (*(volatile uint32_t *)RTC_BASE + 0x70)        // RTC alarm A binary register
#define RTC_ALRBBINR (*(volatile uint32_t *)RTC_BASE + 0x74)        // RTC alarm B binary register

// #############################
// ### RCC                   ###
// ### RM0461 Page 230       ###
// #############################

#define RCC_CR         (*(volatile uint32_t *)RCC_BASE + 0x00)         // RCC clock control register
#define RCC_ICSCR      (*(volatile uint32_t *)RCC_BASE + 0x04)         // RCC internal clock sources calibration register
#define RCC_CFGR       (*(volatile uint32_t *)RCC_BASE + 0x08)         // RCC clock configuration register
#define RCC_PLLCFGR    (*(volatile uint32_t *)RCC_BASE + 0x0C)         // RCC system PLL configuration register
#define RCC_CIER       (*(volatile uint32_t *)RCC_BASE + 0x18)         // RCC clock interrupt enable register
#define RCC_CIFR       (*(volatile uint32_t *)RCC_BASE + 0x1C)         // RCC clock interrupt flag register
#define RCC_CICR       (*(volatile uint32_t *)RCC_BASE + 0x20)         // RCC clock interrupt clear register
#define RCC_AHB1RSTR   (*(volatile uint32_t *)RCC_BASE + 0x28)         // RCC AHB1 peripheral reset register
#define RCC_AHB2RSTR   (*(volatile uint32_t *)RCC_BASE + 0x2C)         // RCC AHB2 peripheral reset register
#define RCC_AHB3RSTR   (*(volatile uint32_t *)RCC_BASE + 0x30)         // RCC AHB3 peripheral reset register
#define RCC_APB1RSTR1  (*(volatile uint32_t *)RCC_BASE + 0x38)         // RCC APB1 peripheral reset register 1
#define RCC_APB1RSTR1  (*(volatile uint32_t *)RCC_BASE + 0x3C)         // RCC APB1 peripheral reset register 2
#define RCC_APB2RSTR   (*(volatile uint32_t *)RCC_BASE + 0x40)         // RCC APB2 peripheral reset register
#define RCC_APB3RSTR   (*(volatile uint32_t *)RCC_BASE + 0x44)         // RCC APB3 peripheral reset register
#define RCC_AHB1ENR    (*(volatile uint32_t *)RCC_BASE + 0x48)         // RCC AHB1 peripheral clock enable register
#define RCC_AHB2ENR    (*(volatile uint32_t *)RCC_BASE + 0x4C)         // RCC AHB2 peripheral clock enable register
#define RCC_AHB3ENR    (*(volatile uint32_t *)RCC_BASE + 0x50)         // RCC AHB3 peripheral clock enable register
#define RCC_APB1ENR1   (*(volatile uint32_t *)RCC_BASE + 0x58)         // RCC APB1 peripheral clock enable register 1
#define RCC_APB1ENR2   (*(volatile uint32_t *)RCC_BASE + 0x5C)         // RCC APB1 peripheral clock enable register 2
#define RCC_APB2ENR    (*(volatile uint32_t *)RCC_BASE + 0x60)         // RCC APB2 peripheral clock enable register
#define RCC_APB3ENR    (*(volatile uint32_t *)RCC_BASE + 0x64)         // RCC APB3 peripheral clock enable register
#define RCC_AHB1SMENR  (*(volatile uint32_t *)RCC_BASE + 0x68)         // RCC AHB1 peripheral clocks enable in Sleep and Stop modes register
#define RCC_AHB2SMENR  (*(volatile uint32_t *)RCC_BASE + 0x6C)         // RCC AHB2 peripheral clocks enable in Sleep and Stop modes register
#define RCC_AHB3SMENR  (*(volatile uint32_t *)RCC_BASE + 0x70)         // RCC AHB3 peripheral clocks enable in Sleep and Stop modes register
#define RCC_APB1SMENR1 (*(volatile uint32_t *)RCC_BASE + 0x78)         // RCC APB1 peripheral clocks enable in Sleep and Stop modes register 1
#define RCC_APB1SMENR2 (*(volatile uint32_t *)RCC_BASE + 0x7C)         // RCC APB1 peripheral clocks enable in Sleep and Stop modes register 2
#define RCC_APB2SMENR  (*(volatile uint32_t *)RCC_BASE + 0x80)         // RCC APB2 peripheral clocks enable in Sleep and Stop modes register
#define RCC_APB3SMENR  (*(volatile uint32_t *)RCC_BASE + 0x84)         // RCC APB3 peripheral clocks enable in Sleep and Stop modes register
#define RCC_CCIPR      (*(volatile uint32_t *)RCC_BASE + 0x88)         // RCC peripherals independent clock configuration register
#define RCC_BDCR       (*(volatile uint32_t *)RCC_BASE + 0x90)         // RCC Backup domain control register
#define RCC_CSR        (*(volatile uint32_t *)RCC_BASE + 0x94)         // RCC clock control & status register
#define RCC_EXTCFGR    (*(volatile uint32_t *)RCC_BASE + 0x108)        // RCC extended clock recovery register


// #############################
// ### I2C                   ###
// #############################

#define I2C2_CR1      (*(volatile uint32_t *)I2C2_BASE + 0x00)        // I2C control register 1
#define I2C2_CR2      (*(volatile uint32_t *)I2C2_BASE + 0x04)        // I2C control register 2
#define I2C2_OAR1     (*(volatile uint32_t *)I2C2_BASE + 0x08)        // I2C own address register 1
#define I2C2_OAR2     (*(volatile uint32_t *)I2C2_BASE + 0x0C)        // I2C own address register 2
#define I2C2_TIMINGR  (*(volatile uint32_t *)I2C2_BASE + 0x10)        // I2C timing register
#define I2C2_TIMEOUTR (*(volatile uint32_t *)I2C2_BASE + 0x14)        // I2C timeout register
#define I2C2_ISR      (*(volatile uint32_t *)I2C2_BASE + 0x18)        // I2C interrupt and status register
#define I2C2_ICR      (*(volatile uint32_t *)I2C2_BASE + 0x1C)        // I2C interrupt clear register
#define I2C2_PECR     (*(volatile uint32_t *)I2C2_BASE + 0x20)        // I2C PEC register
#define I2C2_RXDR     (*(volatile uint32_t *)I2C2_BASE + 0x24)        // I2C receive data register
#define I2C2_TXDR     (*(volatile uint32_t *)I2C2_BASE + 0x28)        // I2C transmit data register


// #############################
// ### GPIO                  ###
// #############################

#define PORTA_MODER  (*(volatile uint32_t *)GPIOA_BASE + 0x00)        // GPIO port mode register
#define PORTA_OTYPER (*(volatile uint32_t *)GPIOA_BASE + 0x04)        // GPIO port output type register
#define PORTA_SPEEDR (*(volatile uint32_t *)GPIOA_BASE + 0x08)        // GPIO port output speed register
#define PORTA_PUPDR  (*(volatile uint32_t *)GPIOA_BASE + 0x0C)        // GPIO port pull-up/pull-down register
#define PORTA_IDR    (*(volatile uint32_t *)GPIOA_BASE + 0x10)        // GPIO port input data register
#define PORTA_ODR    (*(volatile uint32_t *)GPIOA_BASE + 0x14)        // GPIO port output data register
#define PORTA_BSRR   (*(volatile uint32_t *)GPIOA_BASE + 0x18)        // GPIO port bit set/reset register
#define PORTA_LCKR   (*(volatile uint32_t *)GPIOA_BASE + 0x1C)        // GPIO port configuration lock register
#define PORTA_AFR1   (*(volatile uint32_t *)GPIOA_BASE + 0x20)        // GPIO alternate function low register
#define PORTA_AFR2   (*(volatile uint32_t *)GPIOA_BASE + 0x24)        // GPIO alternate function high register
#define PORTA_BRR    (*(volatile uint32_t *)GPIOA_BASE + 0x28)        // GPIO port bit reset register

#define PORTB_MODER  (*(volatile uint32_t *)GPIOB_BASE + 0x00)        // GPIO port mode register
#define PORTB_OTYPER (*(volatile uint32_t *)GPIOB_BASE + 0x04)        // GPIO port output type register
#define PORTB_SPEEDR (*(volatile uint32_t *)GPIOB_BASE + 0x08)        // GPIO port output speed register
#define PORTB_PUPDR  (*(volatile uint32_t *)GPIOB_BASE + 0x0C)        // GPIO port pull-up/pull-down register
#define PORTB_IDR    (*(volatile uint32_t *)GPIOB_BASE + 0x10)        // GPIO port input data register
#define PORTB_ODR    (*(volatile uint32_t *)GPIOB_BASE + 0x14)        // GPIO port output data register
#define PORTB_BSRR   (*(volatile uint32_t *)GPIOB_BASE + 0x18)        // GPIO port bit set/reset register
#define PORTB_LCKR   (*(volatile uint32_t *)GPIOB_BASE + 0x1C)        // GPIO port configuration lock register
#define PORTB_AFR1   (*(volatile uint32_t *)GPIOB_BASE + 0x20)        // GPIO alternate function low register
#define PORTB_AFR2   (*(volatile uint32_t *)GPIOB_BASE + 0x24)        // GPIO alternate function high register
#define PORTB_BRR    (*(volatile uint32_t *)GPIOB_BASE + 0x28)        // GPIO port bit reset register


// #############################
// ### RNG                   ###
// #############################

#define RNG_CR   (*(volatile uint32_t *)RNG_BASE + 0x00)        // RNG control register
#define RNG_SR   (*(volatile uint32_t *)RNG_BASE + 0x04)        // RNG status register
#define RNG_DR   (*(volatile uint32_t *)RNG_BASE + 0x08)        // RNG data register
#define RNG_HTCR (*(volatile uint32_t *)RNG_BASE + 0x10)        // RNG health test control register


#define VREFBUF_SC0_CAL (*(volatile uint32_t *)0x1FFF 75F0)        //   Address of VREFBUF trimming value for VRS=0, VREF_SC0 in STM32WL datasheet */
#define VREFBUF_SC1_CAL (*(volatile uint32_t *)0x1FFF 7530)        //   Address of VREFBUF trimming value for VRS=1, VREF_SC1 in STM32WL datasheet */

#define setBitInRegister(REG, BIT)    ((REG) |= (BIT))
#define clearBitInREgister(REG, BIT)  ((REG) &= ~(BIT))
#define readBitFromRegister(REG, BIT) ((REG) & (BIT))

// We could also have some simple inline function definitions for that.

/* Use of CMSIS compiler intrinsics for register exclusive access */
/* Atomic 32-bit register access macro to set one or several bits */
#define ATOMIC_SET_BIT(REG, BIT)                                  \
    do {                                                          \
        uint32_t val;                                             \
        do {                                                      \
            val = __LDREXW((__IO uint32_t *)&(REG)) | (BIT);      \
        } while ((__STREXW(val, (__IO uint32_t *)&(REG))) != 0U); \
    } while (0)

/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC_CLEAR_BIT(REG, BIT)                                \
    do {                                                          \
        uint32_t val;                                             \
        do {                                                      \
            val = __LDREXW((__IO uint32_t *)&(REG)) & ~(BIT);     \
        } while ((__STREXW(val, (__IO uint32_t *)&(REG))) != 0U); \
    } while (0)
