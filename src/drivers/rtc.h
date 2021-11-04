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

#ifndef __RTC_H
#define __RTC_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include "RTClib.h"

/* Exported constants -------------------------------------------------------------------*/
#define NTP_CAL_TIME         (3600)  // 1 h

/* Exported types -----------------------------------------------------------------------*/
class RTC
{
private:
    uint32_t ntp_cal_cnt = NTP_CAL_TIME;

public:
    bool is_vaild;
    bool is_ntp_enable;
    bool is_cal;
    bool is_night;
    DateTime now;
    struct tm time;
    long get_timestamp(tm *t);
    long get_timestamp(DateTime *dt);
    void setTime(DateTime *dt);
    void setTime(struct tm *t);
    void setTime(long timestamp);

    void init(bool is_sync_ntp);
    void update();
    bool ntp_cal_time(void);
    bool cal_rtc(tm *t);
    bool cal_rtc(DateTime *dt);
    void setTimeZone(long offset, int daylight);
};

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/


#endif