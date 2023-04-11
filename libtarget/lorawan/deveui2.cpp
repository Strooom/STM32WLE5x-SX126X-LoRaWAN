#include "deveui.h"

//#include "stm32wle5.h"
//extern peripheralRegister UID64_0;
//extern peripheralRegister UID64_1;
//peripheralRegister RCC_CR(reinterpret_cast<volatile uint32_t *const>(RCC_BASE + 0x00));

uint64_t devEUI::value() const {
    uint64_t result{0};
    result = 0;
    //(static_cast<uint64_t>(UID64_1.read()) << 32) | static_cast<uint64_t>(UID64_0.read()));
    return result;
}

// TODO : check which is the MSB / LSB register
