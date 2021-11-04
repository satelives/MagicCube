/**
 * @file    display.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-20
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "imu.h"

#define IMU_LOG_ENABLE 1

#if IMU_LOG_ENABLE
#define IMU_LOG(format, ...) Serial.print(format, ##__VA_ARGS__)
#define IMU_LOG_NEWLINE(format, ...) Serial.println(format, ##__VA_ARGS__)
#else
#define IMU_LOG(format, ...)
#define IMU_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------------------*/
/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   初始化
 */
bool IMU::init()
{
    Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
    Wire.setClock(400000);

    is_vaild = false;
    int timeout = 0;

    IMU_LOG_NEWLINE("imu init...");
    while (!is_vaild && (timeout <= 50)) // 5S
    {
        delay(100);
        is_vaild = imu.testConnection();
        timeout++;
    }

    if (is_vaild)
    {
        imu.initialize();
        IMU_LOG_NEWLINE("imu init ok");
    }
    else
    {
        IMU_LOG_NEWLINE("imu init fail");
        IMU_LOG_NEWLINE(imu.getDeviceID());
    }

    return is_vaild;
}

/**
 * @brief
 * @param   interval        text
 */
bool IMU::update(void)
{
    if (is_vaild)
    {
        imu.getMotion6(&Accel.x, &Accel.y, &Accel.z, &Gyro.x, &Gyro.y, &Gyro.z);
// #if IMU_LOG_ENABLE
//         char data[128];
//         sprintf(data, "%6d,%6d,%6d %6d,%6d,%6d", Accel.x, Accel.y, Accel.z, Gyro.x, Gyro.y, Gyro.z);
//         IMU_LOG_NEWLINE(data);
// #endif

    }
    else
    {
        IMU_LOG_NEWLINE("imu error");
        return false;
    }

    return true;
}

IMU_DATA IMU::getAccel()
{
    return Accel;
}

IMU_DATA IMU::getGyro()
{
    return Gyro;
}

int16_t IMU::getAccelX()
{
    return Accel.x;
}

int16_t IMU::getAccelY()
{
    return Accel.y;
}

int16_t IMU::getAccelZ()
{
    return Accel.z;
}

int16_t IMU::getGyroX()
{
    return Gyro.x;
}

int16_t IMU::getGyroY()
{
    return Gyro.y;
}

int16_t IMU::getGyroZ()
{
    return Gyro.z;
}

