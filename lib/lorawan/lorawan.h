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

    macHeader MHDR;               // macHeader used for constructing and parsing LoRaWAN messages
    deviceAddress DevAddr;        // device address
    aesKey applicationKey;
    aesKey networkKey;
    frameCount uplinkFrameCount;
    frameCount downlinkFrameCount;

    dutyCycle theDutyCycle;
    messageIntegrityCode mic;

    dataRates theDataRates;
    uint32_t currentDataRateIndex{0};
    loRaChannelCollection theChannels;
    uint32_t currentChannelIndex{0};
    transmitPower theTransmitPower{transmitPower::max};

    uint8_t rawMessage[272]{};                                                                                         // 256 bytes as this is the maximum we can send to the S126x radio. 16 extra bytes in front, needed to calculate the MIC (they are not part of the message sent to the radio)
    uint32_t payloadLength{0};                                                                                         // length of the application payload

    static constexpr uint32_t macHeaderLength{1};                                                                      // total length of MHDR in [bytes]
    static constexpr uint32_t deviceAddressLength{4};                                                                  // total length of DevAddr in [bytes]
    static constexpr uint32_t frameControlLength{1};                                                                   //
    static constexpr uint32_t framecounterLength{2};                                                                   // truncated to 2 LSbytes only
    static constexpr uint32_t frameHeaderLength{deviceAddressLength + frameControlLength + framecounterLength};        // total length of FHDR in [bytes] - assuming no FOpts as I will always send MAC commands in a dedicated message on FPort = 0
    static constexpr uint32_t framePortLength{1};                                                                      // total length of FPort in [bytes]
    static constexpr uint32_t headerLength{macHeaderLength + frameHeaderLength + framePortLength};                     // total length of all the above
    static constexpr uint32_t micBLockLength{16};                                                                      // length of the so-called B0 block, prepended to payload to calculate MIC
    static constexpr uint32_t headerOffset{micBLockLength};                                                            // offset in the rawMessage where the header starts
    static constexpr uint32_t payloadOffset{micBLockLength + headerLength};                                            // offset in the rawMessage where the payload starts

    void copyPayload(byteBuffer& applicationPayloadToSend);                                                            // copy application payload to correct position in the rawMessage buffer
    void encryptPayload(aesKey& theKey);                                                                               // encrypt the payload in the rawMessage buffer
    void decryptPayload(aesKey& theKey);                                                                               // decrypt the payload in the rawMessage buffer
    void prependHeader(framePort theFramePort);                                                                        // prepend the header to the rawMessage buffer
    void prepareBlockB0(uint32_t applicationPayloadLength, linkDirection theDirection);                                // prepare the so-called B0 block, prepended to payload to calculate MIC
    void prepareBlockB0(linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t micPayloadLength);
    void calculateAndAppendMic();                                                                                      //
    bool calculateAndVerifyMic();                                                                                      //
    void prepareBlockAi(uint8_t* aBlock, uint32_t blockIndex, linkDirection theDirection);
    void prepareBlockAi(uint8_t* aBlock, linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t blockIndex);
    bool isValidDevAddr(deviceAddress testAddress);
    bool isValidDownlinkFrameCount(frameCount testFrameCount);
    uint8_t getFramePort();

    static uint32_t getRandomNumber();
    static void startTimer(uint32_t timeOut);        // timeOut in [ticks] from the 2048 Hz clock driving LPTIM1
    static void stopTimer();

    static uint32_t getReceiveTimeout(spreadingFactor aSpreadingfactor);
};
