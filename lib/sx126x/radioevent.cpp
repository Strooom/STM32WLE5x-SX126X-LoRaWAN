#include "radioevent.h"

const char* toString(radioEvent anEvent) {
    switch (anEvent) {
        case radioEvent::transmitComplete:
            return "transmitComplete";
        case radioEvent::transmitTimeout:
            return "transmitTimeout";
        case radioEvent::receiveComplete:
            return "receiveComplete";
        case radioEvent::receiveTimeout:
            return "receiveTimeout";
        default:
            return "unknown";
    }
}