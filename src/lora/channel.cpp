#include "channel.h"

loRaChannel rx2Channel(869'525'000U, true);

loRaChannel::loRaChannel(uint32_t theFrequency, bool isEnabled) : frequency(theFrequency), enabled(isEnabled) {}

void loRaChannels::initialize() {
    add(loRaChannel(868'100'000U, true));
    add(loRaChannel(868'200'000U, true));
    add(loRaChannel(868'300'000U, true));

    // the next channels are used by TTN. Normally they should be activated through MAC commands

    add(loRaChannel(867'100'000U, true));
    add(loRaChannel(867'300'000U, true));
    add(loRaChannel(867'500'000U, true));
    add(loRaChannel(867'700'000U, true));
    add(loRaChannel(867'900'000U, true));
}

loRaChannel loRaChannels::getNext() {
    return theChannels[0];        // TODO : make it really random
}

void loRaChannels::add(loRaChannel newChannel) {
    // TODO : We should check if the channel not already exists in the list
    if (nmbrAvailableChannels < maxNmbrChannels) {
        theChannels[nmbrAvailableChannels].frequency = newChannel.frequency;
        theChannels[nmbrAvailableChannels].enabled   = newChannel.enabled;
        nmbrAvailableChannels++;
    }
}


void loRaChannels::remove(loRaChannel toBeRemovedChannel) {}
void loRaChannels::remove(uint32_t channelIndex) {}
void loRaChannels::enable(loRaChannel toBeRemovedChannel) {}
void loRaChannels::enable(uint32_t channelIndex) {}
void loRaChannels::disable(loRaChannel toBeRemovedChannel) {}
void loRaChannels::disable(uint32_t channelIndex) {}
