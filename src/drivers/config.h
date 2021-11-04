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

#ifndef __CONFIG_H
#define __CONFIG_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"

/* Exported constants -------------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------------------*/
typedef struct Config_desc
{
    uint32_t is_vaild;
    // wifi
    String default_ssid;        // 默认wifi
    String default_key;
    String wifi_ssid;           // 首选wifi
    String wifi_key;
    String wifi1_ssid;
    String wifi1_key;

    String city_name;           // 显示的城市
    String language;            // 天气查询的地址编码
    String qweather_key;        // 和风天气api_key（秘钥）
    // uint8_t backLight;          // 屏幕亮度（1-100）
    // uint8_t rotation;           // 屏幕旋转方向
} config_t;

/* Exported variables -------------------------------------------------------------------*/
// extern int32_t encoder_diff;
// extern lv_indev_state_t encoder_state;

/* Exported functions -------------------------------------------------------------------*/
void config_read(const char *name, config_t *cfg);
void config_save(const char *name, config_t *cfg);

#endif