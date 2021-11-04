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
#include "qweather.h"
#include "arduinojson.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define QWEATHER_LOG_ENABLE  0

#if QWEATHER_LOG_ENABLE
#define QWEATHER_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define QWEATHER_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define QWEATHER_LOG(format, ...)
#define QWEATHER_LOG_NEWLINE(format, ...)
#endif
 
/* Private typedef ----------------------------------------------------------------------*/
#define QWEATHER_SIMIULATION        0

#define WEATHER_UPDATE_TIME         (3600)  // 60min 更新一次天气预报
#define WEATHER_RETRY_TIME          (15)    // 更新失败，15S后重试
#define WEATHER_RETRY_MAX           (5)     // 60分钟内最多更新 5 次
#define LOCATION_NANSHAN            "101280604" // 南山区

#define WEATHER_STATE_NONE          (0)
#define WEATHER_STATE_TODAY         (1)
#define WEATHER_STATE_RELAX         (2)
#define WEATHER_STATE_FORECAST      (3)

/* Private variables --------------------------------------------------------------------*/
/*
const char *json = "{\"code\":\"200\",\"updateTime\":\"2021-10-22T22:02+08:00\",\"fxLink\":\"http://hfx.link/1u0z1\", \
\"now\":{\"obsTime\":\"2021-10-22T21:53+08:00\",\"temp\":\"17\",\"feelsLike\":\"14\",\"icon\":\"154\",\"text\":\"阴\", \
\"wind360\":\"0\",\"windDir\":\"北风\",\"windScale\":\"4\",\"windSpeed\":\"22\",\"humidity\":\"71\",\"precip\":\"0.0\", \
\"pressure\":\"1014\",\"vis\":\"30\",\"cloud\":\"97\",\"dew\":\"13\"},\"refer\":{\"sources\":[\"QWeather\",\"NMC\",\"ECMWF\"],\"license\":[\"no commercial use\"]}}";
*/

// 1000次/天
const char *weather_url = "https://devapi.qweather.com/v7/weather/now?location=";
const char *weather_key = "1defc3463b4947f98c1c9adb9420c7ed";

static uint8_t weather_update_state = WEATHER_STATE_TODAY;
static uint16_t weather_retry_cnt = 0;
static uint16_t weather_update_cnt = WEATHER_UPDATE_TIME;


/* Extern variables ---------------------------------------------------------------------*/
const char* forecast_sort_text_list[] =
{
    "晴",
    "云",
    "阴",
    "雨",
    "雷",
    "雪",
    "无",
};

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   获取天气类型
 * @param   index           text
 * @return  weather_code_t
 */
