// #############################################################################
// ### This file is part of the source code for the Moovr CNC Controller     ###
// ### https://github.com/Strooom/Moovr                                      ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "maincontroller.h"
#include "applicationevent.h"
#include "eventbuffer.h"
#include "logging.h"
#include "lorawan.h"
#include "sensorcollection.h"
#include "measurementcollection.h"
#include "main.h"

extern LPTIM_HandleTypeDef hlptim1;
extern RNG_HandleTypeDef hrng;

// static void MX_USART2_UART_Init(void);
// static void MX_USART2_UART_DeInit(void);

extern logging theLog;
extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;
extern LoRaWAN loraNetwork;
extern sensorCollection theSensors;
extern measurementCollection theMeasurements;

void mainController::initialize() {
    theLog.snprintf("MuMo v2 - build %s %s\n", __DATE__, __TIME__);
    theLog.snprintf("Initializing...\n");
    theSensors.discover();
    loraNetwork.initialize();        // LoRaWAN layer + the LoRa radio
    theLog.snprintf("... Ready\n");
}

void mainController::handleEvents() {
    if (applicationEventBuffer.hasEvents()) {
        applicationEvent theEvent = applicationEventBuffer.pop();
        theLog.snprintf("Application Event [%u] : %s\n", static_cast<uint8_t>(theEvent), toString(theEvent));
        switch (theEvent) {
            case applicationEvent::usbConnected:
                // MX_USART2_UART_Init();
                break;

            case applicationEvent::usbRemoved:
                // MX_USART2_UART_DeInit();
                break;

            case applicationEvent::downlinkApplicationPayloadReceived: {
                byteBuffer receivedData;
                loraNetwork.getDownlinkMessage(receivedData);
            } break;

            case applicationEvent::realTimeClockTick: {
                // 1. run all measurements
                theSensors.measure();

                // 2. check if we have enough unsent data to send uplink
                uint32_t maxUplinkPayloadNow        = loraNetwork.getMaxApplicationPayloadLength();
                uint32_t measurementToBeTransmitted = theMeasurements.getNmbrToBeTransmitted();
                if (((measurementToBeTransmitted + 1) * measurement::length) > maxUplinkPayloadNow) {
                    theLog.snprintf("[%u] meaurement bytes to transmit, [%u] bytes payload available\n", (measurementToBeTransmitted + 1) * measurement::length, maxUplinkPayloadNow);
                    if (loraNetwork.isReadyToTransmit()) {
                        theLog.snprintf("LoRaWAN layer ready to transmit\n");
                        byteBuffer thePayload;                                                 //
                        thePayload.setFromHexAscii("000102030405060708090A0B0C0D0E0F");        // TODO - TEST msg
                        loraNetwork.sendUplink(thePayload, 0x10);
                    }
                }
            } break;

            default:
                break;
        }
    }
}
