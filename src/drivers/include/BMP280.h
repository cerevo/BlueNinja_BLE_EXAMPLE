/**
 * @file   BMP280.h
 * @brief  Airpressure sensor(BOSH BMP280) driver for TZ10xx
 *
 * @author Cerevo Inc.
 */
 
/*
Copyright 2015 Cerevo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _BMP280_H_
#define _BMP280_H_

#include <stdint.h>
#include <stdbool.h>
#include "Driver_I2C.h"

#define BMP280_I2C_ADDR (0x76)

/* Registers. */
#define BMP280_REG_CALIB00      (0x88)
#define BMP280_REG_CALIB25      (0xa1)
#define BMP280_REG_ID           (0xd0)
#define BMP280_REG_RESET        (0xe0)
#define BMP280_REG_STATUS       (0xf3)
#define BMP280_REG_CTRL_MEAS    (0xf4)
#define BMP280_REG_CONFIG       (0xf5)
#define BMP280_REG_PRESS_MSB    (0xf7)
#define BMP280_REG_PRESS_LSB    (0xf8)
#define BMP280_REG_PRESS_XLSB   (0xf9)
#define BMP280_REG_TEMP_MSB     (0xfa)
#define BMP280_REG_TEMP_LSB     (0xfb)
#define BMP280_REG_TEMP_XLSB    (0xfc)

#define BMP280_BIT_RESET        (0xb6)


bool BMP280_drv_reset(void);
uint8_t BMP280_drv_id_get(void);
uint8_t BMP280_drv_status_get(void);
bool BMP280_drv_ctrl_meas_set(uint8_t val);
uint8_t BMP280_drv_ctrl_meas_get(void);
bool BMP280_drv_config_set(uint8_t val);
uint8_t BMP280_drv_config_get(void);
uint32_t BMP280_drv_press_get(void);
int32_t BMP280_drv_temp_get(void);
bool BMP280_drv_init(ARM_DRIVER_I2C *i2c_drv);


#endif  /* _BMP280_H_ */
