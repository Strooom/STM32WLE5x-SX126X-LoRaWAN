// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <stdint.h>         // required for uint8_t and similar type definitions
#include <string.h>         // required for strncpy()
#include <stdio.h>          // required for vsnprintf()
#include <stdarg.h>         // required for handling variable number of argument functions, in this case snprintf
#include "logging.h"        //
#include "main.h"           // required for ITM_Sendchar - TODO : I could reduce the attack surface by only including the core_cm4.h from CMSIS

#ifndef strlcpy
// on some platforms, string.h does not contain strlcpy, so in this case we add it here directly
size_t
strlcpy(char *dst, const char *src, size_t maxlen) {
    const size_t srclen = strlen(src);
    if (srclen + 1 < maxlen) {
        memcpy(dst, src, srclen + 1);
    } else if (maxlen != 0) {
        memcpy(dst, src, maxlen - 1);
        dst[maxlen - 1] = '\0';
    }
    return srclen;
}
#endif

#ifndef strlcat
// on some platforms, string.h does not contain strlcpy, so in this case we add it here directly
size_t
strlcat(char *dst, const char *src, size_t maxlen) {
    const size_t srclen = strlen(src);
    const size_t dstlen = strnlen(dst, maxlen);
    if (dstlen == maxlen) return maxlen + srclen;
    if (srclen < maxlen - dstlen) {
        memcpy(dst + dstlen, src, srclen + 1);
    } else {
        memcpy(dst + dstlen, src, maxlen - 1);
        dst[dstlen + maxlen - 1] = '\0';
    }
    return dstlen + srclen;
}
#endif

void logging::snprintf(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    uint32_t length = vsnprintf(buffer, bufferLength, format, argList);
    va_end(argList);

    for (uint32_t index = 0; index < length; index++) {
        ITM_SendChar(static_cast<uint32_t>(buffer[index]));
    }
}
