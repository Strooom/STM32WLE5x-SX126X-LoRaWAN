// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "bme680.h"
#include "main.h"
#include "logging.h"

extern I2C_HandleTypeDef hi2c2;
extern logging theLog;

bool bme680::testI2cAddress(uint8_t addressToTest) {
    return (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c2, addressToTest << 1, halTrials, halTimeout));
}

uint8_t bme680::readRegister(registers registerAddress) {
    uint8_t result[1]{0};
    HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, result, 1, halTimeout);
    //theLog.snprintf("Read I2C : register = [%02X], data = [%02X] \n", registerAddress, result[0]);
    return result[0];
}

void bme680::writeRegister(registers registerAddress, uint8_t value) {
    HAL_I2C_Mem_Write(&hi2c2, i2cAddress << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, &value, 1, halTimeout);
}

void bme680::readRegisters(uint16_t startAddress, uint16_t length, uint8_t* destination) {
    HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, startAddress, I2C_MEMADD_SIZE_8BIT, destination, length, halTimeout);
    // theLog.snprintf("Read I2C : register = [%02X], data[%u] = [", startAddress, length);
    // for (uint32_t index = 0; index < length; index++) {
    //     theLog.snprintf("%02X ", destination[index]);
    // }
    // theLog.snprintf("]\n");
}


