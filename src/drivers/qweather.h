/**
 * @file    weather.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-21
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/

#ifndef __QWEATHER_H
#define __QWEATHER_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"

/* Exported constants -------------------------------------------------------------------*/
extern const char* forecast_sort_text_list[];

/* Exported types -----------------------------------------------------------------------*/
typedef enum
{
    Code_Sunny = 0,
    Code_Cloudy,
    Code_Overcast,
    Code_Isolated,
    Code_HeavyRain,
    Code_Storm,
    Code_Snow,
    Code_Other,
    Code_Max,
} weather_code_t;

typedef struct qweather_now
{
    int code;               // 天气代码
    String update_time;     // 天气数据更新时间
    int temp;               // 温度
    int humidity;           // 湿度
    String text;            // 天气状况描述
    String windDir;         // 风向
    String windScale;       // 风力
    String windSpeed;       // 风速
} qweather_now_t;

typedef struct qweather_forecast
{
    int code;               // 天气代码
    int tmp_min;            // 最低温度
    int tmp_max;            // 最高温度
    int humidity;           // 湿度
    String textDay;
    String textNight;
    String windDir;         // 风向
    String windScale;       // 风力
} qweather_forecast_t;

class QWEATHER
{
private:
    // int getIconCode(String cond_code);

public:
    int today_result;                   // 结果：200为成功
    weather_code_t today_index;
    qweather_now_t today;

    int forecast_result;                // 结果：200为成功
    String forecast_update;             // 更新时间
    weather_code_t forecast_index[3];
    qweather_forecast_t forecast[3];

    int get_now(String location);
    int get_forcast(String location);
    void update(void);
};

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

#endif