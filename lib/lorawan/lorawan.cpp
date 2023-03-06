#include "lorawan.h"

bool LoRaWAN::isReadyToTransmit() const {
    return true;        // TODO : add real implementation, taking into account the duty-cycle, being joined, etc
    // For the time being, we keep it simple and just return true
}

uint32_t LoRaWAN::getMaxApplicationPayloadLength() const {
    return 50;        // TODO : add real implementation, taking into account the region, data rate, etc
}

void LoRaWAN::sendUplink(byteBuffer& applicationPayloadToSend, framePort theFramePort) {
    if (applicationPayloadToSend.length <= getMaxApplicationPayloadLength()) {
        frameControl uplinkFrameControl(linkDirection::uplink);
        frameHeader tmpFrameHeader(devAddr, uplinkFrameControl, uplinkFrameCount);
        encryptApplicationPayload();
        uplinkMessage.constructUplinkMessage(tmpFrameHeader, theFramePort, applicationPayloadToSend, key1);
        uplinkMessage.transmit();
        // TODO : update the LoRaWAN state and store it in Non-Volatile Storage
    } else {
        // TODO : log error : trying to send too much data
    }
}

void LoRaWAN::getDownlinkMessage(byteBuffer& applicationPayloadReceived) {
    downlinkMessage.processDownlinkMessage(applicationPayloadReceived);
}

void LoRaWAN::runMAC() {
}