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
#include "rtc.h"
#include "time.h"
#include <sys/time.h>
#include "network.h"
// #include "app.h"

#include "esp32-hal.h"


#define RTC_LOG_ENABLE 1

#if RTC_LOG_ENABLE
#define RTC_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define RTC_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define RTC_LOG(format, ...)
#define RTC_LOG_NEWLINE(format, ...)
#endif

// struct tm
// {
//     int tm_sec;   /* seconds after the minute - [0,59] */
//     int tm_min;   /* minutes after the hour - [0,59] */
//     int tm_hour;  /* hours since midnight - [0,23] */
//     int tm_mday;  /* day of the month - [1,31] */
//     int tm_mon;   /* months since January - [0,11] */
//     int tm_year;  /* years since 1900 */
//     int tm_wday;  /* days since Sunday - [0,6] */
//     int tm_yday;  /* days since January 1 - [0,365] */
//     int tm_isdst; /* daylight savings time flag */
// };

/* Private typedef ----------------------------------------------------------------------*/
static RTC_DS3231 ds3231;
// extern APP *app;
/* Extern variables ---------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------------------*/

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   设置时区
 * @param   offset          text
 * @param   daylight        text
 */
void RTC::setTimeZone(long offset, int daylight)
{
    // char cst[17] = {0};
    // char cdt[17] = "DST";
    // char tz[33] = {0};

    // if (offset % 3600)
    // {
    //     sprintf(cst, "UTC%ld:%02u:%02u", offset / 3600, abs((offset % 3600) / 60), abs(offset % 60));
    // }
    // else
    // {
    //     sprintf(cst, "UTC%ld", offset / 3600);
    // }
    // if (daylight != 3600)
    // {
    //     long tz_dst = offset - daylight;
    //     if (tz_dst % 3600)
    //     {
    //         sprintf(cdt, "DST%ld:%02u:%02u", tz_dst / 3600, abs((tz_dst % 3600) / 60), abs(tz_dst % 60));
    //     }
    //     else
    //     {
    //         sprintf(cdt, "DST%ld", tz_dst / 3600);
    //     }
    // }
    // sprintf(tz, "%s%s", cst, cdt);
    // setenv("TZ", tz, 1);
    // tzset();
}

/**
 * @brief   获取时间戳
 * @param   datetime        text
 * @return  long
 */
long RTC::get_timestamp(DateTime *dt)
{
    // seconds, minute, hour, day, month, year $ microseconds(optional)
    // ie setTime(20, 34, 8, 1, 4, 2021) = 8:34:20 1/4/2021
    tm t = {0};
    t.tm_year = dt->year() - 1900; //since 1900, 121 = 2021
    t.tm_mon = dt->month() - 1;    // 0..11
    t.tm_mday = dt->day();
    t.tm_hour = dt->hour();
    t.tm_min = dt->minute();
    t.tm_sec = dt->second();

    return mktime(&t);
}

/**
 * @brief   获取时间戳
 * @param   t               text
 * @return  long
 */
long RTC::get_timestamp(tm *t)
{
    return mktime(t);
}

/**
 * @brief   设置系统时间
 * @param   epoch           unix 时间戳
 */
void RTC::setTime(long timestamp)
{
    struct timeval tv;
    tv.tv_sec = timestamp;  // epoch time (seconds)
    tv.tv_usec = 0;         // microseconds
    settimeofday(&tv, 0);
}

/**
 * @brief   设置系统时间
 * @param   datetime        text
 */
void RTC::setTime(DateTime* dt)
{
    long timestamp = get_timestamp(dt);
    setTime(timestamp);
}

/**
 * @brief   设置系统时间
 * @param   datetime        text
 */
void RTC::setTime(tm *t)
{
    long timestamp = get_timestamp(t);
    setTime(timestamp);
}

/**
 * @brief   初始化
 */
