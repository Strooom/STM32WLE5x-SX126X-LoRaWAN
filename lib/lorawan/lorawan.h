// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

#include "frameport.h"
#include "messagetype.h"
#include "txrxcycle.h"
#include "collisionavoidancestrategy.h"
#include "macheader.h"
#include "deviceaddress.h"
#include "aeskey.h"
#include "framecount.h"
#include "dutycycle.h"
#include "mic.h"
#include "datarate.h"
#include "lorachannelcollection.h"
#include "transmitpower.h"
#include "linkdirection.h"
#include "spreadingfactor.h"
#include "bytebuffer.h"
#include "payloadencoder.h"

class LoRaWAN {
  public:
    void initialize();                                      // initialize the LoRaWAN layer
    void handleEvents();                                    // handle timeouts and tx/rxComplete
    bool isReadyToTransmit() const;                         // is the LoRaWAN layer in a state, ready to transmit a message?
    uint32_t getMaxApplicationPayloadLength() const;        // [bytes]

    void sendUplink(byteBuffer& applicationPayloadToSend, framePort aFramePort);

    messageType decodeMessage();        // is it for the application, for the MAC layer, or invalid msg

    void getDownlinkMessage(byteBuffer& applicationPayloadReceived);

#ifndef unitTesting
  private:
#endif
    txRxCycleState theTxRxCycleState{txRxCycleState::idle};        // state variable tracking the TxRxCycle state machine
    void goTo(txRxCycleState newState);                            // move the state of the TxRxCycle state machine - handles exit old state actions and entry new state actions

    collisionAvoidanceStrategy theCollisionAvoidanceStrategy{collisionAvoidanceStrategy::none};

    // macHeader MHDR;
    deviceAddress DevAddr;
    aesKey applicationKey;
    aesKey networkKey;
    frameCount uplinkFrameCount;
    frameCount downlinkFrameCount;

    // dutyCycle theDutyCycle;
    //  messageIntegrityCode mic;

    dataRates theDataRates;
    uint32_t currentDataRateIndex{0};
    loRaChannelCollection theChannels;
    uint32_t currentChannelIndex{0};
    // transmitPower theTransmitPower{transmitPower::max};

    static constexpr uint32_t maxLoRaPayloadLength{256};                         // limited to length of databuffer in SX126x
    static constexpr uint32_t b0BlockLength{16};                                 // length of the so-called B0 block, prepended to payload to calculate MIC

    uint8_t rawMessage[maxLoRaPayloadLength + b0BlockLength]{};                  // 256 bytes as this is the maximum we can send to the S126x radio. 16 extra bytes in front, needed to calculate the MIC (they are not part of the message sent to the radio)
    uint32_t payloadLength{0};                                                   // length of the application payload

    uint32_t loRaPayloadLength{0};                                               // length of the data in the radio Tx/Rx buffer
    uint32_t framePayloadLength{0};                                              // length of the application (or MAC layer) payload. excludes header, port and mic

    static constexpr uint32_t macHeaderLength{1};                                // length of MHDR in [bytes]
    static constexpr uint32_t micLength{messageIntegrityCode::length};           // length of MIC in [bytes]
    static constexpr uint32_t deviceAddressLength{deviceAddress::length};        // length of DevAddr in [bytes]
    static constexpr uint32_t frameControlLength{1};                             //
    static constexpr uint32_t frameCountLSHLength{2};                            // frameCount Least Significant Halve - truncated to 2 LSbytes only
    uint32_t frameOptionsLength{0};                                              // this is not static, it can vary between 0 and 15
    uint32_t frameHeaderLength{0};                                               //
    static constexpr uint32_t framePortLength{1};                                // total length of FPort in [bytes]

    // static constexpr uint32_t frameHeaderLength{deviceAddressLength + frameControlLength + frameCountLSHLength};                                                                 // total length of FHDR in [bytes] - assuming no FOpts as I will always send MAC commands in a dedicated message on FPort = 0
    // static constexpr uint32_t headerLength{macHeaderLength + frameHeaderLength + framePortLength};                                                                               // total length of all the above
    // static constexpr uint32_t payloadOffset{b0BlockLength + headerLength};                                                                                                       // offset in the rawMessage where the payload starts

