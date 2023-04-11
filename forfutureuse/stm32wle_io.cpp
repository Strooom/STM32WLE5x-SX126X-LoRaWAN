// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "stm32wle5.h"
#include "stm32wle_io.h"

/*
vBus = PB4
I2C2_SDA = PA15
I2C_SCL = PB15
writeProtect = PB9
SPI2_MOSI = PA10
SPI2_CLK = PB13
SPI2_chipSelect = PB5
SPI2_dataCommand = PB14
SPI2_displayReset = PA0
SPI2_displayBusy = PB10
UART2_Rx = PA3
UART2_Tx = PA2
UART1_Rx = PB7
UART1_Tx = PB6
DEBUG_SWDIO = PA13
DEBUG_SWCLK = PA14
DBG_SWO = PB3

// Note : in V2.0 van de hardware zitten deze signalen wel op andere pinnen... -> voorlopig tot ik de nieuwe PCBs heb moet ik deze code nog aanpassen
vBus = PB4
I2C2_SDA = PA15
I2C_SCL = PB15
writeProtect = PA0
SPI2_MOSI = PA10
SPI2_CLK = PB13
SPI2_chipSelect = PB5
SPI2_dataCommand = PC1
SPI2_displayReset = ???
SPI2_displayBusy = PC0
UART1_Rx = PA3
UART1_Tx = PA2
UART2_Rx = PB7
UART2_Tx = PB6
DEBUG_SWDIO = PA13
DEBUG_SWCLK = PA14
DBG_SWO = PB3

*/

// #############################
// ### GPIO                  ###
// #############################

extern peripheralRegister RCC_AHB2ENR;
extern peripheralRegister RCC_AHB2RSTR;

peripheralRegister PORTA_MODER(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x00));
peripheralRegister PORTA_OTYPER(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x04));
peripheralRegister PORTA_SPEEDR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x08));
peripheralRegister PORTA_PUPDR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x0C));
peripheralRegister PORTA_IDR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x10));
peripheralRegister PORTA_ODR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x14));
peripheralRegister PORTA_BSRR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x18));
peripheralRegister PORTA_LCKR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x1C));
peripheralRegister PORTA_AFR1(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x20));
peripheralRegister PORTA_AFR2(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x24));
peripheralRegister PORTA_BRR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x28));

peripheralRegister PORTB_MODER(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x00));
peripheralRegister PORTB_OTYPER(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x04));
peripheralRegister PORTB_SPEEDR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x08));
peripheralRegister PORTB_PUPDR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x0C));
peripheralRegister PORTB_IDR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x10));
peripheralRegister PORTB_ODR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x14));
peripheralRegister PORTB_BSRR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x18));
peripheralRegister PORTB_LCKR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x1C));
peripheralRegister PORTB_AFR1(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x20));
peripheralRegister PORTB_AFR2(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x24));
peripheralRegister PORTB_BRR(reinterpret_cast<volatile uint32_t *const>(GPIOB_BASE + 0x28));

peripheralRegister PORTC_MODER(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x00));
peripheralRegister PORTC_OTYPER(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x04));
peripheralRegister PORTC_SPEEDR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x08));
peripheralRegister PORTC_PUPDR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x0C));
peripheralRegister PORTC_IDR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x10));
peripheralRegister PORTC_ODR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x14));
peripheralRegister PORTC_BSRR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x18));
peripheralRegister PORTC_LCKR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x1C));
peripheralRegister PORTC_AFR1(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x20));
peripheralRegister PORTC_AFR2(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x24));
peripheralRegister PORTC_BRR(reinterpret_cast<volatile uint32_t *const>(GPIOC_BASE + 0x28));

void stm32wleIo::initialize() {
    RCC_AHB2ENR.setBit(0);               // Enable clock to the GPIO Port A
    (void)RCC_AHB2ENR.readBit(0);        // not 100% sure why this is needed but it looks like a way to ensure the clock is running stable before accessing the registers of the peripheral
    RCC_AHB2ENR.setBit(1);               // Enable clock to the GPIO Port B
    (void)RCC_AHB2ENR.readBit(1);
    RCC_AHB2ENR.setBit(2);        // Enable clock to the GPIO Port C
    (void)RCC_AHB2ENR.readBit(2);

    PORTB_MODER.writeBits(0x03 << 8, 0x00);        // PB4 is an input
    PORTB_PUPDR.writeBits(0x03 << 8, 0x00U);     // need to disable the Pull-up/Pull-down resistors for PB4, because as a JTAG pin, its pull-up is enabled at reset.

    // PORTA_OTYPER.write(0);        // 0=push-pull, 1=open-drain -> I2C is open drain
    // PORTB_OTYPER.write(0);        // 0=push-pull, 1=open-drain
    // PORTC_OTYPER.write(0);        // 0=push-pull, 1=open-drain
    // PORTA_AFR1.write(0);        // Select alternate function for pins 0-7
    // PORTA_AFR2.write(0);        // Select alternate function for pins 8-15
    // PORTB_AFR1.write(0);        // Select alternate function for pins 0-7
    // PORTB_AFR2.write(0);        // Select alternate function for pins 8-15
}

void stm32wleIo::prepareForSleep() {
   
    PORTA_MODER.write(0xABFF'FFFF);        // set all ports to analog input, with no alternate function - same as in reset state
    PORTB_MODER.write(0xFFFF'FEBF);

// Note : think about keeping the debug ports working when debugging
// Power them down in a release version
#ifdef DEBUG
    //    HAL_DBGMCU_EnableDBGSleepMode();
    // HAL_DBGMCU_EnableDBGStopMode();
    // HAL_DBGMCU_EnableDBGStandbyMode();

#else
    RCC_AHB2ENR.clearBit(0);        // Disable clock to the GPIO Port A
    RCC_AHB2ENR.clearBit(1);        // Disable clock to the GPIO Port B
    RCC_AHB2ENR.clearBit(2);        // Disable clock to the GPIO Port C
#endif
}
void stm32wleIo::restoreAfterSleep() {
    initialize();        // for the time being, restoring is the same as initializing..
}
