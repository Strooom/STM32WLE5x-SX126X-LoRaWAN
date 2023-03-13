// #############################################################################
// ### This file is part of the source code for the Moovr CNC Controller     ###
// ### https://github.com/Strooom/Moovr                                      ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include "logging.h"
#include "maincontroller.h"
#include "eventbuffer.h"
#include "applicationevent.h"
#include "lorawan.h"

extern logging uLog;
extern eventBuffer<applicationEvent, static_cast<size_t>(16)> applicationEventBuffer;
extern LoRaWAN loraNetwork;

mainController::mainController() {
}

void mainController::handleEvents() {
    if (applicationEventBuffer.hasEvents()) {
        applicationEvent anEvent = applicationEventBuffer.popEvent();
        uLog.snprintf("event : %s\n", toString(anEvent));
        switch (anEvent) {
            case applicationEvent::usbConnected:
                // use UART and configure IOs
                break;
            case applicationEvent::usbRemoved:
                // disable UART and disable IOs
                break;

            case applicationEvent::downlinkMessageReceived:
                break;

            default:
                if (txTimer.expired()) {
                    if (loraNetwork.isReadyToTransmit()) {
                        byteBuffer myData;
                        myData.set("testMessage");
                        loraNetwork.sendUplink(myData, 20U);
                    }
                }
                if (event == event::downlinkReceived) {
                    loraNetwork.getDownlinkMessage(myData);
                }
                break;
        }
    }
}

uint32_t mainController::getRootSamplingPeriod() {
    return rootSamplingPeriod;
}
