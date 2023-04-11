#include "mic.h"

uint8_t messageIntegrityCode::asUint8(uint32_t index) const {
    if (index > 3) {
        return 0;
    }
    return static_cast<uint8_t>((micValue >> (index * 8)) & 0xFF);
}