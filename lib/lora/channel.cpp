#include "channel.h"


uint32_t loRaChannels::getRandomChannelIndex() {
    return (getRandomNumber() % nmbrAvailableChannels);
}
