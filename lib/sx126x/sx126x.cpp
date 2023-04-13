// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "sx126x.h"
#include "logging.h"	

extern logging theLog;

void sx126x::initialize() {
    initializeInterface();
    initializeRadio();
}

void sx126x::configForTransmit(spreadingFactor theSpreadingFactor, uint32_t frequency, uint8_t* payload, uint32_t payloadLength) {
    theLog.snprintf("Configuring for Transmit : [%s], [%u] Hz [%u] bytes\n", toString(theSpreadingFactor), frequency, payloadLength);
    goStandby();
    setRfSwitch(rfSwitchState::tx);
    setRfFrequency(frequency);
    setModulationParameters(theSpreadingFactor, bandwidth::b125kHz, codingRate::cr4_5);
    setPacketParametersTransmit(payloadLength);
    writeBuffer(payload, payloadLength);
}

void sx126x::configForReceive(spreadingFactor theSpreadingFactor, uint32_t frequency) {
    theLog.snprintf("Configuring for Receive : [%s], [%u] Hz\n", toString(theSpreadingFactor), frequency);
    goStandby();
    setRfSwitch(rfSwitchState::rx);
    setRfFrequency(frequency);
    setModulationParameters(theSpreadingFactor, bandwidth::b125kHz, codingRate::cr4_5);
}

void sx126x::goSleep(sleepMode theSleepMode) {
    setRfSwitch(rfSwitchState::off);
    constexpr uint8_t nmbrCommandParameters{1};
    uint8_t commandParameters[nmbrCommandParameters]{static_cast<uint8_t>(theSleepMode)};
    executeCommand(sx126xCommand::setSleep, commandParameters, nmbrCommandParameters);
}

void sx126x::goStandby(standbyMode theStandbyMode) {
    constexpr uint8_t nmbrCommandParameters{1};
    uint8_t commandParameters[nmbrCommandParameters]{static_cast<uint8_t>(theStandbyMode)};
    executeCommand(sx126xCommand::setStandby, commandParameters, nmbrCommandParameters);
}

void sx126x::startTransmit(uint32_t timeOut) {
    constexpr uint8_t nmbrCommandParameters{3};
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((timeOut >> 16) & 0xFF);
    commandParameters[1] = static_cast<uint8_t>((timeOut >> 8) & 0xFF);
    commandParameters[2] = static_cast<uint8_t>(timeOut & 0xFF);
    executeCommand(sx126xCommand::setTx, commandParameters, nmbrCommandParameters);
}

void sx126x::startReceive(uint32_t timeOut) {
    constexpr uint8_t nmbrCommandParameters{3};
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((timeOut >> 16) & 0xFF);
    commandParameters[1] = static_cast<uint8_t>((timeOut >> 8) & 0xFF);
    commandParameters[2] = static_cast<uint8_t>(timeOut & 0xFF);
    executeCommand(sx126xCommand::setRx, commandParameters, nmbrCommandParameters);
}

void sx126x::setRfFrequency(uint32_t frequencyInHz) {
    constexpr uint8_t nmbrCommandParameters{4};
    uint32_t frequencyRegisterValue = calculateFrequencyRegisterValue(frequencyInHz);
    uint8_t commandParameters[nmbrCommandParameters];
    commandParameters[0] = static_cast<uint8_t>((frequencyRegisterValue >> 24) & 0xFF);        //
    commandParameters[1] = static_cast<uint8_t>((frequencyRegisterValue >> 16) & 0xFF);        //
    commandParameters[2] = static_cast<uint8_t>((frequencyRegisterValue >> 8) & 0xFF);         //
    commandParameters[3] = static_cast<uint8_t>((frequencyRegisterValue)&0xFF);                //
    executeCommand(sx126xCommand::setRfFRequency, commandParameters, nmbrCommandParameters);
}

void sx126x::setModulationParameters(spreadingFactor theSpreadingFactor, bandwidth theBandwidth, codingRate theCodingRate) {
    constexpr uint8_t nmbrCommandParameters{8};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = static_cast<uint8_t>(theSpreadingFactor);        //
    commandParameters[1] = static_cast<uint8_t>(theBandwidth);              //
    commandParameters[2] = static_cast<uint8_t>(theCodingRate);             //
    commandParameters[3] = 0x01;                                            // TODO ?? LowDatarateOptimize // why would you NOT want to do this ??
                                                                            // the remaining 4 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

void sx126x::setPacketParametersTransmit(uint8_t payloadLength) {
    constexpr uint8_t nmbrCommandParameters{9};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = 0x00;                 // MSB for PreambleLength
    commandParameters[1] = 0x08;                 // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    commandParameters[2] = 0x00;                 // Variable length packet (explicit header)
    commandParameters[3] = payloadLength;        // Payload Length
    commandParameters[4] = 0x01;                 // CRC On ??
    commandParameters[5] = 0x00;                 // Standard IQ : for uplink. Downlink requires inverted IQ
                                                 // the remaining 3 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

void sx126x::setPacketParametersReceive() {
    constexpr uint8_t nmbrCommandParameters{9};
    uint8_t commandParameters[nmbrCommandParameters]{0};
    commandParameters[0] = 0x00;        // MSB for PreambleLength
    commandParameters[1] = 0x08;        // LSB for PreambleLength LoRaWAN is fixed to 8 symbols
    commandParameters[2] = 0x00;        // Variable length packet (explicit header)
    commandParameters[3] = 0xFF;        // Maximum Payload Length
    commandParameters[4] = 0x00;        // CRC Off ??
    commandParameters[5] = 0x01;        // Downlink requires inverted IQ
                                        // the remaining 3 bytes are empty 0x00 for LoRa
    executeCommand(sx126xCommand::setPacketParameters, commandParameters, nmbrCommandParameters);
}

uint32_t sx126x::calculateFrequencyRegisterValue(uint32_t rfFrequency) {
    uint64_t tmpResult;
    tmpResult = static_cast<uint64_t>(rfFrequency) << 25;
    tmpResult = tmpResult / crystalFrequency;
    return static_cast<uint32_t>(tmpResult);
}

// void sx126x::setTxParameters(int8_t transmitPowerdBm, rampTime theRamptime) {        // caution - signed int8, negative dBm values are in two's complement
//     constexpr uint8_t nmbrCommandParameters{2};
//     uint8_t commandParameters[nmbrCommandParameters];

//     if (transmitPowerdBm < 0) {
//         commandParameters[0] = 255 - static_cast<uint8_t>(-(transmitPowerdBm + 1));        // magic to turn negative signed value into a raw byte in two's complement
//     } else {
//         commandParameters[0] = static_cast<uint8_t>(transmitPowerdBm);        // if value is not negative, casting it is safe
//     }
//     commandParameters[1] = static_cast<uint8_t>(theRamptime);        // rampTime 200 uS - no info why this value, but this was in the demo application from ST / Semtech
//                                                                      // the remaining 4 bytes are empty 0x00 for LoRa
//     executeCommand(sx126xCommand::setTxParams, commandParameters, nmbrCommandParameters);
// }