weather_code_t get_wather_type(int index)
{
    weather_code_t code = Code_Other;

    // Serial.println(index);
    switch (index)
    {
    case 100: // 晴
    case 150: // 晴晚
    case 102: // 少云
    case 152: // 少云晚
        code = Code_Sunny;
        break;
    case 101: // 多云
    case 151: // 多云晚
    case 103: // 晴间多云
    case 153: // 晴间多云晚
        code = Code_Cloudy;
        break;
    case 104: // 阴
    case 154:
    case 500: // 薄雾
    case 501: // 雾
    case 502: // 霾
        code = Code_Overcast;
        break;
    case 300: // 阵雨
    case 350: // 阵雨
        code = Code_Isolated;
        break;
    // --- 雨
    case 301: // 强阵雨
    case 305: // 小雨
    case 306: // 中雨
    case 307: // 大雨
    case 308: // 极端降雨
    case 309: // 毛毛雨/细雨
    case 310: // 暴雨
    case 311: // 大暴雨
    case 312: // 特大暴雨
    case 313: // 冻雨
    case 314: // 小到中雨
    case 315: // 小到中雨
    case 316: // 大到暴雨
    case 317: // 暴雨到大暴雨
    case 318: // 大暴雨到特大暴雨
    case 351: // 强阵雨
    case 399: // 雨
        code = Code_HeavyRain;
        break;
    case 302: // 雷阵雨
    case 303: // 强雷阵雨
    case 304: // 雷阵雨伴有冰雹
        code = Code_Storm;
        break;
    // 雪
    // case 400: // 小雪
    // case 401: // 中雪
    // case 405: // 大雪
    // case 406: // 雨夹雪
    // case 407: // 雨雪天气
    // case 408: // 阵雨夹雪
    // case 409: // 阵雪
    // case 410: // 小到中雪
    // case 411: // 中到大雪
    // case 412: // 大到暴雪
    // case 413: // 阵雨夹雪
    // case 414: // 阵雪
    // case 499: // 雪
    //     /* code */
    //     break;
    default:
        // 雪
        if (index >= 400 && index <= 499)
        {
            code = Code_Snow;
        }
        // 雾、霾、沙尘暴
        else if (index >= 500 && index <= 515)
        {
            code = Code_Overcast;
        }
        // 新月,蛾眉月,上弦月,盈凸月,满月,亏凸月,下弦月,残月
        // else if (index >= 800 && index <= 807)
        // {
        //     code = Code_Sunny;
        // }
        else
        {
            code = Code_Sunny;
        }
        break;
    }

    // Serial.println(code);
    return code;
}

/**
 * @brief   天气预报
 */
int QWEATHER::get_forcast(String location)
{

#if QWEATHER_SIMIULATION
    forecast_result = 200;
    // 2021-10-23T11:07+08:00 -> 11:07
    forecast[0].code = 100;
    forecast[0].tmp_min = 25;
    forecast[0].tmp_max = 28;
    forecast[0].humidity = 70;
    forecast[0].textDay = "晴";
    forecast[0].textNight = "晴";
    forecast[0].windDir = "北风";
    forecast[0].windScale = "1";
    forecast_index[0] = get_wather_type(forecast[0].code);

    forecast[1].code = 101;
    forecast[1].tmp_min = 26;
    forecast[1].tmp_max = 27;
    forecast[1].humidity = 70;
    forecast[1].textDay = "多云";
    forecast[1].textNight = "多云";
    forecast[1].windDir = "北风";
    forecast[1].windScale = "2";
    forecast_index[1] = get_wather_type(forecast[1].code);

    forecast[2].code = 302;
    forecast[2].tmp_min = 28;
    forecast[2].tmp_max = 29;
    forecast[2].humidity = 70;
    forecast[2].textDay = "雷阵雨";
    forecast[2].textNight = "雷阵雨";
    forecast[2].windDir = "北风";
    forecast[2].windScale = "3";
    forecast_index[2] = get_wather_type(forecast[2].code);

    return true;
#else

    std::unique_ptr<WiFiClientSecure> client(new WiFiClientSecure);
    client->setInsecure(); // 不进行服务器身份认证

    HTTPClient https;
    String request_url = "https://devapi.qweather.com/v7/weather/3d?location=" + location + "&key=" + weather_key + "&lang=cn&gzip=n"; // 不启用压缩

    QWEATHER_LOG_NEWLINE("[HTTPS] begin...\r\n");

    forecast_result = 0;
    https.setTimeout(3000);
    if (https.begin(*client, request_url))
    {
        int httpCode = https.GET();

        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = https.getString();
                QWEATHER_LOG_NEWLINE("ok");

                // QWEATHER_LOG_NEWLINE(payload);
                // Serial.flush();
                const size_t capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(2) +
                                        JSON_OBJECT_SIZE(5) + 3 * JSON_OBJECT_SIZE(26) + 1320;

                DynamicJsonDocument doc(capacity);
                DeserializationError error = deserializeJson(doc, payload);
                if (!error)
                {
                    forecast_result = doc["code"].as<int>();
                    String updateTime = doc["updateTime"].as<String>();
                    forecast_update = updateTime.substring(updateTime.indexOf('T'), 5);
                    QWEATHER_LOG_NEWLINE("updateTime:" + forecast_update);

                    if (forecast_result == 200)
                    {
                        JsonArray daily = doc["daily"];

                        for (int i = 0; i < 3; i++)
                        {
                            JsonObject day = daily[i];

                            forecast[i].code = day["iconDay"].as<int>();
                            forecast[i].tmp_min = day["tempMin"].as<int>();
                            forecast[i].tmp_max = day["tempMax"].as<int>();
                            forecast[i].humidity = day["humidity"].as<int>();
                            forecast[i].textDay = day["textDay"].as<String>();
                            forecast[i].textNight = day["textNight"].as<String>();
                            forecast[i].windDir = day["windDirDay"].as<String>();
                            forecast[i].windScale = day["windScaleDay"].as<String>();
                            forecast_index[i] = get_wather_type(forecast[i].code);

                            QWEATHER_LOG_NEWLINE("code:" + String(forecast[i].code));
                            QWEATHER_LOG_NEWLINE("tmp_min:" + String(forecast[i].tmp_min));
                            QWEATHER_LOG_NEWLINE("tmp_max:" + String(forecast[i].tmp_max));
                            QWEATHER_LOG_NEWLINE("humidity:" + String(forecast[i].humidity));
                            QWEATHER_LOG_NEWLINE("textDay:" + forecast[i].textDay);
                            QWEATHER_LOG_NEWLINE("textNight:" + forecast[i].textNight);
                            QWEATHER_LOG_NEWLINE("windDir:" + forecast[i].windDir);
                            QWEATHER_LOG_NEWLINE("windScale:" + forecast[i].windScale);
                        }
                    }
                }
                else
                {
                    QWEATHER_LOG_NEWLINE(error.f_str());
                }
                doc.clear();
            }
        }
        else
        {
            QWEATHER_LOG_NEWLINE(https.errorToString(httpCode).c_str());
        }

        https.end();
    }

    return forecast_result;
