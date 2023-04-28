// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <stdint.h>        // required for uint8_t and similar type definitions
#include <string.h>        // required for strncpy()
#include <stdio.h>         // required for vsnprintf()
#include <stdarg.h>        // required for handling variable number of argument functions, in this case snprintf
// #include <cstdio> TODO : which version is the best ?
// #include <cstdarg>
#include "logging.h"        //

bool logging::isDebugProbePresent() {
    return debugProbePresent;
}

#ifndef environment_desktop

#include "main.h"        // required for ITM_Sendchar - TODO : I could reduce the attack surface by only including the core_cm4.h from CMSIS

char logging::buffer[bufferLength]{};          // Transmit buffer
bool logging::debugProbePresent{false};        // remembers if we detected a debug probe present or not.

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

void logging::detectDebugProbe() {
    debugProbePresent = ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0x0001);        // This LSBit is 1 when the ST-Link is connected, 0 when disconnected
}

#else
bool logging::debugProbePresent{false};
void logging::detectDebugProbe() {}
void logging::snprintf(const char *format, ...) {}

#endif