
#include "logging.h"        //
#include "main.h"           // required for ITM_Sendchar - TODO : I could reduce the attack surface by only including the core_cm4.h from CMSIS
#include <cstdio>
#include <cstdarg>

void logging::snprintf(const char *format, ...) {
    if (debugProbePresent) {
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
    debugProbePresent = (DBGMCU->CR & DBGMCU_CR_DBG_STANDBY) != 0;
}
