// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

#include "bandwidth.h"

class loRaChannel {
  public:
    loRaChannel();
    loRaChannel(uint32_t frequency, bool enabled);

    // private:
    uint32_t frequency{0};                             // [Hz]
    bandwidth theBandwidth{bandwidth::b125kHz};        // [Hz]
    bool enabled{false};                               // [true/false]
    friend class loRaChannels;
};

class loRaChannels {
  public:
    loRaChannels();
    void initialize();
    void add(loRaChannel newChannel);
    loRaChannel get(uint32_t index);        //
    uint32_t getNext();                     // randomly select an index of the next to be used channel from the list of active channels
    loRaChannel getRx2channel();            // a special channel used by TTN
    static constexpr uint32_t maxNmbrChannels{16};
    uint32_t nmbrAvailableChannels{0};

  private:
    loRaChannel theChannels[maxNmbrChannels];
};

// TODO : For all channels, I can precalculate the frequency register value and store it in the channel object.