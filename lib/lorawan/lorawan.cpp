#include <iostream>
#include "lorawan.h"
#include "AES-128.h"
#include "Encrypt.h"
#include "Struct.h"
#include "txrxcycle.h"
#include "sx126x.h"
#include "logging.h"
#include "eventbuffer.h"
#include "txrxcycle.h"
#include "nvs.h"
#include "applicationevent.h"
#include "maccommand.h"

extern nonVolatileStorage nvs;
extern sx126x theRadio;
extern eventBuffer<loRaWanEvent, 16U> loraWanEventBuffer;
extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;

void LoRaWAN::initialize() {
    // TODO : CHECK all of this should come from NVS
    currentDataRateIndex = 5;
    logging::snprintf("dataRateIndex = %u\n", currentDataRateIndex);
    theRadio.initialize();
    DevAddr.set(nvs.readBlock32(static_cast<uint32_t>(nvsMap::blockIndex::DevAddr)));
    logging::snprintf("devAddr = %04X\n", DevAddr.asUint32);

    uint8_t tmpKeyArray[aesKey::binaryKeyLength];
    nvs.readBlock(static_cast<uint32_t>(nvsMap::blockIndex::applicationSessionKey), tmpKeyArray);
    applicationKey.setFromBinary(tmpKeyArray);
    nvs.readBlock(static_cast<uint32_t>(nvsMap::blockIndex::networkSessionKey), tmpKeyArray);
    networkKey.setFromBinary(tmpKeyArray);

    logging::snprintf("applicationKey = %s\n", applicationKey.asASCII());
    logging::snprintf("networkKey = %s\n", networkKey.asASCII());

    uplinkFrameCount.set(nvs.readBlock32(static_cast<uint32_t>(nvsMap::blockIndex::uplinkFrameCounter)));
    logging::snprintf("uplinkFrameCount = %u\n", uplinkFrameCount.asUint32);
    downlinkFrameCount.set(nvs.readBlock32(static_cast<uint32_t>(nvsMap::blockIndex::downlinkFrameCounter)));
    logging::snprintf("downlinkFrameCount = %u\n", downlinkFrameCount.asUint32);
}

