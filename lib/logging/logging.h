// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include <stdarg.h>                               // required so we can define functions with variable number of arguments

enum class loggingDestination : uint32_t {        // We can send logging to the following destinations, when they are connected and enabled :
    none,
    debugProbe,
    uart
};

enum class loggingSource : uint32_t {        // We can send logging from the following sources, when they are enabled :
    applicationEvents,

    lorawanSettings,
    lorawanState,
    lorawanEvents,
    lorawanMac,
    lorawanTiming,

    sx126xControl,
    sx126xBufferData,

    sensorEvents,
    sensorData,

    nvs,

    error,
    criticalError
};

class logging {
  public:
    // static void detectDebugProbe();                                              // check if a debug probe is connected by reading the DBGMCU_CR register
    // static bool isDebugProbePresent();                                           // return true if a debug probe is connected
    static constexpr uint32_t bufferLength{1024};                                //
    static void snprintf(const char *format, ...);                               // logs always
    static void snprintf(loggingSource aSource, const char *format, ...);        // logs only if the channel is active
    static void enableLoggingSource(loggingSource aSource);
    static void disableLoggingSource(loggingSource aSource);
    static void enableLoggingDestination(loggingDestination aDestination);
    static void disableLoggingDestination(loggingDestination aDestination);
    static bool isActive(loggingSource aSource);
    static bool isActive(loggingDestination aDestination);

  private:
    static char buffer[bufferLength];        // Transmit buffer. In this buffer the snprintf is expanded before the contents is being sent to one or more of the destinations
    // static bool debugProbePresent;           // remembers if we detected a debug probe present or not.
    static uint32_t activeLoggingSources;
    static uint32_t activeLoggingDestinations;
    static void write(uint32_t dataLength);
};
