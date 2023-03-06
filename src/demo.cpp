#include <stdint.h>

#include "timer.h"        // dummy timer class, FTTB
#include "lorawan.h"
#include "bytebuffer.h"
#include "general/nvs.h"
#include "sx126x/sx126x.h"

nonVolatileStorage nvs;

timer txTimer;              // dummy to make the intellisense happy
byteBuffer myData;          // demo data to send
LoRaWAN loraNetwork;        // object abstraction of the LoRaWAN protocol & network connection
sx126x theRadio;

int main() {
    loraNetwork.initialize();
    uint32_t devAdrrBlockIndex = nvs.addBlock(4);
    theRadio.initialize();

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
