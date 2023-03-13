#include "lorawan.h"
#include "nvs.h"

extern nonVolatileStorage nvs;

void LoRaWAN::handleEvents() {
}

bool LoRaWAN::isReadyToTransmit() const {
    return true;        // TODO : add real implementation, taking into account the duty-cycle, being joined, etc
    // For the time being, we keep it simple and just return true
}

uint32_t LoRaWAN::getMaxApplicationPayloadLength() const {
    return 50;        // TODO : add real implementation, taking into account the region, data rate, etc
}

void LoRaWAN::sendUplink(byteBuffer& applicationPayloadToSend, framePort theFramePort) {
    if (applicationPayloadToSend.length <= getMaxApplicationPayloadLength()) {
        frameControl uplinkFrameControl(linkDirection::uplink); // frameControl is needed as element for the frameHeader
        frameHeader tmpFrameHeader(devAddr, uplinkFrameControl, uplinkFrameCount); // frameHeader will be placed in front of the payload
        encryptPayload(applicationPayloadToSend, applicationKey);
        uplinkMessage.constructUplinkMessage(tmpFrameHeader, theFramePort, applicationPayloadToSend); // assemble the message by putting all elements together
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

void LoRaWAN::initialize() {
    (void)nvs.addBlock(16);        // this object needs some of its members stored in non-volatile storage, so we need allocate some space for it
}

void LoRaWAN::encryptPayload(byteBuffer& payloadToEncrypt, aesKey& theEncryptionKey) {
    // TODO : encrypt the payload with the given key..
    // for application payloads (FPORT > 0), we use the application key
    // for MAC commands (FPort == 0), we use the network key
}