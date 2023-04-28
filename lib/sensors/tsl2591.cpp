// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include "tsl2591.h"
#include "logging.h"


// Definition of NON-CONST STATIC variables
tsl2591::integrationTimes tsl2591::integrationTime{integrationTimes::integrationTime100ms};
tsl2591::gains tsl2591::gain{gains::gain1x};

float tsl2591::luxCoefficient{408.0F};
float tsl2591::ch0Coefficient{1.64F};
float tsl2591::ch1Coefficient{0.59F};
float tsl2591::ch2Coefficient{0.86F};

bool tsl2591::isPresent() {
    // 1. Check if something is connected to the I2C bus at the address of the bme680
    if (!testI2cAddress(i2cAddress)) {
        return false;
    }
    // 2. Check if it is a TSL2591 by reading the chip id register/value
    uint8_t chipidValue = readRegister(tsl2591::registers::id);
    return (tsl2591::chipIdValue == chipidValue);
}

void tsl2591::initialize() {
    writeRegister(registers::enable, powerOn);
    setIntegrationTime(integrationTimes::integrationTime100ms);
    setGain(gains::gain428x);
}

bool tsl2591::goSleep() {
    writeRegister(registers::enable, powerOff);
    return false;
}

float tsl2591::readVisibleLight() {
    // 1. Enable the sensor
    writeRegister(registers::enable, 0x03);

    // 2. Wait for the ADC to have new data
    // TODO : HAL_Delay(700); The sensor needs 100 ms per integration time step... But we could also poll it for being ready

    // 3. Read the data // CHAN0 must be read before CHAN1 See: https://forums.adafruit.com/viewtopic.php?f=19&t=124176
    uint32_t raw1 = readRegister(registers::c0datah);
    uint32_t raw2 = readRegister(registers::c0datal);
    uint32_t raw3 = readRegister(registers::c1datah);
    uint32_t raw4 = readRegister(registers::c1datal);

    uint32_t rawch0 = (raw1 << 8) + raw2;
    uint32_t rawch1 = (raw3 << 8) + raw4;

    // logging::snprintf("S ch0 [%u], ch1 [%u]\n", rawch0, rawch1);

    if ((rawch0 == 0xFFFF) || (rawch1 == 0xFFFF)) {
        decreaseSensitivity();
    }

    if ((rawch0 < 0x3333) || (rawch1 < 0x3333)) {
        increaseSensitivity();
    }

    float integrationTimeFactor = static_cast<float>((static_cast<uint32_t>(integrationTime) + 1) * 100);
    float gainFactor;
    switch (gain) {
        case gains::gain1x:
        default:
            gainFactor = 1.0F;
            break;

        case gains::gain25x:
            gainFactor = 25.0F;
            break;

        case gains::gain428x:
            gainFactor = 428.0F;
            break;

        case gains::gain9876x:
            gainFactor = 9876.0F;
            break;
    }

    float totalFactor = (integrationTimeFactor * gainFactor) / luxCoefficient;
    float lux         = (((float)rawch0 - (float)rawch1)) * (1.0F - ((float)rawch1 / (float)rawch0)) / totalFactor;
    return lux;
}

void tsl2591::setIntegrationTime(integrationTimes theIntegrationTime) {
    integrationTime = theIntegrationTime;
    uint8_t control = (static_cast<uint8_t>(integrationTime)) | (static_cast<uint8_t>(gain));
    writeRegister(registers::config, control);
}

void tsl2591::setGain(gains theGain) {
    gain            = theGain;
    uint8_t control = (static_cast<uint8_t>(integrationTime)) | (static_cast<uint8_t>(gain));
    writeRegister(registers::config, control);
}

// If the RAW ADC readings are too low, we need to increase the gain or integrationtime, otherwise we would lose precision
// Notes : factors between gains are ~25
// With integration time factors is 6 (600 / 100)

void tsl2591::increaseSensitivity() {
    // When the ADC reading is below 20% (of maximum reading), we first increase integration time, then gain
}
void tsl2591::decreaseSensitivity() {
    // When the ADC reading overflow, we first reduce integration time, then gain
}

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
