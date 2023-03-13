#include "mic.h"

uint8_t& messageIntegrityCode::operator[](uint32_t byteIndex) {
    return (mic + byteIndex);
}