// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "macheader.h"
#include "mic.h"

class loraMessage {        // aka PHYPayload
  public:
    static constexpr uint32_t maxPayloadLength{256};
    uint32_t length() const;         // returns the length of the message in bytes
    const uint8_t* asBytes();        // returns a pointer to the message as a byte array
    void setHeader();
    void setPayload(const uint8_t* payload, uint32_t length);
    void addMIC(const uint8_t* key);

    bool validateMIC(const uint8_t* key) const;        // validates the MIC over the buffer, using the key
    uint32_t getPayloadLength() const;                 // returns the length of the payload in bytes
    uint8_t* getPayload();                             // returns a pointer to the payload

  private:
    macHeader theMacHeader;
    uint8_t payload[maxPayloadLength]{0};
    messageIntegrityCode theMIC;
};
