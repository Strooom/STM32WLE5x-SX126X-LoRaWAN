// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include "lorachannel.h"

class loRaChannelCollection {
  public:
    uint32_t getRandomChannelIndex();
    static constexpr uint32_t maxNmbrChannels{8};        // For EU868 this is 8, when extending to other regions this may be different
    uint32_t nmbrAvailableChannels{3};                   // at startup, only 3 channels are activated by default.
    void addChannel(uint32_t frequency);
    void removeChannel(uint32_t frequency);              // modifying a channel can be done by removing and then adding it again with the new parameters

    loRaChannel txRxChannels[maxNmbrChannels] = {
        loRaChannel(true, 868'100'000U, 0, 5),
        loRaChannel(true, 868'300'000U, 0, 5),
        loRaChannel(true, 868'500'000U, 0, 5),
        loRaChannel(false, 0U, 0, 5),
        loRaChannel(false, 0U, 0, 5),
        loRaChannel(false, 0U, 0, 5),
        loRaChannel(false, 0U, 0, 5),
        loRaChannel(false, 0U, 0, 5)};

    loRaChannel rx2Channel{loRaChannel(true, 869'525'000U, 0, 0)};

  private:
    uint32_t getRandomNumber();
};
