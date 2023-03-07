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

extern logging uLog;
extern eventBuffer<applicationEvent, static_cast<size_t>(16)> applicationEventBuffer;


mainController::mainController() {
}

void mainController::handleEvents() {
    if (applicationEventBuffer.hasEvents()) {
        event anEvent = theEventBuffer.popEvent();
        uLog.snprintf("event : %s\n", toString(anEvent));
        switch (anEvent) {
        case event::usbConnected:
        	// use UART and configure IOs
        	break;
        case event::usbRemoved:
        	// disable UART and disable IOs
        	break;
        }
    }
}

uint32_t mainController::getRootSamplingPeriod() {
    return rootSamplingPeriod;
}

