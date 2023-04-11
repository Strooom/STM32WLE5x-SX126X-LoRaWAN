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
            return "lowPowerTimerExpired";
            break;

        case applicationEvent::downlinkMessageReceived:
            return "downlinkMessageReceived";
            break;

        default:
            return "unknown application event";
            break;
    }
}
