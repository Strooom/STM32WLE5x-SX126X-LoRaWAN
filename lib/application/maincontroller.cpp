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

#include "main.h"

extern LPTIM_HandleTypeDef hlptim1;
extern RNG_HandleTypeDef hrng;

extern logging theLog;
extern eventBuffer<applicationEvent, 16U> applicationEventBuffer;
extern LoRaWAN loraNetwork;
extern sensorCollection theSensors;

void mainController::initialize() {
}

void mainController::handleEvents() {
    if (applicationEventBuffer.hasEvents()) {
        applicationEvent anEvent = applicationEventBuffer.pop();
        theLog.snprintf("applicationEvent : %s\n", toString(anEvent));
        switch (anEvent) {
            case applicationEvent::usbConnected:
                // use UART and configure IOs
                break;

            case applicationEvent::usbRemoved:
                // disable UART and disable IOs
                break;

            case applicationEvent::downlinkMessageReceived: {
                byteBuffer receivedData;
                loraNetwork.getDownlinkMessage(receivedData);
            } break;

            case applicationEvent::realTimeClockTick:
                theSensors.measure();
                // 1. get time from RTC
                // 2. check sensorCollection if any of them needs to take a measurement
                // 2.1 store all measurements in EEPROM
                // 3. check if we have data to send uplink
                // 3.1 unconfirmed msgs with more than 24 hours old data
                // 3.2 new data that was not yet sent
                break;

            default:
                // if (txTimer.expired()) {
                //     if (loraNetwork.isReadyToTransmit()) {
                //         byteBuffer myData;
                //         myData.set("testMessage");
                //         loraNetwork.sendUplink(myData, 20U);
                //     }
                // }
                // if (event == event::downlinkReceived) {
                // }
                break;
        }
    }
}


