// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "stm32wle5.h"
#include "msiclockfrequency.h"
#include "stm32wle_cpu.h"


// #############################
// ### RCC                   ###
// ### RM0461 Page 230       ###
// #############################

peripheralRegister RCC_CR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x00));
peripheralRegister RCC_AHB1ENR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x48));
peripheralRegister RCC_AHB2ENR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x4C));
peripheralRegister RCC_AHB3ENR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x50));
peripheralRegister RCC_APB1ENR1(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x58));
peripheralRegister RCC_APB1ENR2(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x5C));
peripheralRegister RCC_APB2ENR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x60));
peripheralRegister RCC_APB3ENR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x64));
peripheralRegister RCC_CCIPR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x88));
peripheralRegister RCC_BDCR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x90));
peripheralRegister RCC_CFGR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x08));

peripheralRegister RCC_AHB2RSTR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x2C));

// #define RCC_ICSCR (*(volatile uint32_t *)RCC_BASE + 0x04)            // RCC internal clock sources calibration register
// #define RCC_PLLCFGR (*(volatile uint32_t *)RCC_BASE + 0x0C)          // RCC system PLL configuration register
// #define RCC_CIER (*(volatile uint32_t *)RCC_BASE + 0x18)             // RCC clock interrupt enable register
// #define RCC_CIFR (*(volatile uint32_t *)RCC_BASE + 0x1C)             // RCC clock interrupt flag register
// #define RCC_CICR (*(volatile uint32_t *)RCC_BASE + 0x20)             // RCC clock interrupt clear register
// #define RCC_AHB1RSTR (*(volatile uint32_t *)RCC_BASE + 0x28)         // RCC AHB1 peripheral reset register
// #define RCC_AHB2RSTR (*(volatile uint32_t *)RCC_BASE + 0x2C)         // RCC AHB2 peripheral reset register
// #define RCC_AHB3RSTR (*(volatile uint32_t *)RCC_BASE + 0x30)         // RCC AHB3 peripheral reset register
// #define RCC_APB1RSTR1 (*(volatile uint32_t *)RCC_BASE + 0x38)        // RCC APB1 peripheral reset register 1
// #define RCC_APB1RSTR1 (*(volatile uint32_t *)RCC_BASE + 0x3C)        // RCC APB1 peripheral reset register 2
// #define RCC_APB2RSTR (*(volatile uint32_t *)RCC_BASE + 0x40)         // RCC APB2 peripheral reset register
// #define RCC_APB3RSTR (*(volatile uint32_t *)RCC_BASE + 0x44)         // RCC APB3 peripheral reset register
// #define RCC_AHB1SMENR (*(volatile uint32_t *)RCC_BASE + 0x68)         // RCC AHB1 peripheral clocks enable in Sleep and Stop modes register
// #define RCC_AHB2SMENR (*(volatile uint32_t *)RCC_BASE + 0x6C)         // RCC AHB2 peripheral clocks enable in Sleep and Stop modes register
// #define RCC_AHB3SMENR (*(volatile uint32_t *)RCC_BASE + 0x70)         // RCC AHB3 peripheral clocks enable in Sleep and Stop modes register
// #define RCC_APB1SMENR1 (*(volatile uint32_t *)RCC_BASE + 0x78)        // RCC APB1 peripheral clocks enable in Sleep and Stop modes register 1
// #define RCC_APB1SMENR2 (*(volatile uint32_t *)RCC_BASE + 0x7C)        // RCC APB1 peripheral clocks enable in Sleep and Stop modes register 2
// #define RCC_APB2SMENR (*(volatile uint32_t *)RCC_BASE + 0x80)         // RCC APB2 peripheral clocks enable in Sleep and Stop modes register
// #define RCC_APB3SMENR (*(volatile uint32_t *)RCC_BASE + 0x84)         // RCC APB3 peripheral clocks enable in Sleep and Stop modes register
// #define RCC_CSR (*(volatile uint32_t *)RCC_BASE + 0x94)             // RCC clock control & status register
// #define RCC_EXTCFGR (*(volatile uint32_t *)RCC_BASE + 0x108)        // RCC extended clock recovery register

// #############################
// ### PWR                   ###
// #############################

peripheralRegister PWR_CR1(reinterpret_cast<volatile uint32_t *const>(PWR_BASE + 0x00));
peripheralRegister PWR_SR2(reinterpret_cast<volatile uint32_t *const>(PWR_BASE + 0x14));
peripheralRegister SUBGHZ_SPI_CR(reinterpret_cast<volatile uint32_t *const>(PWR_BASE + 0x90));

// #############################
// ### FLASH                 ###
// #############################

peripheralRegister FLASH_ACR(reinterpret_cast<volatile uint32_t *const>(FLASH_BASE + 0x00));

