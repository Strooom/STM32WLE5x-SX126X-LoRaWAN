#include "lorawan.h"
#include <iostream>
#include "AES-128.h"
#include "Encrypt.h"
#include "Struct.h"
#include "txrxcycle.h"
#include "sx126x.h"
#include "logging.h"
#include "eventbuffer.h"
#include "txrxcycle.h"

extern sx126x theRadio;
extern logging theLog;
extern eventBuffer<loRaWanEvent, 16U> loraWanEventBuffer;

// extern nonVolatileStorage nvs;

LoRaWAN::LoRaWAN() {
}

void LoRaWAN::handleEvents() {
    if (!loraWanEventBuffer.hasEvents()) {
        return;
    }
    loRaWanEvent theEvent = loraWanEventBuffer.pop();

    switch (theTxRxCycleState) {
        case txRxCycleState::idle:
            // unexpected event for this state..
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
                    break;
                default:
                    // unexpected event for this state..
                    break;
            }
            break;

        case txRxCycleState::waitForTxComplete:
            switch (theEvent) {
                case loRaWanEvent::sx126xTxComplete:
                    // Start Timer for Rx1Start
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
                case loRaWanEvent::timeOut:
                    // Start Timer for Rx2Start
                    break;
                default:
                    // unexpected event for this state..
                    break;
            }
            break;

        case txRxCycleState::waitForRx1CompleteOrTimeout:
            switch (theEvent) {
                case loRaWanEvent::sx126xRxComplete:
                    break;
                case loRaWanEvent::sx126xTimeout:
                    break;
                default:
                    // unexpected event for this state..
                    break;
            }
            break;

        case txRxCycleState::waitForRx2Start:
            switch (theEvent) {
                case loRaWanEvent::timeOut:
                    break;
                default:
                    // unexpected event for this state..
                    break;
            }
            break;

        case txRxCycleState::waitForRx2CompleteOrTimeout:
            switch (theEvent) {
                case loRaWanEvent::sx126xRxComplete:
                    break;
                case loRaWanEvent::sx126xTimeout:
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

void LoRaWAN::goTo(txRxCycleState newState) {
    theLog.snprintf("LoRaWAN stateChange from %s[%d] to %s[%d]", toString(theTxRxCycleState), theTxRxCycleState, toString(newState), newState);
    exitState(theTxRxCycleState);
    theTxRxCycleState = newState;
    enterState(newState);
}

void LoRaWAN::exitState(txRxCycleState currentState) {
    switch (currentState) {
        case txRxCycleState::idle:
            break;

        case txRxCycleState::waitForRandomTimeBeforeTransmit:
            stopTimer();
            break;

        case txRxCycleState::waitForTxComplete:
            break;

        case txRxCycleState::waitForRx1Start:
            stopTimer();
            break;

        case txRxCycleState::waitForRx1CompleteOrTimeout:
            break;

        case txRxCycleState::waitForRx2Start:
            stopTimer();
            break;

        case txRxCycleState::waitForRx2CompleteOrTimeout:
            break;

        default:
            break;
    }
}

void LoRaWAN::enterState(txRxCycleState newState) {
    switch (newState) {
        case txRxCycleState::idle:
            break;

        case txRxCycleState::waitForRandomTimeBeforeTransmit: {
            uint32_t randomDelay = getRandomNumber() >> 17;        // TODO : this results in a random number between 0 and 32768, which results in a delay between 0 and ~16 seconds (lptim clocked with 2048 Hz)
            startTimer(randomDelay);
        } break;

        case txRxCycleState::waitForTxComplete:
            break;

        case txRxCycleState::waitForRx1Start:
            startTimer(2048U);        // 1 second
            break;

        case txRxCycleState::waitForRx1CompleteOrTimeout:
            break;

        case txRxCycleState::waitForRx2Start:
            startTimer(2048U);        // 1 more second
            break;

        case txRxCycleState::waitForRx2CompleteOrTimeout:
            break;

        default:
            break;
    }
}

bool LoRaWAN::isReadyToTransmit() const {
    return true;        // TODO : add real implementation, taking into account the duty-cycle, being joined, etc
    // For the time being, we keep it simple and just return true
}

uint32_t LoRaWAN::getMaxApplicationPayloadLength() const {
    return 50;        // TODO : add real implementation, taking into account the region, data rate, etc
}

void LoRaWAN::copyPayload(byteBuffer& applicationPayloadToSend) {
    for (uint32_t index = 0; index < applicationPayloadToSend.length; index++) {
        rawMessage[payloadOffset + index] = applicationPayloadToSend.buffer[index];
    }
    payloadLength = applicationPayloadToSend.length;
}

void LoRaWAN::prepareBlockAi(uint8_t* theBlock, uint32_t blockIndex, linkDirection theDirection) {
    theBlock[0] = 0x01;
    theBlock[1] = 0x00;
    theBlock[2] = 0x00;
    theBlock[3] = 0x00;
    theBlock[4] = 0x00;

    theBlock[5] = static_cast<uint8_t>(theDirection);

    theBlock[6] = DevAddr.asUint8(0);        // LSByte
    theBlock[7] = DevAddr.asUint8(1);
    theBlock[8] = DevAddr.asUint8(2);
    theBlock[9] = DevAddr.asUint8(3);        // MSByte

    if (theDirection == linkDirection::uplink) {
        theBlock[10] = uplinkFrameCount.asUint8(0);        // LSByte
        theBlock[11] = uplinkFrameCount.asUint8(1);
        theBlock[12] = uplinkFrameCount.asUint8(2);
        theBlock[13] = uplinkFrameCount.asUint8(3);        // MSByte
    } else {
        theBlock[10] = downlinkFrameCount.asUint8(0);
        theBlock[11] = downlinkFrameCount.asUint8(1);
        theBlock[12] = downlinkFrameCount.asUint8(2);
        theBlock[13] = downlinkFrameCount.asUint8(3);
    }

    theBlock[14] = 0x00;

    theBlock[15] = blockIndex + 1;        // Blocks Ai are indexed from 1..k, where k is the number of blocks

    std::cout << "Block A" << (blockIndex + 1) << " : ";
    for (uint32_t index = 0; index < 16; index++) {
        std::cout << std::hex << (int)theBlock[index] << " ";
    }
    std::cout << std::endl;
}

void LoRaWAN::encryptPayload(aesKey& theKey) {
    uint32_t nmbrOfBlocks{payloadLength / 16};        // Split payload in blocks of 16 bytes, last part could be less than 16 bytes - integer division

    bool hasIncompleteBlock{(payloadLength % 16) != 0};        // Check if last block is incomplete
    uint32_t incompleteBlockSize{0};
    if (hasIncompleteBlock) {
        nmbrOfBlocks++;                                  // even if the last block is less than 16 bytes, we include it
        incompleteBlockSize = payloadLength % 16;        // how long is the last block
    }

    uint8_t theBlock[16];        // 16 bytes, which will be filled with certain values from LoRaWAN context, and then encrypted

    for (uint32_t blockIndex = 0x00; blockIndex < nmbrOfBlocks; blockIndex++) {
        prepareBlockAi(theBlock, blockIndex, linkDirection::uplink);
        AES_Encrypt(theBlock, theKey.asUnsignedChar());

        if (hasIncompleteBlock && (blockIndex == (nmbrOfBlocks - 1))) {
            for (uint32_t byteIndex = 0; byteIndex < incompleteBlockSize; byteIndex++) {
                rawMessage[(blockIndex * 16) + byteIndex + payloadOffset] ^= theBlock[byteIndex];
            }
        } else {
            for (uint32_t byteIndex = 0; byteIndex < 16; byteIndex++) {
                rawMessage[(blockIndex * 16) + byteIndex + payloadOffset] ^= theBlock[byteIndex];
            }
        }
    }
}

void LoRaWAN::prependHeader(framePort theFramePort) {
    // 3. put the header (MHDR - FHDR - FPORT) in front of the payload..
    // As we never send MAC commands in FOptions, we know this is always 8 bytes : MHDR(1) - DEVADDR(4) - FCTRL(1) - FCNT(2)
    rawMessage[headerOffset] = macHeader(frameType::unconfirmedDataUp).asUint8();        //

    rawMessage[headerOffset + 1] = DevAddr.asUint8(0);        //
    rawMessage[headerOffset + 2] = DevAddr.asUint8(1);        //
    rawMessage[headerOffset + 3] = DevAddr.asUint8(2);        //
    rawMessage[headerOffset + 4] = DevAddr.asUint8(3);        //

    rawMessage[headerOffset + 5] = 0;                                  // FCTRL : all zero bits for the time being.. TODO : later we will need to add ACK etc..
    rawMessage[headerOffset + 6] = uplinkFrameCount.asUint8(0);        // only the 2 LSBytes from framecounter are sent in the header
    rawMessage[headerOffset + 7] = uplinkFrameCount.asUint8(1);        //

    rawMessage[headerOffset + 8] = theFramePort;        //

    payloadLength += headerLength;
}

void LoRaWAN::prepareBlockB0(uint32_t applicationPayloadLength, linkDirection theDirection) {
    // 4. Now prepare a so-called Block B0, which is used for the MIC calculation
    rawMessage[0] = 0x49;                      // see LoRaWAN® L2 1.0.4 Specification - line 808
    rawMessage[1] = 0;                         //
    rawMessage[2] = 0;                         //
    rawMessage[3] = 0;                         //
    rawMessage[4] = 0;                         //
    rawMessage[5] = 0;                         // 0 for uplink frames, 1 for downlink frames
    rawMessage[6] = DevAddr.asUint8(0);        // LSByte
    rawMessage[7] = DevAddr.asUint8(1);        //
    rawMessage[8] = DevAddr.asUint8(2);        //
    rawMessage[9] = DevAddr.asUint8(3);        // MSByte
    if (theDirection == linkDirection::uplink) {
        rawMessage[10] = uplinkFrameCount.asUint8(0);
        rawMessage[11] = uplinkFrameCount.asUint8(1);
        rawMessage[12] = uplinkFrameCount.asUint8(2);
        rawMessage[13] = uplinkFrameCount.asUint8(3);
    } else {
        rawMessage[10] = downlinkFrameCount.asUint8(0);
        rawMessage[11] = downlinkFrameCount.asUint8(1);
        rawMessage[12] = downlinkFrameCount.asUint8(2);
        rawMessage[13] = downlinkFrameCount.asUint8(3);
    }
    rawMessage[14] = 0;                                              //
    rawMessage[15] = applicationPayloadLength + headerLength;        // application payload + 8 bytes header TODO : check for byte overflow..

    std::cout << "Block B0 : ";
    for (uint32_t index = 0; index < 16; index++) {
        std::cout << std::hex << (int)rawMessage[index] << " ";
    }
    std::cout << std::endl;
}

void LoRaWAN::calculateAndAppendMic() {
    uint8_t result[16]{0};

    sBuffer sourceData;
    sourceData.Data    = rawMessage;
    sourceData.Counter = payloadLength + micBLockLength;

    std::cout << "MIC Source Data : ";
    for (uint32_t index = 0; index < sourceData.Counter; index++) {
        std::cout << std::hex << (int)sourceData.Data[index] << " ";
    }
    std::cout << std::endl;

    sLoRa_Message* Message;
    // Calculate_MIC(&sourceData, networkKey.asUnsignedChar(), Message);

    Calculate_MIC2(&sourceData, networkKey.asUnsignedChar(), result);
    rawMessage[headerOffset + payloadLength]     = result[0];        // LSByte
    rawMessage[headerOffset + payloadLength + 1] = result[1];        //
    rawMessage[headerOffset + payloadLength + 2] = result[2];        //
    rawMessage[headerOffset + payloadLength + 3] = result[3];        // MSByte
    payloadLength += 4;
}

void LoRaWAN::sendUplink(byteBuffer& applicationPayloadToSend, framePort theFramePort) {
    if (applicationPayloadToSend.length > getMaxApplicationPayloadLength()) {
        return;        // TODO : log error : trying to send too much data
    }

    copyPayload(applicationPayloadToSend);

    // std::cout << "payload before encryption : ";
    // for (uint32_t index = 0; index < payloadLength; index++) {
    //     std::cout << std::hex << (int)rawMessage[payloadOffset + index] << " ";
    // }
    // std::cout << std::endl;

    encryptPayload(applicationKey);

    // std::cout << "payload after encryption : ";
    // for (uint32_t index = 0; index < payloadLength; index++) {
    //     std::cout << std::hex << (int)rawMessage[payloadOffset + index] << " ";
    // }
    // std::cout << std::endl;

    prependHeader(theFramePort);

    // std::cout << "header prepended : ";
    // for (uint32_t index = 0; index < (payloadLength); index++) {
    //     std::cout << std::hex << (int)rawMessage[headerOffset + index] << " ";
    // }
    // std::cout << std::endl;

    prepareBlockB0(applicationPayloadToSend.length, linkDirection::uplink);
    calculateAndAppendMic();

    // std::cout << "phy payload : ";
    // for (uint32_t index = 0; index < (payloadLength); index++) {
    //     std::cout << std::hex << (int)rawMessage[headerOffset + index] << " ";
    // }
    // std::cout << std::endl;
    // uplinkMessage.transmit();
}

void LoRaWAN::getDownlinkMessage(byteBuffer& applicationPayloadReceived) {
    // downlinkMessage.processDownlinkMessage(applicationPayloadReceived);
}

void LoRaWAN::runMAC() {
}

void LoRaWAN::initialize() {
    theRadio.initialize();
    //(void)nvs.addBlock(16);        // this object needs some of its members stored in non-volatile storage, so we need allocate some space for it
    DevAddr.fromUint32(0x260B3B92);
    applicationKey.setFromASCII("398F459FE521152FD5B014EA44428AC2");
    networkKey.setFromASCII("680AB79064FD273E52FBBF4FC6349B13");
    uplinkFrameCount.fromUint32(0x1UL);
}