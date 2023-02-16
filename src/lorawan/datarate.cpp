#include "datarate.h"

uint32_t getMaximumPayloadLength(dataRate aDataRate) {
    switch (aDataRate) {
        case dataRate::DR0:
            return 51;
        case dataRate::DR1:
            return 51;
        case dataRate::DR2:
            return 51;
        case dataRate::DR3:
            return 123;
        case dataRate::DR4:
            return 230;
        case dataRate::DR5:
            return 230;
        case dataRate::DR6:
            return 230;
        case dataRate::DR7:
            return 230;
        default:
            return 0;
    }
}


dataRate getDownlinkDataRate(dataRate uplinkDataRate, uint8_t Rx1DataRateOffset) {
    // uint8_t downlinkDataRate = static_cast<uint8_t>(uplinkDataRate) + Rx1DataRateOffset;
    // if (downlinkDataRate > 7) {
    //     downlinkDataRate = 7;
    // }
    return static_cast<dataRate>(downlinkDataRate);
}