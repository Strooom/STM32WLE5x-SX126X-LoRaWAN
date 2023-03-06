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
    loRaChannel(uint32_t frequency, bool enabled);

  private:
    uint32_t frequency{0};                             // [Hz]
    bandwidth theBandwidth{bandwidth::b125kHz};        // [Hz]
    bool enabled{false};                               // [true/false]
    friend class loRaChannels;
};

class loRaChannels {
  public:
    void initialize();
    void add(loRaChannel newChannel);
    void remove(loRaChannel toBeRemovedChannel);
    void remove(uint32_t channelIndex);
    void enable(loRaChannel toBeRemovedChannel);
    void enable(uint32_t channelIndex);
    void disable(loRaChannel toBeRemovedChannel);
    void disable(uint32_t channelIndex);
    loRaChannel getNext();        // randomly select the next to be used channel from the list of active channels
    static constexpr uint32_t maxNmbrChannels{16};
    uint32_t nmbrAvailableChannels{0};

  private:
    loRaChannel theChannels[maxNmbrChannels];
};