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

    union {
        uint32_t asUInt32;         // using UNIX time : seconds elapsed since 1970 Jan 01 00:00:00 UTC
        uint8_t asBytes[4];        //
    } timestamp;
    union {
        float asFloat;              // value can be of any data type, but I assume it always fits in 4 bytes..
        uint32_t asUInt32;          //
        uint8_t asBytes[4];         //
    } value;
    measurementChannel type;        // defines what data the measurement contains, eg temperature, humidity, pressure, etc
    uint8_t flags;                  // flags to indicate if the measurement has been transmitted to the cloud, and if it has been acknowledged by the cloud, etc..

    static uint32_t getTimeStamp();

  private:
};

// Notes: total storage per measurement is 10 bytes.
// Total bytes to transmit is 9 bytes,as we do not transmit the flags FTTB

enum class transmitPriority : uint8_t {
    doNotTransmit = 0b0000'0000,        // this will be stored in EEPROM, but not transmitted (over LoRaWAN) to the cloud
    low           = 0b0100'0000,        // transmitted only when network payload is full
    medium        = 0b1000'0000,        // this will be transmitted before the 'low', but only when the network payload is full
    high          = 0b1100'0000,        // this will cause a immediate network transmission, so the measurement goes witout any delay to the cloud
};