#endif
}

/**
 * @brief   实时天气
 */
int QWEATHER::get_now(String location)
{

#if QWEATHER_SIMIULATION

    today_result = 200;
    // 2021-10-23T11:07+08:00 -> 11:07
    today.update_time = "08:00";
    today.temp = 25;
    today.humidity = 75;
    today.windDir = "北风";
    today.windScale = "3";
    today.windSpeed = "10";
    today.code = 100;
    today.text = "晴转多云";
    today_index = get_wather_type(today.code);

    return true;
#else

    std::unique_ptr<WiFiClientSecure> client(new WiFiClientSecure);
    client->setInsecure(); // 不进行服务器身份认证

    HTTPClient https;
    String request_url = "https://devapi.qweather.com/v7/weather/now?location=" + location + "&key=" + weather_key + "&lang=cn&gzip=n"; // 不启用压缩

    QWEATHER_LOG_NEWLINE("[HTTPS] begin...\r\n");
    // QWEATHER_LOG_NEWLINE(request_url);
    // Serial.flush();
    today_result = 0;
    https.setTimeout(3000);
    if (https.begin(*client, request_url))
    {
        int httpCode = https.GET();

        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                String payload = https.getString();
                // QWEATHER_LOG_NEWLINE(payload);

                DynamicJsonDocument doc(1024);
                DeserializationError error = deserializeJson(doc, payload);
                if (!error)
                {
                    JsonObject now = doc["now"];

                    today_result = doc["code"].as<int>();
                    if (today_result == 200)
                    {
                        String updateTime = doc["updateTime"].as<String>();
                        // 2021-10-23T11:07+08:00 -> 11:07
                        today.update_time = updateTime.substring(updateTime.indexOf('T'), 5);
                        today.code = now["icon"].as<int>();
                        today.temp = now["temp"].as<int>();
                        today.humidity = now["humidity"].as<int>();
                        today.windDir = now["windDir"].as<String>();
                        today.windScale = now["windScale"].as<String>();
                        today.windSpeed = now["windSpeed"].as<String>();
                        today.text = now["text"].as<String>();
                        today_index = get_wather_type(today.code);

                        QWEATHER_LOG_NEWLINE("updateTime:" + today.update_time);
                        QWEATHER_LOG_NEWLINE("icon:" + String(today.code));
                        QWEATHER_LOG_NEWLINE("temp:" + String(today.temp));
                        QWEATHER_LOG_NEWLINE("humidity:" + String(today.humidity));
                        QWEATHER_LOG_NEWLINE("windSpeed:" + today.windSpeed);
                        QWEATHER_LOG_NEWLINE(today.text);

                        return true;
                    }
                }
                else
                {
                    QWEATHER_LOG_NEWLINE(error.f_str());
                }
                doc.clear();
            }
        }
        else
        {
            QWEATHER_LOG_NEWLINE(https.errorToString(httpCode).c_str());
        }

        https.end();
    }

    return today_result;
