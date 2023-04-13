#include "datarate.h"

void dataRates::initialize() {
    add(spreadingFactor::SF12, bandwidth::b125kHz, 51);
    add(spreadingFactor::SF11, bandwidth::b125kHz, 51);
    add(spreadingFactor::SF10, bandwidth::b125kHz, 51);
    add(spreadingFactor::SF9, bandwidth::b125kHz, 115);
    add(spreadingFactor::SF8, bandwidth::b125kHz, 242);
    add(spreadingFactor::SF7, bandwidth::b125kHz, 242);
}

void dataRates::add(spreadingFactor aSpreadingFactor, bandwidth someBandWidth, uint32_t maxPayloadLength) {
    if (nmbrUsedDataRates < maxNmbrDataRates) {
        theDataRates[nmbrUsedDataRates].theSpreadingFactor   = aSpreadingFactor;
        theDataRates[nmbrUsedDataRates].theBandwidth         = someBandWidth;
        theDataRates[nmbrUsedDataRates].maximumPayloadLength = maxPayloadLength;
        nmbrUsedDataRates++;
    }
}

uint32_t dataRates::getMaximumPayloadLength(uint32_t dataRateIndex) {
    if (dataRateIndex < nmbrUsedDataRates) {
        return theDataRates[dataRateIndex].maximumPayloadLength;

    } else {
        return 0;
    }
}

uint32_t getDownlinkDataRateIndex(uint32_t uplinkDataRateIndex, uint8_t Rx1DataRateOffset) {
    if (uplinkDataRateIndex > Rx1DataRateOffset) {
        return uplinkDataRateIndex - Rx1DataRateOffset;
    }
    return 0;
}
