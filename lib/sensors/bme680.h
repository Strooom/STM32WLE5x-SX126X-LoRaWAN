// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>

#include "sensor.h"

// Represents a Bosch BME680 sensor

class bme680 {
  public:
    static bool isPresent();                      // detect if there is an BME680 on the I2C bus
    static void reset();                          // soft-reset
    static void initialize();                     //
    static float readTemperature();               //
    static float readHumidity();                  //
    static float readBarometricPressure();        //
    static void goSleep();                        //

    static constexpr uint8_t i2cAddress{0x76};        // default I2C address for this sensor, DSO tied to GND on our hardware
    static constexpr uint8_t halTrials{0x03};         // ST HAL requires a 'retry' parameters
    static constexpr uint8_t halTimeout{0x10};        // ST HAL requires a 'timeout' in ms

    // Registers
    enum class registers : uint8_t {
        chipId    = 0xD0,        // address of register holding chipId
        reset     = 0xE0,        // address of register to reset the device : Write 0xB6
        ctrl_hum  = 0x72,        // address of register to set humidity oversampling
        ctrl_meas = 0x74,        // address of register to set temperature & pressure versampling
        config    = 0x75,        // address of register to set IIR filter & power mode

        temp_xlsb = 0x8F,        // address of register to read temperature LSB
        temp_lsb  = 0x8E,        // address of register to read temperature LSB
        temp_msb  = 0x8D,        // address of register to read temperature MSB

        hum_lsb = 0x89,        // address of register to read humidity LSB
        hum_msb = 0x88,        // address of register to read humidity MSB

        press_xlsb = 0x8C,        // address of register to read pressure LSB
        press_lsb  = 0x8B,        // address of register to read pressure LSB
        press_msb  = 0x8A,        // address of register to read pressure MSB

        meas_status = 0x1D,        // address of register to read measurement status

        status      = 0x73,        // address of register to read status
        meas_index  = 0x1E,        // address of register to read measurement index
        meas_period = 0x68,        // address of register to read measurement period
        gas_wait0   = 0x64,        // address of register to read gas wait time
        res_heat0   = 0x5A,        // address of register to read heater resistance
        idac_heat0  = 0x50,        // address of register to read heater current
        gas_r_lsb   = 0x2B,        // address of register to read gas resistance LSB
        gas_r_msb   = 0x2A,        // address of register to read gas resistance MSB
    };

    // Commands
    enum class commands : uint8_t {
        softReset = 0xB6,        // value to write to resetRegister to reset the BME68X
    };

    // Other
    static constexpr uint8_t chipIdValue{0x61};        // value to expect at the chipIdregister, this allows to discover/recognize the BME68x

  private:
    static bool testI2cAddress(uint8_t addressToTest);                          //
    static uint8_t readRegister(registers aRegister);                           //
    static void writeRegister(registers aRegister, const uint8_t value);        //

    static void calculateTemperature();               //
    static void calculateBarometricPressure();        //
    static void calculateRelativeHumidity();          //
};
