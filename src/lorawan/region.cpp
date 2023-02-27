#include "region.h"

const char* toString(region aRegion) {
    switch (aRegion) {
        case region::EU868:
            return "EU-868";
            break;
        case region::US915:
            return "US-915";
            break;
        default:
            return "Unknown region";
            break;
    }
}