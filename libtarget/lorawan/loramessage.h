// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "../general/bytebuffer.h"
#include "../general/aeskey.h"
#include "macheader.h"
#include "macpayload.h"
#include "mic.h"

// This is a class that represents the payload that will be
// * sent to the LoRa modem
// * received from the LoRa modem
// When transmitting : prepare this object, then send it to the Sx126x
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
    macHeader theMacHeader;
    macPayload theMacPayload;
    messageIntegrityCode theMIC;
};
