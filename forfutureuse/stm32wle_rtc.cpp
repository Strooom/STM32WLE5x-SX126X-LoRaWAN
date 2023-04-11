// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "stm32wle_rtc.h"

// #############################
// ### RTC                   ###
// ### RM0461 Page 884       ###
// #############################

#define RTC_CALR (*(volatile uint32_t *)RTC_BASE + 0x28)            // RTC calibration register
#define RTC_SHIFTR (*(volatile uint32_t *)RTC_BASE + 0x2C)          // RTC shift control register
#define RTC_TSTR (*(volatile uint32_t *)RTC_BASE + 0x30)            // RTC time stamp time register
#define RTC_TSDR (*(volatile uint32_t *)RTC_BASE + 0x34)            // RTC time stamp date register
#define RTC_TSSSR (*(volatile uint32_t *)RTC_BASE + 0x38)           // RTC time-stamp sub second register
#define RTC_ALRMAR (*(volatile uint32_t *)RTC_BASE + 0x40)          // RTC alarm A register
#define RTC_ALRMASSR (*(volatile uint32_t *)RTC_BASE + 0x44)        // RTC alarm A sub second register
#define RTC_ALMRBR (*(volatile uint32_t *)RTC_BASE + 0x48)          // RTC alarm B register
#define RTC_ALRMBSSR (*(volatile uint32_t *)RTC_BASE + 0x4C)        // RTC alarm B sub second register
#define RTC_SR (*(volatile uint32_t *)RTC_BASE + 0x50)              // RTC status register
#define RTC_MISR (*(volatile uint32_t *)RTC_BASE + 0x54)            // RTC masked interrupt status register
#define RTC_SCR (*(volatile uint32_t *)RTC_BASE + 0x5C)             // RTC status clear register
#define RTC_ALRABINR (*(volatile uint32_t *)RTC_BASE + 0x70)        // RTC alarm A binary register
#define RTC_ALRBBINR (*(volatile uint32_t *)RTC_BASE + 0x74)        // RTC alarm B binary register

peripheralRegister RTC_TR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x00));          // RTC time register - shadow register
peripheralRegister RTC_DR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x04));          // RTC date register - shadow register
peripheralRegister RTC_SSR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x08));         // RTC sub-second register - 256 ticks per second
peripheralRegister RTC_ICSR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x0C));        // RTC initialization and status register
peripheralRegister RTC_PRER(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x10));        // RTC prescaler register
peripheralRegister RTC_WUTR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x14));        // RTC wakeup timer register
peripheralRegister RTC_CR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x18));          // RTC control register

peripheralRegister RTC_WPR(reinterpret_cast<volatile uint32_t *const>(RTC_BASE + 0x24));        // RTC prescaler register

void realTimeClock::initialize() {
    if (!isInitialized()) {
        setWriteProtection(false);
        setInitializationMode(true);

        // RTC_PRER.write(0x007F00FFU);        // Set the prescaler to 127 for asynchronous and 255 for synchronous // This is already the default value..
        // RTC_CR.write(0x00000000U);          // Defaults seem to be OK
        // RTC_ICSR.write(0x00000000U);        // Defaults seem to be OK

        // TODO : set the time and date to the current time and date - from compiler

        RTC_CR.clearBit(10);         // disable WakeUp timer - note we are in initialization mode, so no need to wait for WUTWF
                                     // WUCKSEL default value is OK
        RTC_WUTR.write(2047);        // WakeUp every second

        setInitializationMode(false);
        setWriteProtection(true);
    }
}

bool realTimeClock::isInitialized() const {
    return RTC_ICSR.readBit(4);        // RTC_ICSR.INITS bit indicates if the RTC is initialized (year != 00)
}

void realTimeClock::setWriteProtection(bool writeProtectOn) {
    if (writeProtectOn) {
        RTC_WPR.write(0xFFU);
    } else {
        RTC_WPR.write(0xCAU);        // magic number sequence
        RTC_WPR.write(0x53U);
    }
}
void realTimeClock::setInitializationMode(bool initializationModeOn) {
    if (initializationModeOn) {
        RTC_ICSR.setBit(7);                 // RTC_ICSR.INIT bit to 1
        while (!RTC_ICSR.readBit(6))        // Wait for RTC_ICSR.INITF bit to be 1
        {
        }
    } else {
        RTC_ICSR.clearBit(7);        // RTC_ICSR.INIT bit to 0
    }
}

void realTimeClock::setRootSamplePeriod(uint32_t newPeriod) {
    if (newPeriod < 1) {
        newPeriod = 1;
    }
    if (newPeriod > 32) {
        newPeriod = 32;
    }
    newPeriod = 0x800 << (newPeriod - 1);
    //	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, newPeriod, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0);
}
