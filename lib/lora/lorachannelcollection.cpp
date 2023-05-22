#include "lorachannelcollection.h"

// void loRaChannelCollection::addChannel(uint32_t frequency) {
//     if (nmbrAvailableChannels < maxNmbrChannels) {
//         txRxChannels[nmbrAvailableChannels].frequency = frequency;
//         nmbrAvailableChannels++;
//     }
// }

// void loRaChannelCollection::removeChannel(uint32_t frequency) {
//     // TODO : remove channel from collection
//     // nmbrAvailableChannels--;
//     // Make sure there are no gaps in the array, swap items if needed..
// }

uint32_t loRaChannelCollection::getRandomChannelIndex() {
    uint32_t result{0};
    uint32_t randomCount = getRandomNumber() % maxNmbrChannels;
    for (uint32_t count = 0; count < randomCount; count++) {
        while (txRxChannels[result].frequency == 0) {
            result = (result + 1) % maxNmbrChannels;
        }
    }
    return result;
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