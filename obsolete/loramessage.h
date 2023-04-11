// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "bytebuffer.h"
#include "aeskey.h"
#include "macheader.h"
#include "macpayload.h"
#include "mic.h"

// This is a class that represents the payload that will be
// * sent to the LoRa modem
// * received from the LoRa modem
// When transmitting :
// * set an application payload
// * encrypt the payload
// * add header
// * calculate and add MIC
// * send it to the Sx126x
//
// When receiving : load this object from the received raw data from the Sx126x, then parse it to run and validate all LoRaWAN logic

class loraMessage {        // aka PHYPayload
  public:
    void constructUplinkMessage(frameHeader aFrameHeader, framePort aFramePort, byteBuffer& applicationPayload, aesKey aKey);        //
    bool transmit();                                                                                                                 // send the Msg to the SX126x radio

    bool receive();                                                        // load message received by the SX126x radio
    void processDownlinkMessage(byteBuffer& theApplicationPayload);        //

  private:
    void setHeader();                                  //
    void addMIC(const uint8_t* key);                   //
    bool validateMIC(const uint8_t* key) const;        // validates the MIC over the buffer, using the key

    // LoRaWAN message consists of following 3 elements
    // macHeader theMacHeader;             //
    // macPayload theMacPayload;           //
    // messageIntegrityCode theMIC;        //

    uint8_t rawMessage[272];                                // 256 bytes as this is the maximum we can send to the S126x radio. 16 extra bytes in front, needed to calculate the MIC (they are not part of the message sent to the radio)
    static constexpr uint32_t headerOffset{16};             // offset in the rawMessage where the header starts
    static constexpr uint32_t payloadOffset{16 + 8};        // offset in the rawMessage where the payload starts
    uint32_t payloadLength{0};                              // length of the payload
};
