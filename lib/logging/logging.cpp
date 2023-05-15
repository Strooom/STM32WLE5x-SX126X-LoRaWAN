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
#include "logging.h"                               //

bool logging::debugProbePresent{false};            // initialisation of static member variable
uint32_t logging::activeLoggingChannels{0};        // initialisation of static member variable

bool logging::isDebugProbePresent() {
    return debugProbePresent;
}

void logging::enableLoggingChannel(loggingChannel aChannel) {
    activeLoggingChannels = activeLoggingChannels | (0x01 << static_cast<uint32_t>(aChannel));
}
void logging::disableLoggingChannel(loggingChannel aChannel) {
    activeLoggingChannels = activeLoggingChannels & ~(0x01 << static_cast<uint32_t>(aChannel));
}

bool logging::loggingIsActive(loggingChannel aChannel) {
    uint32_t channelMask = 0x01 << static_cast<uint32_t>(aChannel);
    return ((activeLoggingChannels & channelMask) != 0);
}

#ifndef environment_desktop

#include "main.h"                            // required for ITM_Sendchar - TODO : I could reduce the attack surface by only including the core_cm4.h from CMSIS

char logging::buffer[bufferLength]{};        // Transmit buffer

void logging::snprintf(const char *format, ...) {
    if (debugProbePresent) {        // if no debugprobe, then no tracing, so do not bother to calculate the output
        va_list argList;
        va_start(argList, format);
        uint32_t length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);

        for (uint32_t index = 0; index < length; index++) {
            ITM_SendChar(static_cast<uint32_t>(buffer[index]));
        }
    }
}

void logging::snprintf(loggingChannel aChannel, const char *format, ...) {
    uint32_t channelMask = 0x01 << static_cast<uint32_t>(aChannel);
    if (debugProbePresent && ((activeLoggingChannels & channelMask) != 0)) {
        va_list argList;
        va_start(argList, format);
        uint32_t length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);

        for (uint32_t index = 0; index < length; index++) {
            ITM_SendChar(static_cast<uint32_t>(buffer[index]));
        }
    }
}

void logging::detectDebugProbe() {
    debugProbePresent = ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0x0001);        // This LSBit is 1 when the ST-Link is connected, 0 when disconnected
}

#else
void logging::detectDebugProbe() {}
void logging::snprintf(const char *format, ...) {}
void logging::snprintf(loggingChannel aChannel, const char *format, ...) {}
#endif