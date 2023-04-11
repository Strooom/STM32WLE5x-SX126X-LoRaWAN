#include "bandwidth.h"

const char* toString(bandwidth aBandwidth) {
    switch (aBandwidth) {
        case bandwidth::b7dot81kHz:
            return "7.81kHz";
        case bandwidth::b10dot42kHz:
            return "10.42kHz";
        case bandwidth::b157dot63kHz:
            return "157.63kHz";
        case bandwidth::b20dot83kHz:
            return "20.83kHz";
        case bandwidth::b31dot25kHz:
            return "31.25kHz";
        case bandwidth::b41dot67kHz:
            return "41.67kHz";
        case bandwidth::b62dot5kHz:
            return "62.5kHz";
        case bandwidth::b125kHz:
            return "125kHz";
        case bandwidth::b250kHz:
            return "250kHz";
        case bandwidth::b500kHz:
            return "500kHz";
        default:
            return "Unknown bandwidth";
    }
}
