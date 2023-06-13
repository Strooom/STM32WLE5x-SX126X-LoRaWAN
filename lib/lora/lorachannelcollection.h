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
    void selectRandomChannelIndex();
    uint32_t getCurrentChannelIndex() const;
    static constexpr uint32_t maxNmbrChannels{16};        // Regional Parameters 1.0.3 line 320
    // uint32_t nmbrAvailableChannels{3};                    // 3 channels are always activate
    // // void addChannel(uint32_t frequency);
    // // void removeChannel(uint32_t frequency);               // modifying a channel can be done by removing and then adding it again with the new parameters

    loRaChannel txRxChannels[maxNmbrChannels] = {
        loRaChannel(868'100'000U, 0, 5),
        loRaChannel(868'300'000U, 0, 5),
        loRaChannel(868'500'000U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5),
        loRaChannel(0U, 0, 5)};

    loRaChannel rx2Channel{loRaChannel(869'525'000U, 0, 0)};

#ifndef unitTesting
  private:
#endif

    uint32_t getRandomNumber();
    uint32_t currentChannelIndex{0};
    void selectNextActiveChannelIndex();
};