// #############################
// ### SYSTICK               ###
// #############################

#define SYSTICK_BASE (0xE000E010UL)
peripheralRegister STCSR(reinterpret_cast<volatile uint32_t *const>(SYSTICK_BASE + 0x00));
peripheralRegister STRVR(reinterpret_cast<volatile uint32_t *const>(SYSTICK_BASE + 0x04));
peripheralRegister STCVR(reinterpret_cast<volatile uint32_t *const>(SYSTICK_BASE + 0x08));
peripheralRegister STCR(reinterpret_cast<volatile uint32_t *const>(SYSTICK_BASE + 0x0C));

// ####################################
// ### Device electronic signature  ###
// ####################################

peripheralRegister UID96_0(reinterpret_cast<volatile uint32_t *const>(0x1FFF'7590 + 0x00));        // 96 bit unique ID based on Lot, Wafer and X/Y coordinates of chip on wafer
peripheralRegister UID96_1(reinterpret_cast<volatile uint32_t *const>(0x1FFF'7590 + 0x04));
peripheralRegister UID96_2(reinterpret_cast<volatile uint32_t *const>(0x1FFF'7590 + 0x08));

peripheralRegister FLASHSIZE(reinterpret_cast<volatile uint32_t *const>(0x1FFF'75E0 + 0x08));        // Flash size in kBytes - should read 256
peripheralRegister PKG(reinterpret_cast<volatile uint32_t *const>(0x1FFF'7500 + 0x08));              // Package Type : should read 00000 for UFBGA73

peripheralRegister UID64_0(reinterpret_cast<volatile uint32_t *const>(0x1FFF'7580 + 0x00));        // unitque 32 bit ID for each component
peripheralRegister UID64_1(reinterpret_cast<volatile uint32_t *const>(0x1FFF'7580 + 0x04));        // should read 0x0080 E115 meaning 0x0080E1 for STMicroelectronics and 0x15 for device ID

void stm32wleCpu::initialize() {
    initializePower();
    initializeClock();
    setSystickFrequency(currentClockSpeed);
    enableSystick(true);

    // LL_DBGMCU_EnableDBGStopMode(); // enable debugging, even when in Stop mode - only for debug release TODO : make this built type dependent
}

void stm32wleCpu::initializePower() {
}

void stm32wleCpu::initializeClock() {
    // Keep it simple :
    // * SysyClock = MSI @ 16 MHz (4 MHz after reset)
    // * All periperals clocked by SysClock
    RCC_CR.write(0x0000'008B);              // SysClk source is MSI @ 16 MHz
    RCC_CFGR.write(0x0000'0000);            // TODO - check correct bits  : defaults seem to be OK
    RCC_CCIPR.write(0x0000'0000);           // TODO - set correct bits when peripherals are activated
    if (!RCC_BDCR.readBit(0)) {             // if LSE is not yet on...
        PWR_CR1.setBit(8);                  // enable write access to RTC registers
        RCC_BDCR.write(0x0000'8101);        // TODO - check correct bits : enable LSE, enable RTC clock, set RTC clock source to LSE
        PWR_CR1.clearBit(8);                // disable write access to RTC registers
    }
    currentClockSpeed = msiClockFrequency::f16MHz;
}

void stm32wleCpu::setVoltageScaling(voltageScale theNewScale) {
    switch (theNewScale) {
        case voltageScale::v1dot0:        // low power mode
            break;

        case voltageScale::v1dot2:        // high performance mode
        default:
            break;
    }
}

voltageScale stm32wleCpu::getVoltageScaling() {
    switch (PWR_SR2.read() & 0x11)        // TODO : set mask..
    {
        case 0x01:
            return voltageScale::v1dot0;
            break;
        case 0x02:
        default:
            return voltageScale::v1dot2;
            break;

    }        //
}

void stm32wleCpu::setClockSpeed(msiClockFrequency newClockFrequency) {
    // Change MSI speed..
    // 1. This may require the change the voltage scaling.. See RM0461 chapter 6.2.9
    if (static_cast<uint32_t>(newClockFrequency) > 16'000'000U) {
        if (getVoltageScaling() == voltageScale::v1dot0) {
            setVoltageScaling(voltageScale::v1dot2);
        }
    }

    setFlashWaitstates(newClockFrequency);        // 2. Take into account that the wait-states need to be changed..

    // 3. TODO : Reconfigure RCC for new clock speed

    setSystickFrequency(newClockFrequency);        // 4. Finally, reconfigure the systick timer reload value so we still get 1 ms ticks

    currentClockSpeed = newClockFrequency;
}

msiClockFrequency stm32wleCpu::getClockSpeed() {
    uint32_t msiRange = (RCC_CR.read() & 0x0000'00F0) >> 4;
    return msiRangeToClockFrequency(msiRange);
}

void stm32wleCpu::goSleep(sleepMode theSleepMode) {
    // 1. Prepare MCU for sleep
    // HAL_SuspendTick();

    // 2. Go to sleep
    //  TODO : Determine the lowest power sleep mode..
    // HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

    // 3. Wake up from sleep
    //        HAL_ResumeTick();
}

void stm32wleCpu::setSystickFrequency(msiClockFrequency theClockFrequency) {
    STRVR.write((static_cast<uint32_t>(theClockFrequency) / 1000U) - 1U);
    STCVR.write(0U);
    STCSR.write(0x7U);
}

void stm32wleCpu::setFlashWaitstates(msiClockFrequency theClockFrequency) {
    uint32_t nmbrWaitStates{0};
    switch (getVoltageScaling()) {
        case voltageScale::v1dot0:
            if (static_cast<uint32_t>(theClockFrequency) > 12'000'000U) {
                nmbrWaitStates = 2;
            } else if (static_cast<uint32_t>(theClockFrequency) > 6'000'000U) {
                nmbrWaitStates = 1;
            } else {
                nmbrWaitStates = 0;
            }
            break;

        case voltageScale::v1dot2:
            if (static_cast<uint32_t>(theClockFrequency) > 36'000'000U) {
                nmbrWaitStates = 2;
            } else if (static_cast<uint32_t>(theClockFrequency) > 18'000'000U) {
                nmbrWaitStates = 1;
            } else {
                nmbrWaitStates = 0;
            }
            break;

        default:
            nmbrWaitStates = 2;        // safe fall-back - should not occur
            break;
    }
    uint32_t currentFlashSettings = FLASH_ACR.read();
    currentFlashSettings &= 0xFFFF'FFF8;
    currentFlashSettings |= nmbrWaitStates;
    FLASH_ACR.write(currentFlashSettings);
}

uint32_t stm32wleCpu::clockFrequencyToMsiRange(msiClockFrequency theClockFrequency) {
    switch (theClockFrequency) {
        case msiClockFrequency::f100kHz:
            return 0x00;
            break;
        case msiClockFrequency::f200kHz:
            return 0x01;
            break;
        case msiClockFrequency::f400kHz:
            return 0x02;
            break;
        case msiClockFrequency::f800kHz:
            return 0x03;
            break;
        case msiClockFrequency::f1MHz:
            return 0x04;
            break;
        case msiClockFrequency::f2MHz:
            return 0x05;
            break;
        case msiClockFrequency::f4MHz:
        default:
            return 0x06;
            break;
        case msiClockFrequency::f8MHz:
            return 0x07;
            break;
        case msiClockFrequency::f16MHz:
            return 0x08;
            break;
        case msiClockFrequency::f24MHz:
            return 0x09;
            break;
        case msiClockFrequency::f32MHz:
            return 0x0A;
            break;
        case msiClockFrequency::f48MHz:
            return 0x0B;
            break;
    }
}

msiClockFrequency stm32wleCpu::msiRangeToClockFrequency(uint32_t msiRange) {
    msiRange = msiRange & 0xFU;
    switch (msiRange) {
        case 0x00:
            return msiClockFrequency::f100kHz;
            break;
        case 0x01:
            return msiClockFrequency::f200kHz;
            break;
        case 0x02:
            return msiClockFrequency::f400kHz;
            break;
        case 0x03:
            return msiClockFrequency::f800kHz;
            break;
        case 0x04:
            return msiClockFrequency::f1MHz;
            break;
        case 0x05:
            return msiClockFrequency::f2MHz;
            break;
        case 0x06:
        default:
            return msiClockFrequency::f4MHz;
            break;
        case 0x07:
            return msiClockFrequency::f8MHz;
            break;
        case 0x08:
            return msiClockFrequency::f16MHz;
            break;
        case 0x09:
            return msiClockFrequency::f24MHz;
            break;
        case 0x0A:
            return msiClockFrequency::f32MHz;
            break;
        case 0x0B:
            return msiClockFrequency::f48MHz;
            break;
    }
}

void stm32wleCpu::enableSystick(bool on) {
    if (on) {
        STCSR.write(0x7U);
    } else {
        STCSR.write(0x0U);
    }
}

uint64_t stm32wleCpu::getUniqueId() const {
    return (
        (static_cast<uint64_t>(UID64_1.read()) << 32) ||
        static_cast<uint64_t>(UID64_0.read()));
}

void stm32wleCpu::reset() {
//    __NVIC_SystemReset(void);
}

void stm32wleCpu::waitForStableClock() {
//__ASM volatile ("nop")
//__ASM volatile ("nop")
}
