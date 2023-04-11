// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

#include "aeskey.h"
#include "bytebuffer.h"
#include "deviceaddress.h"
#include "dutycycle.h"
#include "framecount.h"
#include "frameport.h"
#include "linkdirection.h"
#include "macheader.h"
#include "mic.h"
#include "txrxcycle.h"


class LoRaWAN {
  public:
    LoRaWAN();
    void initialize();

    void handleEvents();
    bool isReadyToTransmit() const;
    uint32_t getMaxApplicationPayloadLength() const;

    void sendUplink(byteBuffer& applicationPayloadToSend, framePort aFramePort);
    //    void sendUplinkConfirmed(byteBuffer& applicationPayloadToSend); // wait for succes or fail from LoRA LAYER
    void getDownlinkMessage(byteBuffer& applicationPayloadReceived);

    void runMAC();        // taking care of all the MAC layer logic

    // TODO : we need a reference to NVS so we can store keys etc in EEPROM

  private:
    txRxCycleState theTxRxCycleState{txRxCycleState::idle};        // state variable tacking the TxRxCycle state machine
    void goTo(txRxCycleState newState);                            // move the state of the TxRxCycle state machine
    void exitState(txRxCycleState currentState);                   // actions when leaving a state
    void enterState(txRxCycleState newState);                      // actions when entering a state

    macHeader MHDR;               // macHeader used for constructing and parsing LoRaWAN messages
    deviceAddress DevAddr;        // device address
    aesKey applicationKey;
    aesKey networkKey;
    frameCount uplinkFrameCount;
    frameCount downlinkFrameCount;

    dutyCycle theDutyCycle;
    messageIntegrityCode mic;

    uint8_t rawMessage[272];          // 256 bytes as this is the maximum we can send to the S126x radio. 16 extra bytes in front, needed to calculate the MIC (they are not part of the message sent to the radio)
    uint32_t payloadLength{0};        // length of the application payload

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

    void copyPayload(byteBuffer& applicationPayloadToSend);                                    // copy application payload to correct position in the rawMessage buffer
    void encryptPayload(aesKey& theKey);                                                       // encrypt the payload in the rawMessage buffer
    void prependHeader(framePort theFramePort);                                                // prepend the header to the rawMessage buffer
    void prepareBlockB0(uint32_t applicationPayloadLength, linkDirection theDirection);        // prepare the so-called B0 block, prepended to payload to calculate MIC
    void calculateAndAppendMic();                                                              // calculate the MIC
    void prepareBlockAi(uint8_t* aBlock, uint32_t blockIndex, linkDirection theDirection);


    uint32_t getRandomNumber();
    void startTimer(uint32_t timeOut);
    void stopTimer();
};
