#include "framecount.h"

frameCount::frameCount(){};

frameCount::frameCount(uint32_t theInitialFrameCount) : theFrameCount(theInitialFrameCount){};

uint8_t frameCount::asUint8(uint32_t index) const {
    if (index > 3) {
        return 0;
    }
    return static_cast<uint8_t>((theFrameCount >> (index * 8)) & 0xFF);
}

void frameCount::fromUint32(uint32_t theNewFrameCount) {
    theFrameCount = theNewFrameCount;
}

uint32_t frameCount::asUint32() const {
    return theFrameCount;
}

void frameCount::increment() {
    theFrameCount++;
}
