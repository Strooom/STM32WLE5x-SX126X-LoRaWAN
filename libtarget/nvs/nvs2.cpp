// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "nvs.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;

bool nonVolatileStorage::isReady() {
    if (HAL_OK != HAL_I2C_IsDeviceReady(&hi2c2, i2cAddress << 1, halTrials, halTimeout)) {        // testing presence of the first bank of 64K (U7)
        return false;
    }
    if (HAL_OK != HAL_I2C_IsDeviceReady(&hi2c2, (i2cAddress << 1) + 1, halTrials, halTimeout)) {        // testing presence of the second bank of 64K (U8)
        return false;
    }
    return true;
}

void nonVolatileStorage::read(uint32_t blockIndex, uint8_t* destinationDataBuffer) {
    if (blockIndexIsValid(blockIndex)) {
        uint32_t startAddress = theNvsMap.block[blockIndex].startAddress;
        uint32_t length       = theNvsMap.block[blockIndex].length;
        HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, startAddress, I2C_MEMADD_SIZE_8BIT, destinationDataBuffer, length, halTimeout);
    }
}

void nonVolatileStorage::write(uint32_t blockIndex, uint8_t* sourceDataBuffer) {
    if (blockIndexIsValid(blockIndex)) {
        HAL_GPIO_WritePin(GPIOB, writeProtect_Pin, GPIO_PIN_RESET);        // Drive writeProtect LOW = enable write
        uint32_t startAddress = theNvsMap.block[blockIndex].startAddress;
        uint32_t length       = theNvsMap.block[blockIndex].length;
        HAL_I2C_Mem_Write(&hi2c2, i2cAddress << 2, startAddress, I2C_MEMADD_SIZE_8BIT, sourceDataBuffer, length, halTimeout);
        HAL_GPIO_WritePin(GPIOB, writeProtect_Pin, GPIO_PIN_SET);        // disable write
    }
}
