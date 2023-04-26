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
// #include "main.h"

// extern LPTIM_HandleTypeDef hlptim1;
// extern RNG_HandleTypeDef hrng;
// extern RTC_HandleTypeDef hrtc;

// static void MX_USART2_UART_Init(void);
// static void MX_USART2_UART_DeInit(void);

extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;
extern LoRaWAN loraNetwork;
extern sensorCollection theSensors;
extern measurementCollection theMeasurements;
extern nonVolatileStorage nvs;

void mainController::initialize() {
    // logging::snprintf("Initializing mainController\n");

    if (nvs.isReady()) {
        // logging::snprintf("128K EEPROM found\n");
    }
    if (!nvs.isInitialized()) {
        nvs.initializeOnce();
    }

    theSensors.discover();
    loraNetwork.initialize();        // LoRaWAN layer + the LoRa radio

    // logging::snprintf("mainController initialized\n");
}

void mainController::handleEvents() {
    while (applicationEventBuffer.hasEvents()) {
        applicationEvent theEvent = applicationEventBuffer.pop();
        logging::snprintf("Application Event [%u] : %s\n",
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
                // 1. run all measurements
                sensorCollection::runResult theResult = theSensors.run();
                if (theResult == sensorCollection::runResult::newMeasurements) {
                    // 2. Send them as payload in an UpLink message
                    uint8_t tempData[256]{};
                    uint32_t tempDataIndex{0};
                    for (uint32_t measurementIndex = 0; measurementIndex < theSensors.actualNumberOfMeasurements; measurementIndex++) {
                        tempData[tempDataIndex] = static_cast<uint8_t>(theSensors.latestMeasurements[measurementIndex].type);

                        tempData[tempDataIndex + 1] = theSensors.latestMeasurements[measurementIndex].timestampAsBytes[0];
                        tempData[tempDataIndex + 2] = theSensors.latestMeasurements[measurementIndex].timestampAsBytes[1];
                        tempData[tempDataIndex + 3] = theSensors.latestMeasurements[measurementIndex].timestampAsBytes[2];
                        tempData[tempDataIndex + 4] = theSensors.latestMeasurements[measurementIndex].timestampAsBytes[3];

                        tempData[tempDataIndex + 5] = theSensors.latestMeasurements[measurementIndex].valueAsBytes[0];
                        tempData[tempDataIndex + 6] = theSensors.latestMeasurements[measurementIndex].valueAsBytes[1];
                        tempData[tempDataIndex + 7] = theSensors.latestMeasurements[measurementIndex].valueAsBytes[2];
                        tempData[tempDataIndex + 8] = theSensors.latestMeasurements[measurementIndex].valueAsBytes[3];

                        tempDataIndex += 9;
                    }

                    byteBuffer thePayload;
                    thePayload.set(tempData, tempDataIndex);
                    loraNetwork.sendUplink(thePayload, 0x10);
                }

                // 2. check if we have enough unsent data to send uplink
                //                uint32_t maxUplinkPayloadNow =                     loraNetwork.getMaxApplicationPayloadLength();
                //                uint32_t measurementToBeTransmitted =                     theMeasurements.getNmbrToBeTransmitted();
                //                if (((measurementToBeTransmitted + 1) * measurement::length) > maxUplinkPayloadNow) {
                //                    logging::snprintf(
                //                        "[%u] measurement bytes to transmit, [%u] bytes payload capacity\n",
                //                        (measurementToBeTransmitted + 1) * measurement::length,
                //                        maxUplinkPayloadNow);
                //                    if (loraNetwork.isReadyToTransmit()) {
                //                        logging::snprintf("LoRaWAN layer ready to transmit\n");
                //                        byteBuffer thePayload;        //
                //                        thePayload.setFromHexAscii(
                //                            "000102030405060708090A0B0C0D0E0F");        // TODO - TEST msg
                //                        loraNetwork.sendUplink(thePayload, 0x10);
                //                    }
                //                } else {
                //                    logging::snprintf("Not enough data to transmit\n");
                //                }
            } break;

            default:
                break;
        }
    }
}
