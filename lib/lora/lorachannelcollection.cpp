#include "lorachannelcollection.h"

void loRaChannelCollection::selectNextActiveChannelIndex() {
    currentChannelIndex = (currentChannelIndex + 1) % maxNmbrChannels;
    while (txRxChannels[currentChannelIndex].frequency == 0) {
        currentChannelIndex = (currentChannelIndex + 1) % maxNmbrChannels;
    }
}

void loRaChannelCollection::selectRandomChannelIndex() {
    uint32_t randomCount = getRandomNumber() % maxNmbrChannels;
    for (uint32_t count = 0; count < randomCount; count++) {
        selectNextActiveChannelIndex();
    }
}

uint32_t loRaChannelCollection::getCurrentChannelIndex() const {
    return currentChannelIndex;
}

#ifndef environment_desktop

#include "main.h"

extern RNG_HandleTypeDef hrng;

uint32_t loRaChannelCollection::getRandomNumber() {
    uint32_t result{0};
    HAL_RNG_GenerateRandomNumber(&hrng, &result);
    return result;
}

#else

#include <cstdlib>

uint32_t loRaChannelCollection::getRandomNumber() {
    uint32_t result{0};
    result = rand();
    return result;
}

#endif