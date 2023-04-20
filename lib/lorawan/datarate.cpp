#include "datarate.h"


uint32_t getDownlinkDataRateIndex(uint32_t uplinkDataRateIndex, uint32_t Rx1DataRateOffset) {
    if (uplinkDataRateIndex > Rx1DataRateOffset) {
        return uplinkDataRateIndex - Rx1DataRateOffset;
    }
    return 0;
}
