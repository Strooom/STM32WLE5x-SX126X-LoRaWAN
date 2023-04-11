// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "bme680.h"

void bme680::initialize() {
    // TODO
}

bool bme680::isPresent() {
    // 1. Check if something is connected to the I2C bus at the address of the bme680
    if (!testI2cAddress(i2cAddress)) {
        return false;
    }        //
    // 2. Check if it is a BME680 by reading the chip id register/value
    uint8_t chipidValue = readRegister(bme680::registers::chipId);
    return (bme680::chipIdValue == chipidValue);
}

// bool bme680::readRegisters(registers firstRegisterAddress, uint8_t nmbrBytesToRead, uint8_t* values) const {
// }

void bme680::calculateTemperature() {
}
void bme680::calculateBarometricPressure() {
}
void bme680::calculateRelativeHumidity() {
}

bool bme680::goSleep() {
    // nothing to do as the bme680 goes into sleep automatically after a conversion
}

void bme680::reset() {
    writeRegister(bme680::registers::reset, static_cast<uint8_t>(bme680::commands::softReset));
}
