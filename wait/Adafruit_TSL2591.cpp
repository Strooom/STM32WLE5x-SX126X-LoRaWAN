
#include "Adafruit_TSL2591.h"
#include <stdlib.h>
#include "main.h"
#include "logging.h"

extern I2C_HandleTypeDef hi2c2;

/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit TSL2591 class
    @param  sensorID An optional ID # so you can track this sensor, it will tag
   sensorEvents you create.
*/
/**************************************************************************/
Adafruit_TSL2591::Adafruit_TSL2591(int32_t sensorID) {
    _initialized = false;
    _integration = TSL2591_INTEGRATIONTIME_100MS;
    _gain        = TSL2591_GAIN_HIGH;
    _sensorID    = sensorID;

    // we cant do wire initialization till later, because we havent loaded Wire
    // yet
}

Adafruit_TSL2591::~Adafruit_TSL2591() {
    //  if (i2c_dev)
    //    delete i2c_dev;
}

/**************************************************************************/
/*!
    @brief   Setups the I2C interface and hardware, identifies if chip is found
    @param   theWire a reference to TwoWire instance
    @param   addr The I2C adress of the sensor (Default 0x29)
    @returns True if a TSL2591 is found, false on any failure
*/
/**************************************************************************/
bool Adafruit_TSL2591::begin() {
    uint8_t id = read8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_ID);
    if (id != 0x50) {
        return false;
    }
    // Serial.println("Found Adafruit_TSL2591");

    _initialized = true;

    // Set default integration time and gain
    setTiming(_integration);
    setGain(_gain);

    // Note: by default, the device is in power down mode on bootup
    disable();

    return true;
}
/**************************************************************************/
/*!
    @brief   Setups the I2C interface and hardware, identifies if chip is found
    @param   addr The I2C adress of the sensor (Default 0x29)
    @returns True if a TSL2591 is found, false on any failure
*/
/**************************************************************************/

/**************************************************************************/
/*!
    @brief  Enables the chip, so it's ready to take readings
*/
/**************************************************************************/
void Adafruit_TSL2591::enable(void) {
    if (!_initialized) {
        if (!begin()) {
            return;
        }
    }

    // Enable the device by setting the control bit to 0x01
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE,
           TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN |
               TSL2591_ENABLE_NPIEN);
}

/**************************************************************************/
/*!
    @brief Disables the chip, so it's in power down mode
*/
/**************************************************************************/
void Adafruit_TSL2591::disable(void) {
    if (!_initialized) {
        if (!begin()) {
            return;
        }
    }

    // Disable the device by setting the control bit to 0x00
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE,
           TSL2591_ENABLE_POWEROFF);
}

/************************************************************************/
/*!
    @brief  Setter for sensor light gain
    @param  gain {@link tsl2591Gain_t} gain value
*/
/**************************************************************************/
void Adafruit_TSL2591::setGain(tsl2591Gain_t gain) {
    if (!_initialized) {
        if (!begin()) {
            return;
        }
    }

    enable();
    _gain = gain;
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain);
    disable();
}

/************************************************************************/
/*!
    @brief  Getter for sensor light gain
    @returns {@link tsl2591Gain_t} gain value
*/
/**************************************************************************/
tsl2591Gain_t Adafruit_TSL2591::getGain() { return _gain; }

/************************************************************************/
/*!
    @brief  Setter for sensor integration time setting
    @param integration {@link tsl2591IntegrationTime_t} integration time setting
*/
/**************************************************************************/
void Adafruit_TSL2591::setTiming(tsl2591IntegrationTime_t integration) {
    if (!_initialized) {
        if (!begin()) {
            return;
        }
    }

    enable();
    _integration = integration;
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain);
    disable();
}

/************************************************************************/
/*!
    @brief  Getter for sensor integration time setting
    @returns {@link tsl2591IntegrationTime_t} integration time
*/
/**************************************************************************/
tsl2591IntegrationTime_t Adafruit_TSL2591::getTiming() { return _integration; }

