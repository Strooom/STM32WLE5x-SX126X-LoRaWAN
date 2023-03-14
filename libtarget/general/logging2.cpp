#include "logging.h"        //
#include "main.h"           // required for ITM_Sendchar - TODO : I could reduce the attack surface by only including the core_cm4.h from CMSIS

void logging::snprintf(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    uint32_t length = vsnprintf(buffer, bufferLength, format, argList);
    va_end(argList);

    for (uint32_t index = 0; index < length; index++) {
        ITM_SendChar(static_cast<uint32_t>(buffer[index]));
    }
}