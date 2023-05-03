#include "lorachannelcollection.h"

loRaChannel::loRaChannel(bool isEnabled, uint32_t theFrequency, uint32_t theMinimumDataRateIndex, uint32_t theMaximumDataRateIndex) : enabled{isEnabled},
                                                                                                                                      frequency{theFrequency},
                                                                                                                                      minimumDataRateIndex{theMinimumDataRateIndex},
                                                                                                                                      maximumDataRateIndex{theMaximumDataRateIndex} {
}

