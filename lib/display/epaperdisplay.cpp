// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "stm32wle5.h"
#include "epaperdisplay.h"

extern peripheralRegister PORTB_IDR;
extern peripheralRegister PORTC_IDR;
extern peripheralRegister PORTB_BSRR;
extern peripheralRegister PORTC_BSRR;

//peripheralRegister PORTA_IDR(reinterpret_cast<volatile uint32_t *const>(GPIOA_BASE + 0x10));

bool ePaperDisplay::write() {
    if (!isBusy()) {
        setDataOrCommand(true);
        setChipSelect(true);

        // Write data to SPI

        setChipSelect(false);
    } else {
        // TODO : wait or error ??
    }
}

bool ePaperDisplay::isBusy() {
    return (PORTB_IDR.readBit(4));
}

void ePaperDisplay::setDataOrCommand(bool isData) {        // data = HIGH, command = LOW
    if (isData) {
        PORTB_BSRR.write(1 << 14);
    } else {
        PORTB_BSRR.write(1 << (14 + 16));
    }
}

void ePaperDisplay::setChipSelect(bool active) {        // active = LOW
    if (active) {
        PORTB_BSRR.write(1 << (5 + 16));
    } else {
        PORTB_BSRR.write(1 << 5);
    }
}
