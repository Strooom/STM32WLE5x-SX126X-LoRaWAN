#include "channel.h"
#include "logging.h"

extern logging theLog;

loRaChannel::loRaChannel() : frequency(0), enabled(false) {}

loRaChannel::loRaChannel(uint32_t theFrequency, bool isEnabled) : frequency(theFrequency), enabled(isEnabled) {}

loRaChannels::loRaChannels() {}

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

loRaChannel loRaChannels::getRx2channel() {
    return loRaChannel(869'525'000U, true);
}

void loRaChannels::add(loRaChannel newChannel) {
    // TODO : We should check if the channel not already exists in the list
    if (nmbrAvailableChannels < maxNmbrChannels) {
        theLog.snprintf("%u : Added Channel [%u] Hz \n", (nmbrAvailableChannels + 1), newChannel.frequency);
        theChannels[nmbrAvailableChannels].frequency = newChannel.frequency;
        theChannels[nmbrAvailableChannels].enabled   = newChannel.enabled;
        nmbrAvailableChannels++;
    }
}
