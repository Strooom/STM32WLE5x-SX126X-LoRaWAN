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
#include "radiostate.h"
#include "deviceaddress.h"
#include "loramessage.h"
#include "aeskey.h"
#include "dutycycle.h"

class LoRaWAN {
  public:
    void initialize();
    radioState getState() const;

    bool isReadyToTransmit() const;
    uint32_t getMaxApplicationPayloadLength() const;
    void sendUplink(byteBuffer& applicationPayloadToSend, framePort aFramePort);
    //    void sendUplinkConfirmed(byteBuffer& applicationPayloadToSend); // wait for succes or fail from LoRA LAYER
    void getDownlinkMessage(byteBuffer& applicationPayloadReceived);

    void runMAC();        // taking care of all the MAC layer logic

    void encryptApplicationPayload();

    // TODO : we need a reference to NVS so we can store keys etc in EEPROM

  private:
    deviceAddress devAddr;
    aesKey key1;
    frameCount uplinkFrameCount;
    frameCount downlinkFrameCount;
    radioState theState{radioState::idle};
    loraMessage uplinkMessage;
    loraMessage downlinkMessage;
    dutyCycle theDutyCycle;
};
