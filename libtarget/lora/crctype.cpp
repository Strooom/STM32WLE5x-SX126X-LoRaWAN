#include "crctype.h"

const char* toString(crcType theCrcType) {
    switch (theCrcType) {
        case crcType::off:
            return "CRC Off";
            break;
        case crcType::on:
            return "CRC On";
            break;
        default:
            return "unknown CRC type";
            break;
    }
}