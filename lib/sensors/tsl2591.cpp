// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "tsl2591.h"

bool tsl2591::isPresent() {
    // 1. Check if something is connected to the I2C bus at the address of the bme680
    if (!testI2cAddress(i2cAddress)) {
        return false;
    }        //
    // 2. Check if it is a TSL2591 by reading the chip id register/value
    uint8_t chipidValue = readRegister(tsl2591::registers::id);
    return (tsl2591::chipIdValue == chipidValue);
}

bool tsl2591::goSleep() {
    // write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWEROFF); TODO
    return false;
}

// write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN | TSL2591_ENABLE_NPIEN);
// setGain
// setIntergrationTime

static void calculateLight() {
    // cpl = (ATIME * AGAIN) / DF
    //  cpl = (atime * again) / TSL2591_LUX_DF;

    // Original lux calculation (for reference sake)
    // float lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
    // float lux2 = ( ( TSL2591_LUX_COEFC * (float)ch0 ) - ( TSL2591_LUX_COEFD *
    // (float)ch1 ) ) / cpl; lux = lux1 > lux2 ? lux1 : lux2;

    // Alternate lux calculation 1
    // See: https://github.com/adafruit/Adafruit_TSL2591_Library/issues/14
    //  lux = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

    // Alternate lux calculation 2
    // lux = ( (float)ch0 - ( 1.7F * (float)ch1 ) ) / cpl;

}        //

#ifndef environment_desktop

#include "main.h"

extern I2C_HandleTypeDef hi2c2;

bool tsl2591::testI2cAddress(uint8_t addressToTest) {
    return (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c2, addressToTest << 1, halTrials, halTimeout));
}

uint8_t tsl2591::readRegister(registers registerAddress) {
    uint16_t command = commandMask | static_cast<uint16_t>(registerAddress);
    uint8_t result[1]{0};
    HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, command, I2C_MEMADD_SIZE_8BIT, result, 1, halTimeout);
    return result[0];
}

void tsl2591::writeRegister(registers registerAddress, uint8_t value) {
    uint16_t command = commandMask | static_cast<uint16_t>(registerAddress);
    HAL_I2C_Mem_Write(&hi2c2, i2cAddress << 1, command, I2C_MEMADD_SIZE_8BIT, &value, 1, halTimeout);
}

#else

bool tsl2591::testI2cAddress(uint8_t addressToTest) {
    return false;
}

uint8_t tsl2591::readRegister(registers registerAddress) {
    uint8_t result[1]{0};
    return result[0];
}

void tsl2591::writeRegister(registers registerAddress, uint8_t value) {
}

#endif