#include "channel.h"
#include "main.h"

extern RNG_HandleTypeDef hrng;

uint32_t loRaChannels::getRandomNumber() {
    uint32_t result{0};
    HAL_RNG_GenerateRandomNumber(&hrng, &result);
    return result;
}
