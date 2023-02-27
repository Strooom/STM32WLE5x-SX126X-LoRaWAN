#include "codingrate.h"

const char* toString(codingRate aCodingRate) {
    switch (aCodingRate) {
        case codingRate::cr4_4:
            return "CR4/4";
        case codingRate::cr4_5:
            return "CR4/5";
        case codingRate::cr4_6:
            return "CR4/6";
        case codingRate::cr4_7:
            return "CR4/7";
        case codingRate::cr4_8:
            return "CR4/8";
        default:
            return "Unknown coding rate";
    }
}