void LoRaWAN::handleEvents() {
    while (loraWanEventBuffer.hasEvents()) {
        loRaWanEvent theEvent = loraWanEventBuffer.pop();
        // logging::snprintf("LoRaWAN event [%u / %s] in state [%u / %s] \n", static_cast<uint8_t>(theEvent), toString(theEvent), static_cast<uint8_t>(theTxRxCycleState), toString(theTxRxCycleState));

        switch (theTxRxCycleState) {
            case txRxCycleState::idle:
                // unexpected event for this state.. Radio is OFF and LPTIM is stopped
                break;

            case txRxCycleState::waitForCadEnd:
                switch (theEvent) {
                    case loRaWanEvent::sx126xCadEnd:
                        break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;

            case txRxCycleState::waitForRandomTimeBeforeTransmit:
                switch (theEvent) {
                    case loRaWanEvent::timeOut:
                        // goTo(txRxCycleState::waitForTxComplete);
                        break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;

            case txRxCycleState::waitForTxComplete:
                switch (theEvent) {
                    case loRaWanEvent::sx126xTxComplete:
                        //            startTimer(2048U);        // 1 second
                        startTimer(1024);        // 0.5 second
                        goTo(txRxCycleState::waitForRx1Start);
                        break;
                    case loRaWanEvent::sx126xTimeout:
                        break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;

            case txRxCycleState::waitForRx1Start:
                switch (theEvent) {
                    case loRaWanEvent::timeOut: {
                        // logging::snprintf("RX1 start\n");
                        stopTimer();
                        startTimer(2048U);        // 1 second ffrom now until Rx2Start
                        uint32_t rxFrequency = theChannels.txRxChannels[currentChannelIndex].frequency;
                        uint32_t rxTimeout   = getReceiveTimeout(theDataRates.theDataRates[currentDataRateIndex].theSpreadingFactor);
                        theRadio.configForReceive(theDataRates.theDataRates[currentDataRateIndex].theSpreadingFactor, rxFrequency);
                        theRadio.startReceive(rxTimeout);
                        goTo(txRxCycleState::waitForRx1CompleteOrTimeout);
                    } break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;

            case txRxCycleState::waitForRx1CompleteOrTimeout:
                switch (theEvent) {
                    case loRaWanEvent::sx126xRxComplete: {
                        // logging::snprintf("### Message Received in RX1 :-) ###\n");
                        messageType receivedMessageType = decodeMessage();
                        switch (receivedMessageType) {
                            case messageType::application:
                                applicationEventBuffer.push(applicationEvent::downlinkApplicationPayloadReceived);
                                goTo(txRxCycleState::waitForRxMessageReadout);
                                break;
                            case messageType::lorawanMac:
                                processMacContents();
                                logging::snprintf("### MAC message received : ");
                                for (uint32_t i = 0; i < 30; i++) {
                                    logging::snprintf("0x%02X ", macIn[i]);
                                }
                                logging::snprintf("\n");

                                // handle it directly
                                break;
                            default:
                            case messageType::invalid:
                                goTo(txRxCycleState::waitForRx2Start);
                                break;
                        }
                    } break;
                    case loRaWanEvent::sx126xTimeout:
                        // logging::snprintf("nothing received in RX1\n");
                        goTo(txRxCycleState::waitForRx2Start);
                        break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;

            case txRxCycleState::waitForRx2Start:
                switch (theEvent) {
                    case loRaWanEvent::timeOut: {
                        stopTimer();
                        // logging::snprintf("RX2 start\n");
                        uint32_t rxFrequency = theChannels.rx2Channel.frequency;
                        uint32_t rxTimeout   = getReceiveTimeout(spreadingFactor::SF9);
                        theRadio.configForReceive(spreadingFactor::SF9, rxFrequency);
                        theRadio.startReceive(rxTimeout);
                        goTo(txRxCycleState::waitForRx2CompleteOrTimeout);
                    } break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;

            case txRxCycleState::waitForRx2CompleteOrTimeout:
                switch (theEvent) {
                    case loRaWanEvent::sx126xRxComplete: {
                        // logging::snprintf("### Message Received in RX2 :-) ###\n");
                        messageType receivedMessageType = decodeMessage();
                        switch (receivedMessageType) {
                            case messageType::application:
                                applicationEventBuffer.push(applicationEvent::downlinkApplicationPayloadReceived);
                                goTo(txRxCycleState::waitForRxMessageReadout);
                                break;
                            case messageType::lorawanMac:
                                // handle it directly
                                break;
                            default:
                            case messageType::invalid:
                                goTo(txRxCycleState::idle);
                                break;
                        }
                    } break;
                    case loRaWanEvent::sx126xTimeout:
                        // logging::snprintf("nothing received in RX2\n");
                        goTo(txRxCycleState::idle);
                        break;
                    default:
                        // unexpected event for this state..
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void LoRaWAN::goTo(txRxCycleState newState) {
    logging::snprintf("LoRaWAN stateChange from [%d / %s] to [%d / %s]\n", theTxRxCycleState, toString(theTxRxCycleState), newState, toString(newState));
    switch (theTxRxCycleState) {
        case txRxCycleState::idle:
            break;

        case txRxCycleState::waitForRandomTimeBeforeTransmit:
            stopTimer();
            break;

        case txRxCycleState::waitForTxComplete:
            break;

        case txRxCycleState::waitForRx1Start:
            break;

        case txRxCycleState::waitForRx1CompleteOrTimeout:
            break;

        case txRxCycleState::waitForRx2Start:
            break;

        case txRxCycleState::waitForRx2CompleteOrTimeout:
            break;

        default:
            break;
    }
    theTxRxCycleState = newState;
    switch (newState) {
        case txRxCycleState::idle:
            // theRadio.goSleep();
            break;

        case txRxCycleState::waitForRandomTimeBeforeTransmit: {
            uint32_t randomDelay = getRandomNumber() >> 17;        // TODO : this results in a random number between 0 and 32768, which results in a delay between 0 and ~16 seconds (lptim clocked with 2048 Hz)
            startTimer(randomDelay);
        } break;

        case txRxCycleState::waitForTxComplete:
            break;

        case txRxCycleState::waitForRx1Start:
            break;

        case txRxCycleState::waitForRx1CompleteOrTimeout:
            break;

        case txRxCycleState::waitForRx2Start:
            break;

        case txRxCycleState::waitForRx2CompleteOrTimeout:
            break;

        case txRxCycleState::waitForRxMessageReadout:
            break;

        default:
            break;
    }
}

bool LoRaWAN::isReadyToTransmit() const {
    return (theTxRxCycleState == txRxCycleState::idle);
}

uint32_t LoRaWAN::getMaxApplicationPayloadLength() const {
    return theDataRates.theDataRates[currentDataRateIndex].maximumPayloadLength;
}

void LoRaWAN::insertPayload(byteBuffer& applicationPayloadToSend) {
    for (uint32_t index = 0; index < applicationPayloadToSend.length; index++) {
        rawMessage[framePayloadOffset + index] = applicationPayloadToSend.buffer[index];
    }
}

void LoRaWAN::prepareBlockAi(uint8_t* theBlock, linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t blockIndex) {
    theBlock[0]  = 0x01;                                      //
    theBlock[1]  = 0x00;                                      //
    theBlock[2]  = 0x00;                                      //
    theBlock[3]  = 0x00;                                      //
    theBlock[4]  = 0x00;                                      //
    theBlock[5]  = static_cast<uint8_t>(theDirection);        //
    theBlock[6]  = anAddress.asUint8[0];                      // LSByte
    theBlock[7]  = anAddress.asUint8[1];                      //
    theBlock[8]  = anAddress.asUint8[2];                      //
    theBlock[9]  = anAddress.asUint8[3];                      // MSByte
    theBlock[10] = aFrameCounter.asUint8[0];                  // LSByte
    theBlock[11] = aFrameCounter.asUint8[1];                  //
    theBlock[12] = aFrameCounter.asUint8[2];                  //
    theBlock[13] = aFrameCounter.asUint8[3];                  // MSByte
    theBlock[14] = 0x00;                                      //
    theBlock[15] = blockIndex;                                // Blocks Ai are indexed from 1..k, where k is the number of blocks
}

void LoRaWAN::encryptPayload(aesKey& theKey) {
    uint32_t nmbrOfBlocks{framePayloadLength / 16};                 // Split payload in blocks of 16 bytes, last part could be less than 16 bytes - integer division

    bool hasIncompleteBlock{(framePayloadLength % 16) != 0};        // Check if last block is incomplete
    uint32_t incompleteBlockSize{0};
    if (hasIncompleteBlock) {
        nmbrOfBlocks++;                                       // even if the last block is less than 16 bytes, we include it
        incompleteBlockSize = framePayloadLength % 16;        // how long is the last block
    }

    uint8_t theBlock[16];        // 16 bytes, which will be filled with certain values from LoRaWAN context, and then encrypted

    for (uint32_t blockIndex = 0x00; blockIndex < nmbrOfBlocks; blockIndex++) {
        //        prepareBlockAi(theBlock, blockIndex, linkDirection::uplink);
        prepareBlockAi(theBlock, linkDirection::uplink, DevAddr, uplinkFrameCount, (blockIndex + 1));
        AES_Encrypt(theBlock, theKey.asUnsignedChar());

        if (hasIncompleteBlock && (blockIndex == (nmbrOfBlocks - 1))) {
            for (uint32_t byteIndex = 0; byteIndex < incompleteBlockSize; byteIndex++) {
                rawMessage[(blockIndex * 16) + byteIndex + framePayloadOffset] ^= theBlock[byteIndex];
            }
        } else {
            for (uint32_t byteIndex = 0; byteIndex < 16; byteIndex++) {
                rawMessage[(blockIndex * 16) + byteIndex + framePayloadOffset] ^= theBlock[byteIndex];
            }
        }
    }
}

void LoRaWAN::decryptPayload(aesKey& theKey) {
    uint32_t nmbrOfBlocks{framePayloadLength / 16};                 // Split payload in blocks of 16 bytes, last part could be less than 16 bytes - integer division

    bool hasIncompleteBlock{(framePayloadLength % 16) != 0};        // Check if last block is incomplete
    uint32_t incompleteBlockSize{0};
    if (hasIncompleteBlock) {
        nmbrOfBlocks++;                                       // even if the last block is less than 16 bytes, we include it
        incompleteBlockSize = framePayloadLength % 16;        // how long is the last block
    }

    uint8_t theBlock[16];        // 16 bytes, which will be filled with certain values from LoRaWAN context, and then encrypted

    for (uint32_t blockIndex = 0x00; blockIndex < nmbrOfBlocks; blockIndex++) {
        prepareBlockAi(theBlock, linkDirection::downlink, DevAddr, downlinkFrameCount, (blockIndex + 1));
        AES_Encrypt(theBlock, theKey.asUnsignedChar());

        if (hasIncompleteBlock && (blockIndex == (nmbrOfBlocks - 1))) {
            for (uint32_t byteIndex = 0; byteIndex < incompleteBlockSize; byteIndex++) {
                rawMessage[(blockIndex * 16) + byteIndex + framePayloadOffset] ^= theBlock[byteIndex];
            }
        } else {
            for (uint32_t byteIndex = 0; byteIndex < 16; byteIndex++) {
                rawMessage[(blockIndex * 16) + byteIndex + framePayloadOffset] ^= theBlock[byteIndex];
            }
        }
    }
}

void LoRaWAN::insertHeaders(framePort theFramePort) {
    // 3. put the header (MHDR - FHDR - FPORT) in front of the payload..
    // As we never send MAC commands in FOptions, we know this is always 8 bytes : MHDR(1) - DEVADDR(4) - FCTRL(1) - FCNT(2)
    rawMessage[macHeaderOffset]     = macHeader(frameType::unconfirmedDataUp).asUint8();        //
    rawMessage[macHeaderOffset + 1] = DevAddr.asUint8[0];                                       //
    rawMessage[macHeaderOffset + 2] = DevAddr.asUint8[1];                                       //
    rawMessage[macHeaderOffset + 3] = DevAddr.asUint8[2];                                       //
    rawMessage[macHeaderOffset + 4] = DevAddr.asUint8[3];                                       //
    rawMessage[macHeaderOffset + 5] = 0;                                                        // FCTRL : all zero bits for the time being.. TODO : later we will need to add ACK etc..
    rawMessage[macHeaderOffset + 6] = uplinkFrameCount.asUint8[0];                              // only the 2 LSBytes from framecounter are sent in the header
    rawMessage[macHeaderOffset + 7] = uplinkFrameCount.asUint8[1];                              //
    rawMessage[macHeaderOffset + 8] = theFramePort;                                             //
}

void LoRaWAN::insertBlockB0(linkDirection theDirection, deviceAddress& aDeviceAddress, frameCount& aFrameCounter, uint32_t micPayloadLength) {
    rawMessage[0]  = 0x49;                                      // see LoRaWAN® L2 1.0.4 Specification - line 808
    rawMessage[1]  = 0;                                         //
    rawMessage[2]  = 0;                                         //
    rawMessage[3]  = 0;                                         //
    rawMessage[4]  = 0;                                         //
    rawMessage[5]  = static_cast<uint8_t>(theDirection);        // 0x00 for uplink, 0x01 for downlink
    rawMessage[6]  = aDeviceAddress.asUint8[0];                 // LSByte
    rawMessage[7]  = aDeviceAddress.asUint8[1];                 //
    rawMessage[8]  = aDeviceAddress.asUint8[2];                 //
    rawMessage[9]  = aDeviceAddress.asUint8[3];                 // MSByte
    rawMessage[10] = aFrameCounter.asUint8[0];                  // LSByte
    rawMessage[11] = aFrameCounter.asUint8[1];                  //
    rawMessage[12] = aFrameCounter.asUint8[2];                  //
    rawMessage[13] = aFrameCounter.asUint8[3];                  // MSByte
    rawMessage[14] = 0;                                         //
    rawMessage[15] = micPayloadLength;                          // number of bytes over which the MIC is applied, is frameHeader + application payload
}

void LoRaWAN::insertMic() {
    uint8_t result[16]{0};
    sBuffer sourceData;
    sourceData.Data    = rawMessage;
    sourceData.Counter = b0BlockLength + macHeaderLength + macPayloadLength;

    Calculate_MIC2(&sourceData, networkKey.asUnsignedChar(), result);
    rawMessage[micOffset]     = result[0];        // LSByte
    rawMessage[micOffset + 1] = result[1];        //
    rawMessage[micOffset + 2] = result[2];        //
    rawMessage[micOffset + 3] = result[3];        // MSByte
}

bool LoRaWAN::isValidMic() {
    uint8_t result[16]{0};
    sBuffer sourceData;
    sourceData.Data    = rawMessage;
    sourceData.Counter = b0BlockLength + macHeaderLength + macPayloadLength;

    Calculate_MIC2(&sourceData, networkKey.asUnsignedChar(), result);
    if (rawMessage[micOffset] != result[0]) {
        return false;
    }
    if (rawMessage[micOffset + 1] != result[1]) {
        return false;
    }
    if (rawMessage[micOffset + 2] != result[2]) {
        return false;
    }
    if (rawMessage[micOffset + 3] != result[3]) {
        return false;
    }
    return true;
}

void LoRaWAN::sendUplink(byteBuffer& applicationPayloadToSend, framePort theFramePort) {
    // 1. Convert the application payload, to a LoRa(WAN) payload
    setOffsetsAndLengthsTx(applicationPayloadToSend.length);
    insertPayload(applicationPayloadToSend);
    encryptPayload(applicationKey);
    insertHeaders(theFramePort);
    insertBlockB0(linkDirection::uplink, DevAddr, uplinkFrameCount, (macHeaderLength + macPayloadLength));
    insertMic();

    // 2. Configure the radio, and transmit the payload
    currentChannelIndex  = theChannels.getRandomChannelIndex();
    uint32_t txFrequency = theChannels.txRxChannels[currentChannelIndex].frequency;
    spreadingFactor csf  = theDataRates.theDataRates[currentDataRateIndex].theSpreadingFactor;
    theRadio.configForTransmit(csf, txFrequency, rawMessage + macHeaderOffset, loRaPayloadLength);
    uplinkFrameCount.increment();
    nvs.writeBlock32(static_cast<uint32_t>(nvsMap::blockIndex::uplinkFrameCounter), uplinkFrameCount.asUint32);
    theRadio.startTransmit(0);
    goTo(txRxCycleState::waitForTxComplete);
}

void LoRaWAN::getDownlinkMessage(byteBuffer& applicationPayloadReceived) {
    // theRadio.getReceivedMessage();
    //  downlinkMessage.processDownlinkMessage(applicationPayloadReceived);
}

messageType LoRaWAN::decodeMessage() {
    // 1. read the raw message from the radio receiveBuffer
    uint8_t response[2];
    theRadio.executeGetCommand(sx126xCommand::getRxBufferStatus, response, 2);
    loRaPayloadLength = response[0];
    theRadio.readBuffer(rawMessage + b0BlockLength, loRaPayloadLength);        // This reads the full LoRaWAN payload into rawMessage buffer, at an offset so the B0 block still fits in front
    setOffsetsAndLengthsRx(loRaPayloadLength);

    // 2. Extract & guess the downLinkFrameCount, as we need this to check the MIC
    uint16_t receivedDownlinkFramecount = getReceivedFramecount();
    uint32_t lastDownlinkFramecount     = downlinkFrameCount.asUint32;
    uint32_t guessedDownlinkFramecount  = frameCount::guessFromUint16(lastDownlinkFramecount, receivedDownlinkFramecount);
    frameCount tmpDownLinkFrameCount(guessedDownlinkFramecount);

    // 3. Check the MIC
    insertBlockB0(linkDirection::downlink, DevAddr, tmpDownLinkFrameCount, loRaPayloadLength - micLength);
    if (!isValidMic()) {
        logging::snprintf("rxError : invalid MIC\n");
        return messageType::invalid;
    }

    // 4. Extract the deviceAddress, to check if packet is addressed to this node
    deviceAddress receivedDeviceAddress(rawMessage + deviceAddressOffset);
    if (!isValidDevAddr(receivedDeviceAddress.asUint32)) {
        logging::snprintf("rx : msg not for this device\n");        // TODO : also log received deviceAddress
        return messageType::invalid;
    }

    // 5. check if the frameCount is valid
    if (!isValidDownlinkFrameCount(tmpDownLinkFrameCount)) {
        logging::snprintf("rxError : invalid downlinkFrameCount\n");        // TODO : also log current and received frameCount
        return messageType::invalid;
    }

    // 6 Seems a valid message, so update the downlinkFrameCount to what we've received (not just incrementing it, as there could be gaps in the sequence due to lost packets)
    downlinkFrameCount.set(tmpDownLinkFrameCount.asUint32);
    nvs.writeBlock32(static_cast<uint32_t>(nvsMap::blockIndex::downlinkFrameCounter), downlinkFrameCount.asUint32);

    // 6. All clear, so decrypt the payload
    // TODO : there could be MAC requests/responses in the frameOptions, we need to process these as well..

    if (rawMessage[framePortOffset] == 0) {
        decryptPayload(networkKey);
        memcpy(macIn, rawMessage + framePayloadOffset, framePayloadLength);
        macInLevel = framePayloadLength;
        return messageType::lorawanMac;
    } else {
        decryptPayload(applicationKey);
        return messageType::application;
    }
}

uint32_t LoRaWAN::getReceiveTimeout(spreadingFactor aSpreadingFactor) {
    static constexpr uint32_t baseTimeout{640};
    switch (aSpreadingFactor) {
        case spreadingFactor::SF5:
            return baseTimeout;
        case spreadingFactor::SF6:
            return 2 * 2 * baseTimeout;
        case spreadingFactor::SF7:
            return 4 * baseTimeout;
        case spreadingFactor::SF8:
            return 8 * baseTimeout;
        case spreadingFactor::SF9:
            return 16 * baseTimeout;
        case spreadingFactor::SF10:
            return 32 * baseTimeout;
        case spreadingFactor::SF11:
            return 64 * baseTimeout;
        case spreadingFactor::SF12:
        default:
            return 128 * baseTimeout;
    }
}

bool LoRaWAN::isValidDevAddr(deviceAddress testAddress) {
    return (testAddress.asUint32 == DevAddr.asUint32);
}

bool LoRaWAN::isValidDownlinkFrameCount(frameCount testFrameCount) {
    return (testFrameCount.asUint32 > downlinkFrameCount.asUint32);
}

void LoRaWAN::setOffsetsAndLengthsTx(uint32_t theFramePayloadLength) {
    // This function is used in the uplink direction, where we know the length of the application payload, and we construct a LoRa payload from it
    framePayloadLength = theFramePayloadLength;
    frameOptionsLength = 0;
    framePortOffset    = b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength;
    framePayloadOffset = b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength;
    micOffset          = b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength + framePayloadLength;
    loRaPayloadLength  = macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength + theFramePayloadLength + micLength;
    frameHeaderLength  = deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength;
    macPayloadLength   = frameHeaderLength + framePortLength + framePayloadLength;
}

void LoRaWAN::setOffsetsAndLengthsRx(uint32_t theLoRaPayloadLength) {
    // This function is used in the downlink, where we received a certain LoRa payload, and we decode it to identify all the fields inside it
    loRaPayloadLength  = theLoRaPayloadLength;
    frameOptionsLength = rawMessage[frameControlOffset] & 0x0F;
    framePortOffset    = b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength;
    framePayloadOffset = b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength;
    frameHeaderLength  = deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength;
    framePayloadLength = theLoRaPayloadLength - (macHeaderLength + frameHeaderLength + framePortLength + micLength);
    macPayloadLength   = frameHeaderLength + framePortLength + framePayloadLength;
    micOffset          = b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength + framePayloadLength;
}

void LoRaWAN::processMacContents() {
    while (macInLevel > 0) {
        macCommand theMacCommand = static_cast<macCommand>(macIn[0]);

        switch (theMacCommand) {
            case macCommand::linkCheckAnswer:
                // consume 3 bytes : id, margin, gwCount
                break;

            case macCommand::deviceStatusRequest:
                // consume 1 byte
                // append macCommand::deviceStatusAnswer
                // append 2 bytes : batteryLevel, radioStatus std line 1106
                break;

            case macCommand::newChannelRequest:
                // consume 5 bytes : id, channelIndex[1], frequency[3], DRrange[1]
                // append macCommand::newChannelAnswer
                // append 1 byte : status std line 1172
                break;

            case macCommand::linkAdaptiveDataRateRequest:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            case macCommand::dutyCycleRequest:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            case macCommand::receiveParameterSetupRequest:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            case macCommand::receiveTimingSetupRequest:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            case macCommand::transmitParameterSetupRequest:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            case macCommand::downlinkChannelRequest:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            case macCommand::deviceTimeAnswer:
                // consume x bytes :
                // append macCommand::
                // append y bytes :
                break;

            default:
                // unknown command.. abort further processing as each command has a different length and so we can not skip this unknown command
                break;
        }
    }
}

#ifndef environment_desktop

#include "main.h"

extern RNG_HandleTypeDef hrng;
extern LPTIM_HandleTypeDef hlptim1;

uint32_t LoRaWAN::getRandomNumber() {
    uint32_t result{0};
    HAL_RNG_GenerateRandomNumber(&hrng, &result);
    return result;
}

void LoRaWAN::startTimer(uint32_t timeOut) {
    HAL_LPTIM_SetOnce_Start_IT(&hlptim1, 0xFFFF, timeOut);
}

void LoRaWAN::stopTimer() {
    HAL_LPTIM_SetOnce_Stop_IT(&hlptim1);
}

#else

void LoRaWAN::startTimer(uint32_t timeOut) {
}

void LoRaWAN::stopTimer() {
}

uint32_t LoRaWAN::getRandomNumber() {
    uint32_t result{0};
    result = rand();
    return result;
}

#endif
