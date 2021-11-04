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
#include "config.h"
#include <Preferences.h>


// #define IMU_DEBUG

/* Private typedef ----------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------------*/
Preferences prefs;

/* Extern variables ---------------------------------------------------------------------*/
/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

void config_read(const char *name, config_t *cfg)
{
    prefs.begin(name); // 打开命名空间 mynamespace

    cfg->is_vaild       = prefs.getUInt("is_vaild", 0);

    cfg->default_ssid   = prefs.getString("default_ssid", "Seven");
    cfg->default_key    = prefs.getString("default_key", "zhang28jian28");
    cfg->wifi_ssid      = prefs.getString("wifi_ssid", "BDFLY");
    cfg->wifi_key       = prefs.getString("wifi_key", "20210818");
    cfg->wifi1_ssid     = prefs.getString("wifi_ssid", "Seven");
    cfg->wifi1_key      = prefs.getString("wifi_key", "zhang28jian28");

    // cfg->wifi1_ssid     = prefs.getString("wifi1_ssid", "Seven");
    // cfg->wifi1_key      = prefs.getString("wifi1_key", "zhang28jian28");
    // cfg->wifi2_ssid     = prefs.getString("wifi2_ssid", "Seven");
    // cfg->wifi2_key      = prefs.getString("wifi2_key", "zhang28jian28");

    cfg->city_name      = prefs.getString("city_name", "南山");
    cfg->qweather_key   = prefs.getString("qweather_key", "1defc3463b4947f98c1c9adb9420c7ed");

    prefs.end(); // 关闭当前命名空间

    Serial.println(cfg->is_vaild);
    Serial.println(cfg->default_ssid);
    Serial.println(cfg->default_key);
    Serial.println(cfg->wifi_ssid);
    Serial.println(cfg->wifi_key);
    // Serial.println(cfg->wifi1_ssid);
    // Serial.println(cfg->wifi1_key);
    // Serial.println(cfg->city_name);
    // Serial.println(cfg->qweather_key);
}

void config_save(const char *name, config_t *cfg)
{
    // String line("\n");
    // String res = cfg->ssid + line + cfg->password + line + cfg->cityname + line + cfg->language + line + cfg->weather_key;
    // Serial.println(res);
    // tf.deleteFile(file_path);
    // tf.writeFile(file_path, res.c_str());

    prefs.begin(name); // 打开命名空间mynamespace

    cfg->is_vaild = 0xAA55AA55;
    prefs.putUInt("is_vaild", cfg->is_vaild);
    prefs.putString("default_ssid", cfg->default_ssid);
    prefs.putString("default_key", cfg->default_key);
    prefs.putString("wifi_ssid", cfg->wifi_ssid);
    prefs.putString("wifi_key", cfg->wifi_key);
    // prefs.putString("wifi1_ssid", cfg->wifi1_ssid);
    // prefs.putString("wifi1_key", cfg->wifi1_key);
    // prefs.putString("wifi2_ssid", cfg->wifi2_ssid);
    // prefs.putString("wifi2_key", cfg->wifi2_key);

    prefs.putString("city_name", cfg->city_name);
    prefs.putString("qweather_key", cfg->qweather_key);

    prefs.end(); // 关闭当前命名空间
}
