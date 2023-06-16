// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <stdint.h>        // required for uint8_t and similar type definitions
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include "logging.h"                            //

uint32_t logging::activeSources{0};             // initialisation of static member variable
uint32_t logging::activeDestinations{0};        // initialisation of static member variable
char logging::buffer[bufferLength]{};           // initialisation of static member variable

uint32_t logging::snprintf(const char *format, ...) {
    uint32_t length{0};
    if (activeDestinations != 00) {        // only if any destination is active
        va_list argList;
        va_start(argList, format);
        length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);
        write(length);
    }
    return length;
}

uint32_t logging::snprintf(source aSource, const char *format, ...) {
    uint32_t length{0};
    if (activeDestinations != 00) {        // only if any destination is active
        if (isActive(aSource)) {
            va_list argList;
            va_start(argList, format);
            length = vsnprintf(buffer, bufferLength, format, argList);
            va_end(argList);
            write(length);
        }
    }
    return length;
}

#ifndef environment_desktop
#include "main.h"        // required for ITM_Sendchar

void logging::write(uint32_t dataLength) {
    if (isActive(destination::debugProbe)) {
        for (uint32_t index = 0; index < dataLength; index++) {
            ITM_SendChar(static_cast<uint32_t>(buffer[index]));
        }
    }
    if (isActive(destination::uart)) {
        // HAL_UART_();
    }
}

#else
void logging::write(uint32_t dataLength) {}        // TODO : I think this could output to the console
#endif