#endif
}

/**
 * @brief   更新天气状态机
 */
void QWEATHER::update(void)
{
    static uint8_t last_state = WEATHER_STATE_NONE;

    switch (weather_update_state)
    {
    // 更新天气
    case WEATHER_STATE_TODAY:
    case WEATHER_STATE_FORECAST:
    {
        bool result;

        if (weather_update_state == WEATHER_STATE_TODAY)
        {
            QWEATHER_LOG_NEWLINE("WEATHER TODAY");
            result = get_now(LOCATION_NANSHAN);
        }
        else
        {
            QWEATHER_LOG_NEWLINE("WEATHER FORECAST");
            result = get_forcast(LOCATION_NANSHAN);
        }

        if (result != 0)
        {
            if (weather_update_state == WEATHER_STATE_TODAY)
            {
                weather_update_state = WEATHER_STATE_FORECAST;
            }
            else
            {
                weather_update_state = WEATHER_STATE_NONE;
            }
        }
        else
        {
            // 重试超过次数，不再更新
            if (weather_retry_cnt++ >= WEATHER_RETRY_MAX)
            {
                weather_update_state = WEATHER_STATE_NONE;
            }
            else
            {
                last_state = weather_update_state;
                weather_update_state = WEATHER_STATE_RELAX;
            }
        }
        weather_update_cnt = 0;
    }
    break;
    // 过会儿再更新
    case WEATHER_STATE_RELAX:
        if (weather_update_cnt >= WEATHER_RETRY_TIME)
        {
            weather_update_state = last_state;
        }
        else
        {
            weather_update_cnt++;
        }
        break;
    case WEATHER_STATE_NONE:
    default:
        if (weather_update_cnt >= WEATHER_UPDATE_TIME)
        {
            weather_retry_cnt = 0;
            weather_update_state = WEATHER_STATE_TODAY;
        }
        else
        {
            weather_update_cnt++;
        }

        // // 轮流切换显示
        // if (weather_switch_cnt == 0)
        // {
        //     WEATHER_LOG_NEWLINE("today:" + String(weather.today_result));
        //     if (weather.today_result == 200)
        //     {
        //         gui_update_today(get_wather_type(weather.today.code), &weather.today);
        //     }
        // }
        // else if (weather_switch_cnt == WEATHER_SWITCH_TIME)
        // {
        //     WEATHER_LOG_NEWLINE("forecast:" + String(weather.forecast_result));
        //     if (weather.forecast_result == 200)
        //     {
        //         gui_update_forecast(weather.forecast);
        //     }
        // }

        // weather_switch_cnt++;
        // if (weather_switch_cnt >= (WEATHER_SWITCH_TIME * 2))
        // {
        //     weather_switch_cnt = 0;
        // }

        break;
    }
}
