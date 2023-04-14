#include "channel.h"
#include "logging.h"

extern logging theLog;

uint32_t loRaChannels::getRandomChannelIndex() {
    return (getRandomNumber() % nmbrAvailableChannels);
}
