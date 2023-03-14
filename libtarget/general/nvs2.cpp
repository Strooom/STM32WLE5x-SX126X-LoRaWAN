// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"

bool nonVolatileStorage::isReady() {
    return (HAL_OK == HAL_I2C_IsDeviceReady(I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout));
}

uint8_t nonVolatileStorage::read8(uint32_t blockIndex) {
    if (isValidBlockIndex(blockIndex)) {
        uint16_t MemAddress = static_cast<uint16_t>(theNvsMap.block[blockIndex].startAddress);
        uint16_t Size       = static_cast<uint16_t>(theNvsMap.block[blockIndex].length);
        uint8_t result;
        HAL_I2C_Mem_Read(I2C_HandleTypeDef * hi2c, 0xA0, MemAddress, 2U, &result, Size, 50U);
    } else {
        // TODO : log this error
        return 0;
    }
}


void nonVolatileStorage::write(uint32_t blockIndex, uint8_t sourceData) {
    if (isValidBlockIndex(blockIndex)) {
        uint16_t MemAddress = static_cast<uint16_t>(theNvsMap.block[blockIndex].startAddress);
        uint16_t Size       = static_cast<uint16_t>(theNvsMap.block[blockIndex].length);

        HAL_I2C_Mem_Write(I2C_HandleTypeDef * hi2c, 0xA0, MemAddress, 2U, &sourceData, Size, 50U);
    } else {
        // TODO : log this error
    }
}