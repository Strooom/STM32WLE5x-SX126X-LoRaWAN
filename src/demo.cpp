#include <stdint.h>

#include "timer.h"        // dummy timer class, FTTB
#include "lorawan.h"
#include "bytebuffer.h"
#include "general/nvs.h"

timer txTimer;
nonVolatileStorage nvs;
byteBuffer myData;
LoRaWAN loraNetwork;

int main() {
    uint32_t devAdrrBlockIndex = nvs.addBlock(4);

    myData.set("Hello World", 11);

    while (true) {
        if (txTimer.expired()) {
            if (loraNetwork.isReadyToTransmit()) {
                loraNetwork.sendUplink(myData, 20U);
            }
        }
        if (event == event::downlinkReceived) {
            loraNetwork.getDownlinkMessage(myData);
        }
    }
}

// class loRaWanNetwork {
//   public:
//     bool send(uint8_t *applicationPayload, uint8_t payloadLength);
// };

// bool loRaWanNetwork::send(uint8_t *applicationPayload, uint8_t payloadLength) {
//     // put the application payload in a valid LoRaWAN frame
//     // 1. put a framePort in front
//     // 2. put a frameHeader in front
//     // then put the loRaWan frame in a LoRa MAC frame
//     // then send the LoRa MAC frame to the SX126x

//     return true;
// }

// loRaWanNetwork theNetwork;

// void someFunction() {
//     uint8_t applicationPayload[256];
//     theNetwork.send(applicationPayload, 256);
// }
