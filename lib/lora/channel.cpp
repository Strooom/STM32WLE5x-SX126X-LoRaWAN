#include "channel.h"

uint32_t loRaChannels::getRandomChannelIndex() {
    return (getRandomNumber() % nmbrAvailableChannels);
}

#ifndef environment_desktop

#include "main.h"

extern RNG_HandleTypeDef hrng;

uint32_t loRaChannels::getRandomNumber() {
    uint32_t result{0};
    HAL_RNG_GenerateRandomNumber(&hrng, &result);
    return result;
}

#else

// TODO : a desktop console version goes here

#endif