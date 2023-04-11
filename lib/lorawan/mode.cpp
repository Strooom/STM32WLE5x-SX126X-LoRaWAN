#include "mode.h"

const char* toString(mode aMode) {
    switch (aMode) {
        case mode::BW500:
            return "BW 500";
            break;

        case mode::BW250:
            return "BW 250";
            break;

        case mode::BW125:
            return "BW 125";
            break;

// TODO : complete all modes

        default:
		return "unknown";
            break;
    }


}
float getBandwidth(mode aMode) {
    switch (aMode) {
        case mode::BW500:
            return 500.0;
            break;

        case mode::BW250:
            return 250.0;
            break;

        case mode::BW125:
            return 125.0;
            break;

// TODO : complete all modes

        default:
		return 0.0;
            break;
    }
}
uint32_t getIntermediateFRequency(mode aMode) {

    switch (aMode) {
        case mode::BW500:
            return 0;
            break;

        case mode::BW250:
        case mode::BW125:
            return 250;
            break;

// TODO : complete all modes

        default:
		return 0;
            break;
    }


}