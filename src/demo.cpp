#include <stdint.h>

#include "timer.h"        // dummy timer class, FTTB
#include "lorawan.h"
#include "bytebuffer.h"
#include "general/nvs.h"

nonVolatileStorage nvs;

timer txTimer;            // dummy to make the intellisense happy
byteBuffer myData;        // demo data to send

// allocating the necessary blocks in the NVS - NOTE : do not change the order of these blocks if you want future firmware to find this data in the correct location. Only add new blocks to the end of the list. Blocks no longer needed can be ignored by doing (void) nvs.AddBlock(x);


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
