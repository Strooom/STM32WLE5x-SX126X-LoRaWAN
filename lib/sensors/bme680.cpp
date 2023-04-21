// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "bme680.h"

float bme680::calibCoefT1;
float bme680::calibrationCoefficientTemperature2;
float bme680::calibrationCoefficientTemperature3;

void bme680::initialize() {
    // read the calibration data from the sensor
    uint8_t registerData[64]{};
    readRegisters(0x8A, 23, registerData);
    readRegisters(0xE1, 14, registerData + 23);
    readRegisters(0x00, 5, registerData + 37);

    calibCoefT1                        = static_cast<float>((static_cast<uint32_t>(registerData[32]) << 8) + static_cast<uint32_t>(registerData[31]));
    calibrationCoefficientTemperature2 = static_cast<float>((static_cast<uint32_t>(registerData[1]) << 8) + static_cast<uint32_t>(registerData[0]));
    calibrationCoefficientTemperature3 = static_cast<float>(registerData[2]);
}

float bme680::readTemperature() {
    writeRegister(bme680::registers::ctrl_hum, 0);                  // select only temperature measurement, no oversampling
    writeRegister(bme680::registers::ctrl_meas, 0b00100001);        // select only temperature measurement, no oversampling
    bool hasNewData{false};
    while (!hasNewData) {
        hasNewData = readRegister(bme680::registers::meas_status) & 0x80;        // wait until conversion is done
    }

    constexpr uint32_t nmbrRegisters{3};
    uint8_t registerData[nmbrRegisters];
    readRegisters(static_cast<uint16_t>(bme680::registers::temp_msb), nmbrRegisters, registerData);
    uint32_t rawDataTemperature = ((static_cast<uint32_t>(registerData[0]) << 12) |
                                   (static_cast<uint32_t>(registerData[1]) << 4) |
                                   (static_cast<uint32_t>(registerData[2]) >> 4));

    return calculateTemperature(rawDataTemperature);
}

float bme680::readHumidity() {
    writeRegister(bme680::registers::ctrl_hum, 0);                  // select only humidity, no oversampling
    writeRegister(bme680::registers::ctrl_meas, 0b00100001);        // select only humidity measurement, no oversampling
    bool hasNewData{false};
    while (!hasNewData) {
        hasNewData = readRegister(bme680::registers::meas_status) & 0x80;        // wait until conversion is done
    }

    constexpr uint32_t nmbrRegisters{2};
    uint8_t registerData[nmbrRegisters];
    readRegisters(static_cast<uint16_t>(bme680::registers::hum_msb), nmbrRegisters, registerData);
    uint32_t rawDataHumidity = ((static_cast<uint32_t>(registerData[0]) << 8) | (static_cast<uint32_t>(registerData[1])));

    //    return calculateRelativeHumidity(rawDataHumidity);

    return 1.234F;
}

float bme680::readBarometricPressure() {
    writeRegister(bme680::registers::ctrl_hum, 0);                  // select only temperature measurement, no oversampling
    writeRegister(bme680::registers::ctrl_meas, 0b00100001);        // select only temperature measurement, no oversampling
    bool hasNewData{false};
    while (!hasNewData) {
        hasNewData = readRegister(bme680::registers::meas_status) & 0x80;        // wait until conversion is done
    }

    constexpr uint32_t nmbrRegisters{3};
    uint8_t registerData[nmbrRegisters];
    readRegisters(static_cast<uint16_t>(bme680::registers::temp_msb), nmbrRegisters, registerData);
    uint32_t rawDataTemperature = ((static_cast<uint32_t>(registerData[0]) << 12) |
                                   (static_cast<uint32_t>(registerData[1]) << 4) |
                                   (static_cast<uint32_t>(registerData[2]) >> 4));

    return calculateTemperature(rawDataTemperature);
}

float bme680::calculateTemperature(uint32_t rawData) {
    // TODO : this can be made more efficient by precalculating the terms where actual data has no influence
    // also take one conversion to float from rawData io several..
    /* calculate var1 data */
    float var1 = ((((float)rawData / 16384.0f) - (calibCoefT1 / 1024.0f)) * (calibrationCoefficientTemperature2));

    /* calculate var2 data */
    float var2 = (((((float)rawData / 131072.0f) - (calibCoefT1 / 8192.0f)) * (((float)rawData / 131072.0f) - (calibCoefT1 / 8192.0f))) * (calibrationCoefficientTemperature3 * 16.0f));

    /* compensated temperature data*/
    return ((var1 + var2) / 5120.0f);
}
void bme680::calculateBarometricPressure() {
}
void bme680::calculateRelativeHumidity() {
}

void bme680::goSleep() {
}

void bme680::reset() {
    writeRegister(bme680::registers::reset, static_cast<uint8_t>(bme680::commands::softReset));
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

#ifndef environment_desktop

#include "main.h"
// #define showI2cCommunication

extern I2C_HandleTypeDef hi2c2;

bool bme680::testI2cAddress(uint8_t addressToTest) {
    return (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c2, addressToTest << 1, halTrials, halTimeout));
}

uint8_t bme680::readRegister(registers registerAddress) {
    uint8_t result[1]{0};
    HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, result, 1, halTimeout);
#ifdef showI2cCommunication
    logging::snprintf("Read I2C : register = [%02X], data = [%02X] \n", registerAddress, result[0]);
#endif
    return result[0];
}

void bme680::writeRegister(registers registerAddress, uint8_t value) {
    HAL_I2C_Mem_Write(&hi2c2, i2cAddress << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, &value, 1, halTimeout);
}

void bme680::readRegisters(uint16_t startAddress, uint16_t length, uint8_t* destination) {
    HAL_I2C_Mem_Read(&hi2c2, i2cAddress << 1, startAddress, I2C_MEMADD_SIZE_8BIT, destination, length, halTimeout);
#ifdef showI2cCommunication
    logging::snprintf("Read I2C : register = [%02X], data[%u] = [", startAddress, length);
    for (uint32_t index = 0; index < length; index++) {
        logging::snprintf("%02X ", destination[index]);
    }
    logging::snprintf("]\n");
#endif
}

#else

// TODO : a desktop console version goes here

#endif