/************************************************************************/
/*!
    @brief  Calculates the visible Lux based on the two light sensors
    @param  ch0 Data from channel 0 (IR+Visible)
    @param  ch1 Data from channel 1 (IR)
    @returns Lux, based on AMS coefficients (or < 0 if overflow)
*/
/**************************************************************************/
float Adafruit_TSL2591::calculateLux(uint16_t ch0, uint16_t ch1) {
    float atime, again;
    float cpl, lux;

    // Check for overflow conditions first
    if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF)) {
        // Signal an overflow
        return -1;
    }

    // Note: This algorithm is based on preliminary coefficients
    // provided by AMS and may need to be updated in the future

    switch (_integration) {
        case TSL2591_INTEGRATIONTIME_100MS:
            atime = 100.0F;
            break;
        case TSL2591_INTEGRATIONTIME_200MS:
            atime = 200.0F;
            break;
        case TSL2591_INTEGRATIONTIME_300MS:
            atime = 300.0F;
            break;
        case TSL2591_INTEGRATIONTIME_400MS:
            atime = 400.0F;
            break;
        case TSL2591_INTEGRATIONTIME_500MS:
            atime = 500.0F;
            break;
        case TSL2591_INTEGRATIONTIME_600MS:
            atime = 600.0F;
            break;
        default:        // 100ms
            atime = 100.0F;
            break;
    }

    switch (_gain) {
        case TSL2591_GAIN_LOW:
            again = 1.0F;
            break;
        case TSL2591_GAIN_MED:
            again = 25.0F;
            break;
        case TSL2591_GAIN_HIGH:
            again = 428.0F;
            break;
        case TSL2591_GAIN_MAX:
            again = 9876.0F;
            break;
        default:
            again = 1.0F;
            break;
    }

    // cpl = (ATIME * AGAIN) / DF
    cpl = (atime * again) / TSL2591_LUX_DF;

    // Original lux calculation (for reference sake)
    // float lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
    // float lux2 = ( ( TSL2591_LUX_COEFC * (float)ch0 ) - ( TSL2591_LUX_COEFD *
    // (float)ch1 ) ) / cpl; lux = lux1 > lux2 ? lux1 : lux2;

    // Alternate lux calculation 1
    // See: https://github.com/adafruit/Adafruit_TSL2591_Library/issues/14
    lux = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

    // Alternate lux calculation 2
    // lux = ( (float)ch0 - ( 1.7F * (float)ch1 ) ) / cpl;

    // Signal I2C had no errors
    return lux;
}

/************************************************************************/
/*!
    @brief  Reads the raw data from both light channels
    @returns 32-bit raw count where high word is IR, low word is IR+Visible
*/
/**************************************************************************/
uint32_t Adafruit_TSL2591::getFullLuminosity(void) {
    if (!_initialized) {
        if (!begin()) {
            return 0;
        }
    }

    // Enable the device
    enable();

    // Wait x ms for ADC to complete
    for (uint8_t d = 0; d <= _integration; d++) {
        HAL_Delay(120U);
        // delay(120);
    }

    // CHAN0 must be read before CHAN1
    // See: https://forums.adafruit.com/viewtopic.php?f=19&t=124176
    uint32_t x;
    uint16_t y;
    y = read16(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW);
    x = read16(TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW);
    logging::snprintf("O ch0 [%u], ch1 [%u]\n", y, x);
    x <<= 16;
    x |= y;

    disable();

    return x;
}

/************************************************************************/
/*!
    @brief  Reads the raw data from the channel
    @param  channel Can be 0 (IR+Visible, 1 (IR) or 2 (Visible only)
    @returns 16-bit raw count, or 0 if channel is invalid
*/
/**************************************************************************/
uint16_t Adafruit_TSL2591::getLuminosity(uint8_t channel) {
    uint32_t x = getFullLuminosity();

    if (channel == TSL2591_FULLSPECTRUM) {
        // Reads two byte value from channel 0 (visible + infrared)
        return (x & 0xFFFF);
    } else if (channel == TSL2591_INFRARED) {
        // Reads two byte value from channel 1 (infrared)
        return (x >> 16);
    } else if (channel == TSL2591_VISIBLE) {
        // Reads all and subtracts out just the visible!
        return ((x & 0xFFFF) - (x >> 16));
    }

    // unknown channel!
    return 0;
}

/************************************************************************/
/*!
    @brief  Set up the interrupt to go off when light level is outside the
   lower/upper range.
    @param  lowerThreshold Raw light data reading level that is the lower value
   threshold for interrupt
    @param  upperThreshold Raw light data reading level that is the higher value
   threshold for interrupt
    @param  persist How many counts we must be outside range for interrupt to
   fire, default is any single value
*/
/**************************************************************************/
void Adafruit_TSL2591::registerInterrupt(
    uint16_t lowerThreshold, uint16_t upperThreshold,
    tsl2591Persist_t persist = TSL2591_PERSIST_ANY) {
    if (!_initialized) {
        if (!begin()) {
            return;
        }
    }

    enable();
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_PERSIST_FILTER, persist);
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AILTL,
           lowerThreshold);
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AILTH,
           lowerThreshold >> 8);
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AIHTL,
           upperThreshold);
    write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_THRESHOLD_AIHTH,
           upperThreshold >> 8);
    disable();
}

