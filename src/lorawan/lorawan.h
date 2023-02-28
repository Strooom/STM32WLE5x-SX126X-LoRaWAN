// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include "bytebuffer.h"
#include "frameport.h"
#include "macstate.h"
#include "deviceaddress.h"
#include "loramessage.h"
#include "../general/aeskey.h"
#include "dutycycle.h"

class LoRaWAN {
  public:
    void initialize();
    radioState getState() const;

    bool isReadyToTransmit() const;
    uint32_t getMaxApplicationPayloadLength() const;
    void sendUplink(byteBuffer& applicationPayloadToSend, framePort aFramePort);
    //    void sendUplinkConfirmed(byteBuffer& applicationPayloadToSend);
    void getDownlinkMessage(byteBuffer& applicationPayloadReceived);

    void runMAC();        // taking care of all the MAC layer logic

    void encryptApplicationPayload();

    // TODO : we need a reference to NVS so we can store keys etc in EEPROM

  private:
    deviceAddress thisDeviceAddress{0U};
    aesKey key1;
    uint32_t uplinkFrameCount{0};
    uint32_t downlinkFrameCount{0};
    radioState theState{radioState::idle};
    loraMessage uplinkMessage;
    loraMessage downlinkMessage;
    dutyCycle theDutyCycle;
};
