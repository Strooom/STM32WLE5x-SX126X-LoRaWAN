#include "framecontrol.h"

frameControl::frameControl(linkDirection newLinkDirection) {
    theLinkDirection = newLinkDirection;
    if (theLinkDirection == linkDirection::uplink) {
        ADR       = false;
        ADRACKReq = false;
        ACK       = false;
        ClassB    = false;
        FPending  = false;
        FOptsLen  = 0;
    } else {
        //     when we transmit an uplink, we need to set these bits, when we receive a downlink, we need to read them from the received data
    }
}