/************************************************************************/
/*!
    @brief  Clear interrupt status
*/
/**************************************************************************/
void Adafruit_TSL2591::clearInterrupt() {
    if (!_initialized) {
        if (!begin()) {
            return;
        }
    }

    enable();
    //write8(TSL2591_CLEAR_INT);
    disable();
}

/************************************************************************/
/*!
    @brief  Gets the most recent sensor event from the hardware status register.
    @return Sensor status as a byte. Bit 0 is ALS Valid. Bit 4 is ALS Interrupt.
   Bit 5 is No-persist Interrupt.
*/
/**************************************************************************/
uint8_t Adafruit_TSL2591::getStatus(void) {
    if (!_initialized) {
        if (!begin()) {
            return 0;
        }
    }

    // Enable the device
    enable();
    uint8_t x;
    x = read8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_STATUS);
    disable();
    return x;
}

/************************************************************************/
/*!
    @brief  Gets the most recent sensor event
    @param  event Pointer to Adafruit_Sensor sensors_event_t object that will be
   filled with sensor data
    @return True on success, False on failure
*/
/**************************************************************************/
bool Adafruit_TSL2591::getEvent() {
    uint16_t ir, full;
    uint32_t lum = getFullLuminosity();
    /* Early silicon seems to have issues when there is a sudden jump in */
    /* light levels. :( To work around this for now sample the sensor 2x */
    lum  = getFullLuminosity();
    ir   = lum >> 16;
    full = lum & 0xFFFF;

    /* Calculate the actual lux value */
    /* 0 = sensor overflow (too much light) */
    float light = calculateLux(full, ir);

    return true;
}

/**************************************************************************/
/*!
    @brief  Gets the overall sensor_t data including the type, range and
   resulution
    @param  sensor Pointer to Adafruit_Sensor sensor_t object that will be
   filled with sensor type data
*/
/**************************************************************************/
// void Adafruit_TSL2591::getSensor(sensor_t *sensor) {
//     /* Clear the sensor_t object */
//     memset(sensor, 0, sizeof(sensor_t));

//     /* Insert the sensor name in the fixed length char array */
//     strncpy(sensor->name, "TSL2591", sizeof(sensor->name) - 1);
//     sensor->name[sizeof(sensor->name) - 1] = 0;
//     sensor->version                        = 1;
//     sensor->sensor_id                      = _sensorID;
//     sensor->type                           = SENSOR_TYPE_LIGHT;
//     sensor->min_delay                      = 0;
//     sensor->max_value                      = 88000.0;
//     sensor->min_value                      = 0.0;
//     sensor->resolution                     = 0.001;
// }
/*******************************************************/

uint8_t Adafruit_TSL2591::read8(uint8_t registerAddress) {
    uint8_t result[1]{0};
    HAL_I2C_Mem_Read(&hi2c2, 0x29 << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, result, 1, 0x100);
#ifdef showI2cCommunication
    logging::snprintf("Read I2C : register = [%02X], data = [%02X] \n", registerAddress, result[0]);
#endif
    return result[0];
}

uint16_t Adafruit_TSL2591::read16(uint8_t registerAddress) {
    uint8_t result[2]{0};
    HAL_I2C_Mem_Read(&hi2c2, 0x29 << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, result, 2, 0x100);
#ifdef showI2cCommunication
    logging::snprintf("Read I2C : register = [%02X], data = [%02X] \n", registerAddress, result[0]);
#endif
    return uint16_t(result[1]) << 8 | uint16_t(result[0]);
}

void Adafruit_TSL2591::write8(uint8_t registerAddress, uint8_t value) {
    HAL_I2C_Mem_Write(&hi2c2, 0x29 << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, &value, 1, 0x100);
}

// void Adafruit_TSL2591::write8(uint8_t registerAddress) {
//     HAL_I2C_Mem_Write(&hi2c2, 0x29 << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, &value, 1, 0x100);
// }
