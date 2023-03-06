#include "headertype.h"

const char* toString(headerType theHeaderType) {
    switch (theHeaderType) {
        case headerType::explicitHeader:
            return "Explicit Header";
            break;
        case headerType::implicitHeader:
            return "Implicit Header";
            break;
        default:
            return "unknown header type";
            break;
    }
}