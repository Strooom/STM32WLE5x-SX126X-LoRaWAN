#include "adaptivedatarate.h"

void adaptiveDataRate::setDataRate(dataRate newDataRate) {
    currentDataRate = newDataRate;
}

void adaptiveDataRate::decreaseDataRate() {
    if (currentDataRate == dataRate::DR0) {
        return;
    } else {
        currentDataRate = static_cast<dataRate>(static_cast<uint32_t>(currentDataRate) - 1);
    }
}

void adaptiveDataRate::increaseDataRate() {
    if (currentDataRate == dataRate::DR7) {
        return;
    } else {
        currentDataRate = static_cast<dataRate>(static_cast<uint32_t>(currentDataRate) + 1);
    }
}

bandwidth adaptiveDataRate::getBandwidth() const {
    return bandwidth::b125kHz;
}

spreadingFactor adaptiveDataRate::getSpreadingFactor() const {
    switch (currentDataRate) {
        default:
        case dataRate::DR0:
            return spreadingFactor::SF12;
            break;
        case dataRate::DR1:
            return spreadingFactor::SF11;
            break;
        case dataRate::DR2:
            return spreadingFactor::SF10;
            break;
        case dataRate::DR3:
            return spreadingFactor::SF9;
            break;
        case dataRate::DR4:
            return spreadingFactor::SF8;
            break;
        case dataRate::DR5:
            return spreadingFactor::SF7;
            break;
    }
}
