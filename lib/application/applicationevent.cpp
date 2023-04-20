#include "applicationevent.h"

const char* toString(applicationEvent anEvent) {
    switch (anEvent) {
        case applicationEvent::usbConnected:
            return "usb Connected";
            break;

        case applicationEvent::usbRemoved:
            return "usb Removed";
            break;

        case applicationEvent::realTimeClockTick:
            return "realTimeClock Tick";
            break;

        case applicationEvent::lowPowerTimerExpired:
            return "lowPower Timer Expired";
            break;

        case applicationEvent::downlinkApplicationPayloadReceived:
            return "downlink Application Payload Received";
            break;

        case applicationEvent::downlinkMacCommandReceived:
            return "downlink MAC Command Received";
            break;

        default:
            return "unknown application event";
            break;
    }
}
