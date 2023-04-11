#include "deviceaddress.h"

deviceAddress::deviceAddress() {
}

deviceAddress::deviceAddress(uint32_t theDeviceAddress) {
}

uint32_t deviceAddress::asUint32() const {
    return theDeviceAddress;
}
uint8_t deviceAddress::asUint8(uint32_t index) const {
    if (index > 3) {
        return 0;
    }
    return static_cast<uint8_t>((theDeviceAddress >> (index * 8)) & 0xFF);
}
void deviceAddress::fromUint32(uint32_t theNewDeviceAddress) {
    theDeviceAddress = theNewDeviceAddress;
}
