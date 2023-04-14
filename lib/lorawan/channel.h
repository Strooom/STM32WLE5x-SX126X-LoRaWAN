// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

class loRaChannels {
  public:
    uint32_t getRandomChannelIndex();
    static constexpr uint32_t nmbrAvailableChannels{8};
    const uint32_t txRxChannelFrequency[nmbrAvailableChannels]{
        // for some C++ language reason, I cannot make this constexpr..
        868'100'000U,
        868'300'000U,
        868'500'000U,
        867'100'000U,
        867'300'000U,
        867'500'000U,
        867'700'000U,
        867'900'000U};
    static constexpr uint32_t rx2ChannelFrequency{869'525'000U};

  private:
    uint32_t getRandomNumber();
};

// TODO : For all channels, I could precalculate the frequency register value and store it in the channel object.
