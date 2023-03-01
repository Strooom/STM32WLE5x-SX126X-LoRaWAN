#include "invertiq.h"

const char* toString(invertIq theInvertIq) {
    switch (theInvertIq) {
        case invertIq::normal:
            return "Normal";
            break;
        case invertIq::inverted:
            return "Inverted";
            break;
        default:
            return "unknown invert IQ";
            break;
    }
}