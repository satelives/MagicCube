/**
 * @file    display.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-20
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/

#ifndef __IMU_H
#define __IMU_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include "MPU6050.h"

/* Exported constants -------------------------------------------------------------------*/
#define IMU_I2C_SDA 21
#define IMU_I2C_SCL 22

/* Exported types -----------------------------------------------------------------------*/
class IMU_DATA
{
public:
    int16_t x, y, z;
};

class IMU
{
private:
    MPU6050 imu;
    int flag;
    IMU_DATA Accel;
    IMU_DATA Gyro;

public:
    bool is_vaild = false;
    bool init();
    bool update(void);

    IMU_DATA getAccel();
    IMU_DATA getGyro();

    int16_t getAccelX();
    int16_t getAccelY();
    int16_t getAccelZ();

    int16_t getGyroX();
    int16_t getGyroY();
    int16_t getGyroZ();
};

/* Exported variables -------------------------------------------------------------------*/
// extern int32_t encoder_diff;
// extern lv_indev_state_t encoder_state;

/* Exported functions -------------------------------------------------------------------*/


#endif