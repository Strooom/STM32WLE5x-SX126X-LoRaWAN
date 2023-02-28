#include "loramessage.h"

void loraMessage::constructUplinkMessage(frameHeader theFrameHeader, framePort theFramePort, byteBuffer& theApplicationPayload, aesKey theKey) {
    theMacHeader.set(frameType::unconfirmedDataUp);       
    theMacPayload.set(theFrameHeader, theFramePort, theApplicationPayload);
    theMIC.calculate(theKey);
}
bool loraMessage::transmit() {}

bool loraMessage::receive() {}
void loraMessage::processDownlinkMessage(byteBuffer& theApplicationPayload) {}

void loraMessage::setHeader() {}
void loraMessage::addMIC(const uint8_t* key) {}
bool loraMessage::validateMIC(const uint8_t* key) const {}