    static constexpr uint32_t headerOffset{b0BlockLength};                                                                                                                                   // offset in the rawMessage where the header starts
    static constexpr uint32_t deviceAddressOffset{b0BlockLength + macHeaderLength};                                                                                                          // offset in the rawMessage where the deviceAddress is found
    static constexpr uint32_t frameControlOffset{b0BlockLength + macHeaderLength + deviceAddressLength};                                                                                     // offset in the rawMessage where the frameControl is found
    static constexpr uint32_t frameCountOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength};                                                                  // offset in the rawMessage where the frameCountLSH is found
    static constexpr uint32_t frameOptionsOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength};                                          // offset in the rawMessage where the frameOptions are found
    uint32_t framePortOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength};                                         // this is not static, it can vary between 24 and 39
    uint32_t framePayloadOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength};                    // this is not static, it can vary between 25 and 40
    uint32_t micOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength + frameOptionsLength + framePortLength + framePayloadLength};        // this is not static, it can vary between 25 and 40

    uint8_t macIn[256];                                                                                                                                                                      // buffer holding the received MAC requests and/or answers
    uint8_t macOut[256];                                                                                                                                                                     // buffer holding the MAC requests and/or answers to be sent

    void setOffsetsAndLengthsTx(uint32_t framePayloadLength);                                                                                                                                // calculate all offsets and lengths in rawMessage starting from the to be transmitted application payload length
    void setOffsetsAndLengthsRx(uint32_t loRaPayloadLength);                                                                                                                                 // calculate all offsets and lengths in rawMessage by decoding the received LoRa payload

    void copyPayload(byteBuffer& applicationPayloadToSend);                                                                                                                                  // copy application payload to correct position in the rawMessage buffer
    void encryptPayload(aesKey& theKey);                                                                                                                                                     // encrypt the payload in the rawMessage buffer
    void decryptPayload(aesKey& theKey);                                                                                                                                                     // decrypt the payload in the rawMessage buffer
    void prependHeader(framePort theFramePort);                                                                                                                                              // prepend the header to the rawMessage buffer
    void prepareBlockB0(linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t micPayloadLength);
    void calculateAndAppendMic();                                                                                                                                                            //
    bool calculateAndVerifyMic();                                                                                                                                                            //
    void prepareBlockAi(uint8_t* aBlock, linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t blockIndex);
    bool isValidDevAddr(deviceAddress testAddress);
    bool isValidDownlinkFrameCount(frameCount testFrameCount);
    uint8_t getFramePort();

    static uint32_t getRandomNumber();
    static void startTimer(uint32_t timeOut);        // timeOut in [ticks] from the 2048 Hz clock driving LPTIM1
    static void stopTimer();

    static uint32_t getReceiveTimeout(spreadingFactor aSpreadingfactor);

    // uint32_t calculateFramePortOffset(uint32_t theFrameOptionsLength) { return frameOptionsOffset + theFrameOptionsLength; }
    // uint32_t calculateFramePayloadOffset(uint32_t theFrameOptionsLength) { return frameOptionsOffset + theFrameOptionsLength + framePortLength; }
    // uint32_t calculateFrameHeaderLength(uint32_t theFrameOptionsLength) { return deviceAddressLength + frameControlLength + frameCountLSHLength + theFrameOptionsLength; }
    // uint32_t calculateFramePayloadLength(uint32_t theLoRaPayloadLength, uint32_t theFrameOptionsLength) { theLoRaPayloadLength - calculateFrameHeaderLength(theFrameOptionsLength) - framePortLength - micLength; }
    uint16_t getReceivedFramecount() { return (static_cast<uint16_t>(rawMessage[frameCountOffset]) + (static_cast<uint16_t>(rawMessage[frameCountOffset + 1]) << 8)); }
};
