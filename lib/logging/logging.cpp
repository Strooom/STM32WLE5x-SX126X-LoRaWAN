// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <stdint.h>        // required for uint8_t and similar type definitions
// #include <string.h>        // required for strncpy()
// #include <stdio.h>         // required for vsnprintf()
// #include <stdarg.h>        // required for handling variable number of argument functions, in this case snprintf
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include "logging.h"        //

// bool logging::debugProbePresent{false};                // initialisation of static member variable
uint32_t logging::activeLoggingSources{0};             // initialisation of static member variable
uint32_t logging::activeLoggingDestinations{0};        // initialisation of static member variable

// bool logging::isDebugProbePresent() {
//     return debugProbePresent;
// }

void logging::enableLoggingSource(loggingSource aSource) {
    activeLoggingSources = activeLoggingSources | (0x01 << static_cast<uint32_t>(aSource));
}
void logging::disableLoggingSource(loggingSource aSource) {
    activeLoggingSources = activeLoggingSources & ~(0x01 << static_cast<uint32_t>(aSource));
}

void logging::enableLoggingDestination(loggingDestination aDestination) {
    activeLoggingDestinations = activeLoggingDestinations | (0x01 << static_cast<uint32_t>(aDestination));
}
void logging::disableLoggingDestination(loggingDestination aDestination) {
    activeLoggingDestinations = activeLoggingDestinations & ~(0x01 << static_cast<uint32_t>(aDestination));
}

bool logging::isActive(loggingSource aSource) {
    uint32_t mask = 0x01 << static_cast<uint32_t>(aSource);
    return ((activeLoggingSources & mask) != 0);
}

bool logging::isActive(loggingDestination aDestination) {
    uint32_t mask = 0x01 << static_cast<uint32_t>(aDestination);
    return ((activeLoggingDestinations & mask) != 0);
}

#ifndef environment_desktop

#include "main.h"                            // required for ITM_Sendchar

char logging::buffer[bufferLength]{};        // Transmit buffer

void logging::snprintf(const char *format, ...) {
    if (activeLoggingDestinations != 00) {        // if any destination is active
        va_list argList;
        va_start(argList, format);
        uint32_t length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);

        write(length);

        // for (uint32_t index = 0; index < length; index++) {
        //     ITM_SendChar(static_cast<uint32_t>(buffer[index]));
        // }
    }
}

void logging::snprintf(loggingSource aSource, const char *format, ...) {
    if (activeLoggingDestinations != 00) {        // if any destination is active
        if (isActive(aSource)) {
            va_list argList;
            va_start(argList, format);
            uint32_t length = vsnprintf(buffer, bufferLength, format, argList);
            va_end(argList);

            write(length);

            // for (uint32_t index = 0; index < length; index++) {
            //     ITM_SendChar(static_cast<uint32_t>(buffer[index]));
            // }
        }
    }
}

void logging::write(uint32_t dataLength) {
    if (isActive(loggingDestination::debugProbe)) {
        for (uint32_t index = 0; index < dataLength; index++) {
            ITM_SendChar(static_cast<uint32_t>(buffer[index]));
        }
    }
    if (isActive(loggingDestination::uart)) {
        // HAL_UART_();
    }
}
// void logging::detectDebugProbe() {
//     debugProbePresent = ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0x0001);        // This LSBit is 1 when the ST-Link is connected, 0 when disconnected
// }

#else

void logging::snprintf(const char *format, ...) {}
void logging::snprintf(loggingSource aSource, const char *format, ...) {}
#endif
