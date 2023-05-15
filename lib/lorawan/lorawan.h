// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include <cstring>

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
#include "bytebuffer2.h"
#include "payloadencoder.h"

class LoRaWAN {
  public:
    void initialize();                                                                                     // initialize the LoRaWAN layer
    void run();                                                                                            // run the LoRaWAN MAC layer
    void handleEvents();                                                                                   // handles events (timeouts and tx/rxComplete events)
    bool isReadyToTransmit() const;                                                                        // is the LoRaWAN layer in a state, ready to transmit a message?
    uint32_t getMaxApplicationPayloadLength() const;                                                       // how many [bytes] can the application send in one message? Depends on active dataRate and amount of MAC stuff waiting to be transmitted
    void sendUplink(framePort aFramePort = 0, const uint8_t data[] = nullptr, uint32_t length = 0);        // send an uplink message
    void getDownlinkMessage();                                       //
    void checkNetwork();
    void removeNonStickyMacStuff();                                                                        // removes all non-sticky MAC stuff from macOut after transmitting them. sticky MAC stuff is kept until a downlink is received

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
    uint32_t rx1Delay{1};        // in [seconds]

    // transmitPower theTransmitPower{transmitPower::max};

    // #################################################
    // ### Encoding and Decoding of LoRaWAN messages ###
    // #################################################

    static constexpr uint32_t maxLoRaPayloadLength{256};                                                                                                   // limited to length of databuffer in SX126x
    static constexpr uint32_t b0BlockLength{16};                                                                                                           // length of the so-called B0 block, in front of the LoRa payload, needed to calculate MIC
    static constexpr uint32_t macHeaderLength{1};                                                                                                          // length of MHDR in [bytes]
    static constexpr uint32_t deviceAddressLength{deviceAddress::length};                                                                                  // length of DevAddr in [bytes]
    static constexpr uint32_t frameControlLength{1};                                                                                                       // length of FCtrl in [bytes]
    static constexpr uint32_t frameCountLSHLength{2};                                                                                                      // frameCount Least Significant Halve - truncated to 2 LSbytes only
    static constexpr uint32_t micLength{messageIntegrityCode::length};                                                                                     // length of MIC in [bytes]

    uint8_t rawMessage[maxLoRaPayloadLength + b0BlockLength]{};                                                                                            // in this buffer, the message is contructed (Tx - Uplink) or decoded (Rx - Downlink)

    uint32_t macPayloadLength{0};                                                                                                                          //
    uint32_t framePortLength{1};                                                                                                                           // total length of FPort in [bytes] 1 if present, 0 if not present
    uint32_t frameOptionsLength{0};                                                                                                                        // this is not static, it can vary between 0 and 15
    uint32_t frameHeaderLength{0};                                                                                                                         // this is not static, it can vary between 7 and 22
    uint32_t framePayloadLength{0};                                                                                                                        // length of the application (or MAC layer) payload. excludes header, port and mic
    uint32_t loRaPayloadLength{0};                                                                                                                         // length of the data in the radio Tx/Rx buffer

    static constexpr uint32_t macHeaderOffset{b0BlockLength};                                                                                              // offset in the rawMessage where the header(s) starts
    static constexpr uint32_t deviceAddressOffset{b0BlockLength + macHeaderLength};                                                                        // offset in the rawMessage where the deviceAddress is found
    static constexpr uint32_t frameControlOffset{b0BlockLength + macHeaderLength + deviceAddressLength};                                                   // offset in the rawMessage where the frameControl is found
    static constexpr uint32_t frameCountOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength};                                // offset in the rawMessage where the frameCountLSH is found
    static constexpr uint32_t frameOptionsOffset{b0BlockLength + macHeaderLength + deviceAddressLength + frameControlLength + frameCountLSHLength};        // offset in the rawMessage where the frameOptions are found

    uint32_t framePortOffset{};                                                                                                                            //
    uint32_t framePayloadOffset{};                                                                                                                         //
    uint32_t micOffset{};                                                                                                                                  //

    // static constexpr uint32_t macInOutLength{256};
    byteBuffer2<64> macIn;         // buffer holding the received MAC requests and/or answers
    byteBuffer2<64> macOut;        // buffer holding the MAC requests and/or answers to be sent

    // ################################################################
    // ### Helper functions for constructing an uplink message - Tx ###
    // ################################################################

    void setOffsetsAndLengthsTx(uint32_t framePayloadLength, uint32_t frameOptionsLength = 0);                                                                      // calculate all offsets and lengths in rawMessage starting from the to be transmitted application payload length
    void insertPayload(const uint8_t data[], const uint32_t length);                                                                                                // copy application payload to correct position in the rawMessage buffer
    void encryptPayload(aesKey& theKey);                                                                                                                            // encrypt the payload in the rawMessage buffer
    void decryptPayload(aesKey& theKey);                                                                                                                            // decrypt the payload in the rawMessage buffer
    void insertHeaders(const uint8_t theFrameOptions[], const uint32_t theFrameOptionslength, const uint32_t theFramePayloadLength, framePort theFramePort);        // prepend the header to the rawMessage buffer
    void insertBlockB0(linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t micPayloadLength);                                 //
    void insertMic();                                                                                                                                               //

    // #############################################################
    // ### Helper functions for decoding a downlink message - Rx ###
    // #############################################################

    void setOffsetsAndLengthsRx(uint32_t loRaPayloadLength);                                                                                                                   // calculate all offsets and lengths in rawMessage by decoding the received LoRa payload
    uint16_t getReceivedFramecount() { return (static_cast<uint16_t>(rawMessage[frameCountOffset]) + (static_cast<uint16_t>(rawMessage[frameCountOffset + 1]) << 8)); }        //
    bool isValidMic();                                                                                                                                                         //
    bool isValidDevAddr(deviceAddress testAddress);                                                                                                                            //
    bool isValidDownlinkFrameCount(frameCount testFrameCount);                                                                                                                 //
    messageType decodeMessage();                                                                                                                                               //

    static uint32_t getRandomNumber();
    static void startTimer(uint32_t timeOut);        // timeOut in [ticks] from the 2048 Hz clock driving LPTIM1
    static void stopTimer();

    void prepareBlockAi(uint8_t* aBlock, linkDirection theDirection, deviceAddress& anAddress, frameCount& aFrameCounter, uint32_t blockIndex);
    static uint32_t getReceiveTimeout(spreadingFactor aSpreadingfactor);

    void processMacContents();        // process the contents of the macIn buffer, output goes to macOut buffer
};
