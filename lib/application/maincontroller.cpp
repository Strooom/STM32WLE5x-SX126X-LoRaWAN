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
#include "nvs.h"
#include "main.h"

extern LPTIM_HandleTypeDef hlptim1;
extern RNG_HandleTypeDef hrng;
extern RTC_HandleTypeDef hrtc;

// static void MX_USART2_UART_Init(void);
// static void MX_USART2_UART_DeInit(void);

extern logging theLog;
extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;
extern LoRaWAN loraNetwork;
extern sensorCollection theSensors;
extern measurementCollection theMeasurements;
extern nonVolatileStorage nvs;

void mainController::initialize() {
    theLog.snprintf("MuMo v2 - build %s %s test \n", __DATE__, __TIME__);

    theLog.snprintf("Initializing...\n");

    if (nvs.isReady()) {
        theLog.snprintf("128K EEPROM found\n");
    }
    if (!nvs.isInitialized()) {
        nvs.initializeOnce();
    }

    theSensors.discover();
    loraNetwork.initialize();        // LoRaWAN layer + the LoRa radio
    theLog.snprintf("... Ready\n");
}

void mainController::handleEvents() {
    if (applicationEventBuffer.hasEvents()) {
        applicationEvent theEvent = applicationEventBuffer.pop();
        theLog.snprintf("Application Event [%u] : %s\n",
                        static_cast<uint8_t>(theEvent), toString(theEvent));
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
                // 0. Print Time to check clock is running
                {
                    RTC_TimeTypeDef currTime = {0};
                    RTC_DateTypeDef currDate = {0};

                    HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
                    HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);
                    theLog.snprintf("Time = %02u:%02u:%02u\n", currTime.Hours, currTime.Minutes, currTime.Seconds);
                }

                // 1. run all measurements
                theSensors.measure();

                // 2. check if we have enough unsent data to send uplink
                uint32_t maxUplinkPayloadNow =
                    loraNetwork.getMaxApplicationPayloadLength();
                uint32_t measurementToBeTransmitted =
                    theMeasurements.getNmbrToBeTransmitted();
                if (((measurementToBeTransmitted + 1) * measurement::length) > maxUplinkPayloadNow) {
                    theLog.snprintf(
                        "[%u] measurement bytes to transmit, [%u] bytes payload capacity\n",
                        (measurementToBeTransmitted + 1) * measurement::length,
                        maxUplinkPayloadNow);
                    if (loraNetwork.isReadyToTransmit()) {
                        theLog.snprintf("LoRaWAN layer ready to transmit\n");
                        byteBuffer thePayload;        //
                        thePayload.setFromHexAscii(
                            "000102030405060708090A0B0C0D0E0F");        // TODO - TEST msg
                        loraNetwork.sendUplink(thePayload, 0x10);
                    }
                } else {
                    theLog.snprintf("Not enough data to transmit\n");
                }
            } break;

            default:
                break;
        }
    }
}
