#include "sx126x.h"

sx126x theRadio;

void doCAD() {

}

void prepareTx() {
    theRadio.setRfFrequency(868000000);
    theRadio.setPacketType(packetType::LoRa);
    theRadio.setTxParameters(14, rampTime::rt200us);
    theRadio.setModulationParameters(spreadingFactor::SF10, bandwidth::b125kHz, codingRate::cr4_5, lowDataRateOptimize::on);
    theRadio.setPacketParameters(8, headerType::explicitHeader, 123, crcType::on, invertIq::normal);
}


void prepareRx() {
    theRadio.setRfFrequency(868000000);
    theRadio.setPacketType(packetType::LoRa);
    theRadio.setModulationParameters(spreadingFactor::SF10, bandwidth::b125kHz, codingRate::cr4_5, lowDataRateOptimize::on);
    theRadio.setPacketParameters(8, headerType::explicitHeader, 123, crcType::on, invertIq::inverted);
}