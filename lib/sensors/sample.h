// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "infochanneltype.h"
#include "sampleflags.h"

// represents a single sample taken from a sensor, with a timestamp, and a flag if it has been transmitted to the cloud and acknowledged

class sample {
  public:
  private:
    infoChannelType type;        // defines what data the sample contains, eg temperature, humidity, pressure, etc
    uint32_t timestamp;          // TODO : define what standard we use here.. Unix time? GPS time?
    uint32_t value;              // value can be of any data type, but I assume it always fits in 4 bytes..
    sampleFlags flags;           // flags to indicate if the sample has been transmitted to the cloud, and if it has been acknowledged by the cloud, etc..
};

// Notes: total storage per sample is 10 bytes.
// Total bytes to transmit is 9 bytes,as we do not transmit the flags FTTB