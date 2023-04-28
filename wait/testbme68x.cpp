#include "logging.h"
#include <stdio.h>
#include "bme68x.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;


// #include "common.h"
// #include "coines.h"
BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

int testbme68x(void) {
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;
    uint32_t del_period;
    uint32_t time_ms = 0;
    uint8_t n_fields;
    uint16_t sample_count = 1;

    //dev_addr      = BME68X_I2C_ADDR_LOW;
    bme.read     = bme68x_i2c_read;
    bme.write    = bme68x_i2c_write;
    bme.intf     = BME68X_I2C_INTF;
    //bme.delay_us = bme68x_delay_us;
    //bme->intf_ptr = &dev_addr;
    bme.amb_temp = 25; /* The ambient temperature in deg C is used for defining the heater temperature */

    //    rslt = bme68x_interface_init(&bme, BME68X_I2C_INTF);
    //    bme68x_check_rslt("bme68x_interface_init", rslt);

    rslt = bme68x_init(&bme);
    // bme68x_check_rslt("bme68x_init", rslt);

    conf.filter  = BME68X_FILTER_OFF;
    conf.odr     = BME68X_ODR_NONE;
    conf.os_hum  = BME68X_OS_16X;
    conf.os_pres = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    rslt         = bme68x_set_conf(&conf, &bme);
    // bme68x_check_rslt("bme68x_set_conf", rslt);

    heatr_conf.enable     = BME68X_DISABLE;
    heatr_conf.heatr_temp = 300;
    heatr_conf.heatr_dur  = 100;
    rslt                  = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);
    // bme68x_check_rslt("bme68x_set_heatr_conf", rslt);

    while (sample_count <= 1) {
        rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
        // bme68x_check_rslt("bme68x_set_op_mode", rslt);

        /* Calculate delay period in microseconds */
        del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
        HAL_Delay(500);
        //bme.delay_us(del_period, bme.intf_ptr);

        //time_ms = coines_get_millis();

        rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
        // bme68x_check_rslt("bme68x_get_data", rslt);

        if (n_fields) {
            //logging::snprintf("T %.3f / %.3f / %.3f\n", data.temperature, data.humidity, data.pressure);
            sample_count++;
        }
    }

    // bme68x_coines_deinit();

    return rslt;
}

BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    HAL_I2C_Mem_Read(&hi2c2, 0x76 << 1, static_cast<uint16_t>(reg_addr), I2C_MEMADD_SIZE_8BIT, reg_data, len, 0x10);

    // logging::snprintf("Read I2C : register = [%02X], data[%u] = [", reg_addr, len);
    // for (uint32_t index = 0; index < len; index++) {
    // 	logging::snprintf("%02X ", reg_data[index]);
    // }
    // logging::snprintf("]\n");
    return BME68X_INTF_RET_SUCCESS;
}

BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    // logging::snprintf("Write I2C : register = [%02X], data[%u] = [", reg_addr, len);
    // for (uint32_t index = 0; index < len; index++) {
    // 	logging::snprintf("%02X ", reg_data[index]);
    // }
    // logging::snprintf("]\n");

    HAL_I2C_Mem_Write(&hi2c2, 0x76 << 1, static_cast<uint16_t>(reg_addr), I2C_MEMADD_SIZE_8BIT, (uint8_t *)reg_data, len, 0x10);
    return BME68X_INTF_RET_SUCCESS;
}



void bme68x_delay();