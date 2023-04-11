// #############################################################################
// ### This file is part of the source code for the MuMo project             ###
// ### https://github.com/Strooom/MuMo-v2-Node-SW                            ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <stdint.h>
#include "logging.h"                 // definition of the logging class
#include "power.h"                   //
#include "eventbuffer.h"             // application uses 3 eventbuffers, and the eventbuffer class is defined here
#include "applicationevent.h"        // definition of all events that can happen in the application
#include "maincontroller.h"          // definition of the class that handles the top-level control of the application - a finite state machine
#include "lorawanevent.h"            // definition of all events that can happen in the LoRaWAN protocol
#include "lorawan.h"                 // definition of the class that implements the lorawan protocol layer
#include "radioevent.h"              // definition of all events that can happen in the radio layer / SC126x
#include "nvs.h"                     // definition of the class that implements the non-volatile storage
//#include "sx126x.h"                  // definition of the class that implements the radio / sx126x driver
#include "cli.h"                     // definition of the class that implements the command line interface
#include <iostream>
//power thePower;

//cli theCli;

//eventBuffer<radioEvent, static_cast<size_t>(16)> sx126xEventBuffer;
//eventBuffer<loRaWanEvent, static_cast<size_t>(16)> loraWanEventBuffer;
//eventBuffer<applicationEvent, static_cast<size_t>(16)> applicationEventBuffer;

//nonVolatileStorage nvs;        // instance of the non-volatile storage class
//mainController mainCtrl;
LoRaWAN loraNetwork;        // object abstraction of the LoRaWAN protocol & network connection
//sx126x theRadio;            // instance of the radio / sx126x driver

int main() {
    loraNetwork.initialize();
    byteBuffer applicationPayload;
    applicationPayload.setFromASCII("101112131415161718191A1B1C1D1E1F");
    loraNetwork.sendUplink(applicationPayload, 0x09);

std::cout << "Ready!" << std::endl;


    //theRadio.initialize();
    // TODO : a lot more initialization code is needed here



    // while (true) {
    //     thePower.detectUsbConnectOrRemove();

    //     while (sx126xEventBuffer.isEmpty()) {
    //         theRadio.handleEvents();
    //     }

    //     while (loraWanEventBuffer.isEmpty()) {
    //         loraNetwork.handleEvents();
    //     }

    //     while (!applicationEventBuffer.isEmpty()) {
    //         mainCtrl.handleEvents();
    //     }

    //     if (thePower.isUsbConnected()) {
    //         theCli.handleEvents();
    //     } else {
    //         // theMcu.goSleep(), go sleep, real-time clock will wake us up
    //     }
    // }
}
