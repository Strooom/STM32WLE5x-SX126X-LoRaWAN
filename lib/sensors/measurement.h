// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "measurementchannel.h"

// represents a single measurement taken from a sensor, with a timestamp, and a flag if it has been transmitted to the cloud and acknowledged

class measurement {
  public:
    static constexpr uint32_t length{10};                             // total length of a measurement in bytes - NOTE : in RAM the compiler aligns things, so it occupies 16 bytes...
    static constexpr uint8_t transmittedToCloud{0b0000'0001};         // bit 0
    static constexpr uint8_t acknowledgedByCloud{0b0000'0010};        // bit 1
    static constexpr uint8_t priority{0b1100'0000};                   // bit 7..6

  private:
    measurementChannel type;        // defines what data the measurement contains, eg temperature, humidity, pressure, etc
    uint32_t timestamp;             // using GPS time : seconds elapsed since 2000 Jan 01 00:00:00 UTC
    uint32_t value;                 // value can be of any data type, but I assume it always fits in 4 bytes..
    uint8_t flags;                  // flags to indicate if the measurement has been transmitted to the cloud, and if it has been acknowledged by the cloud, etc..
};

// Notes: total storage per measurement is 10 bytes.
// Total bytes to transmit is 9 bytes,as we do not transmit the flags FTTB

enum class transmitPriority : uint8_t {
    low      = 0b0000'0000,
    medium   = 0b0100'0000,
    high     = 0b1000'0000,
    critical = 0b1100'0000
};
