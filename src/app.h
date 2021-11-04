/**
 * @file    app.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-28
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/
#ifndef __APP_H
#define __APP_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include "drivers/rtc.h"
#include "drivers/imu.h"
#include "drivers/display.h"
#include "drivers/network.h"
#include "drivers/config.h"
#include "drivers/qweather.h"
#include "drivers/genius.h"
#include "widget.h"
#include "lvgl.h"

/* Exported constants -------------------------------------------------------------------*/
#define APP_VERSION "1.0"


/* Exported types -----------------------------------------------------------------------*/
#define KEY_NONE        (0)
#define KEY_RIGHT       (1)
#define KEY_LEFT        (2)
#define KEY_HOLD_FRONT  (3)
#define KEY_HOLD_BACK   (4)
#define KEY_FRONT       (5)
#define KEY_BACK        (6)
#define KEY_REVERSE     (7)  // 反

#define KEY_L_R_CNT     (1)  // 向左、向右的持续时间
#define KEY_F_B_CNT     (2)  // 向前、向后的持续时间
#define KEY_REVERSE_CNT (50) // 翻转的持续时间

class KEY
{
private:
public:
    bool     holding;
    uint32_t holding_cnt;
    uint8_t  event_tmp;
    uint8_t  event;
};


class APP
{
private:

public:
    bool reset_wifi = false;
    bool is_extern_power = false;
    WIDGET widget;
    RTC rtc;
    IMU imu;
    Display display;
    Network wifi;
    config_t config;
    QWEATHER weather;
    GENIUS genius;
    KEY key;

public:
    APP();
    ~APP();

    void init(void);
    void sensor_monitor(void);
    void system_status_monitor(void);
};

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/
void page_startup_init(void);

#endif
