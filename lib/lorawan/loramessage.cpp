#include "loramessage.h"
#include "../sx126x/sx126x.h"

extern sx126x theRadio;

void loraMessage::constructUplinkMessage(frameHeader theFrameHeader, framePort theFramePort, byteBuffer& theApplicationPayload, aesKey theKey) {
    theMacHeader.set(frameType::unconfirmedDataUp);       
    theMacPayload.set(theFrameHeader, theFramePort, theApplicationPayload);
    theMIC.calculate(theKey);
}

bool loraMessage::transmit() {
    theRadio.setRfFrequency(868000000);
    theRadio.setPacketType(packetType::LoRa);
    theRadio.setTxParameters(14, rampTime::rt200us);
    theRadio.setModulationParameters(spreadingFactor::SF10, bandwidth::b125kHz, codingRate::cr4_5, lowDataRateOptimize::on);
    theRadio.setPacketParameters(8, headerType::explicitHeader, 123, crcType::on, invertIq::normal);
    theRadio.writeBuffer(rawMessage);
}

bool loraMessage::receive() {}
void loraMessage::processDownlinkMessage(byteBuffer& theApplicationPayload) {}

void loraMessage::setHeader() {}
void loraMessage::addMIC(const uint8_t* key) {}
bool loraMessage::validateMIC(const uint8_t* key) const {}