void RTC::init(bool ntp_enable)
{
    // 开启网络时间同步
    is_ntp_enable = ntp_enable;

    if (is_ntp_enable)
    {
        // 默认同步间隔
        // #define SNTP_UPDATE_DELAY           3600000
        configTime(8 * 3600, 0, "ntp1.aliyun.com", "ntp1.aliyun.com", "ntp1.aliyun.com");
    }

    // 设置时区
    // setTimeZone(8 * 3600, 0);

    // 初始化DS3231，修正系统时间
    if (ds3231.begin())
    {
        // 电池供电时使能振荡器
        uint8_t status = ds3231.get_control_status();
        if (status & 0x80)
        {
            ds3231.enable_battery();

        }

        // 同步到系统时间
        DateTime now = ds3231.now();
        setTime(&now);
        is_vaild = true;

#if RTC_LOG_ENABLE
        char s[50];
        snprintf(s, 50, "%4d-%2d-%2d %2d:%2d:%2d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
        RTC_LOG_NEWLINE(s);
        RTC_LOG_NEWLINE("RTC OK");
#endif

    }
    else
    {
        is_vaild = false;
        RTC_LOG_NEWLINE("Couldn't find RTC");
    }

    ntp_cal_cnt = 0;
}

/**
 * @brief   使用网络时间校正DS3231
 * @return  true
 * @return  false
 */
bool RTC::ntp_cal_time(void)
{
    bool cal = false;

    if (is_ntp_enable)
    {
        // Network ntp;
        long ntp_timestmp;
#if 1
        tm t;

        getLocalTime(&t, 0);
        ntp_timestmp = get_timestamp(&t);
        RTC_LOG_NEWLINE(ntp_timestmp);

        if (ntp_timestmp > 1635242538) // 2021-10-26 18:02:18
        {
            DateTime now = ds3231.now();
            long rtc_timestmp = get_timestamp(&now);
            RTC_LOG_NEWLINE(rtc_timestmp);

            int delta = (int)(ntp_timestmp - rtc_timestmp);
            if (abs(delta) > 5)
            {
                DateTime ad = DateTime((uint32_t)ntp_timestmp + 8 * 3600); // GMT +8
                ds3231.adjust(ad);
                RTC_LOG_NEWLINE("cal ds3231");
            }
        }
#else
        ntp_timestmp = (long)(app->wifi.getTimestamp());
        RTC_LOG_NEWLINE(ntp_timestmp);
        if (ntp_timestmp > 1635232990) // 2021-10-26 15:23:10
        {
            // 修正系统时间
            setTime(ntp_timestmp);
            // 修正DS3231时间
            DateTime now = ds3231.now();
            long rtc_timestmp = get_timestamp(&now);
            int delta = (int)(ntp_timestmp - rtc_timestmp);
            if (abs(delta) > 5)
            {
                DateTime ad = DateTime((uint32_t)ntp_timestmp);
                ds3231.adjust(ad);
                RTC_LOG_NEWLINE("cal ds3231");
            }

            cal = true;
            #if RTC_LOG_ENABLE
                char s[50];
                snprintf(s, 50, "ntp cal ok %ld", ntp_timestmp);
                RTC_LOG_NEWLINE(s);
            #endif
        }
#endif
    }
    else
    {
        RTC_LOG_NEWLINE("ntp cal is disable");
    }


    return cal;
}

/**
 * @brief   强制校正DS3231
 * @param   t               text
 * @return  true
 * @return  false
 */
bool RTC::cal_rtc(tm *t)
{
    long timestamp = get_timestamp(t);

    ds3231.adjust(DateTime(timestamp));

    return true;
}

/**
 * @brief   强制校正DS3231
 * @param   dt              text
 * @return  true
 * @return  false
 */
bool RTC::cal_rtc(DateTime *dt)
{
    long timestamp = get_timestamp(dt);

    ds3231.adjust(DateTime(timestamp));

    return true;
}

/**
 * @brief
 * @param   interval        text
 */
void RTC::update()
{
    getLocalTime(&time, 0);
    now = DateTime(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

    is_night = ((time.tm_hour >= 19) && (time.tm_hour <= 5)) ? true : false;

    // cal ds3231
    if (is_ntp_enable)
    {
        if (ntp_cal_cnt >= NTP_CAL_TIME)
        {
            ntp_cal_cnt = 0;
            is_cal = ntp_cal_time();
        }
        else
        {
            ntp_cal_cnt++;
        }
    }

// #if RTC_LOG_ENABLE
//     char s[50];
//     snprintf(s, 50, "%d-%d-%d %d:%d:%d", now.year(), now.month(), now.month(), now.day(), now.hour(), now.minute(), now.second());
//     RTC_LOG_NEWLINE(s);
// #endif

}

