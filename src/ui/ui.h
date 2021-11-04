/**
 * @file    style.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-28
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/
#ifndef __UI_H
#define __UI_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Exported constants -------------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------------------*/

// style
extern lv_style_t style_default;
extern lv_style_t style_time;
extern lv_style_t style_oppo_sans_14;

// font
LV_FONT_DECLARE(OPPOSans_14)

// icon
LV_IMG_DECLARE(icon_clock)
LV_IMG_DECLARE(icon_weatherclock)
LV_IMG_DECLARE(icon_weather)
LV_IMG_DECLARE(icon_photo)
LV_IMG_DECLARE(icon_inspire)
LV_IMG_DECLARE(icon_setting)

// images
LV_IMG_DECLARE(image_logo)
LV_IMG_DECLARE(image_startup)
LV_IMG_DECLARE(image_bilibili)
LV_IMG_DECLARE(image_sunny)
LV_IMG_DECLARE(image_sunnyNight)
LV_IMG_DECLARE(image_Cloudy)
LV_IMG_DECLARE(image_LittleCloudyNight)
LV_IMG_DECLARE(image_Overcast)
LV_IMG_DECLARE(image_Isolated)
LV_IMG_DECLARE(image_IsolatedNight)
LV_IMG_DECLARE(image_HeavyRain)
LV_IMG_DECLARE(image_Storm)
LV_IMG_DECLARE(image_Snow)
LV_IMG_DECLARE(image_timeout)

// photo
LV_IMG_DECLARE(image_photo1)
LV_IMG_DECLARE(image_photo2)
LV_IMG_DECLARE(image_photo3)
LV_IMG_DECLARE(image_photo4)
LV_IMG_DECLARE(image_photo5)
LV_IMG_DECLARE(image_photo6)
LV_IMG_DECLARE(image_photo7)
LV_IMG_DECLARE(image_photo8)
LV_IMG_DECLARE(image_photo9)
LV_IMG_DECLARE(icon_photo10)
LV_IMG_DECLARE(icon_photo11)
LV_IMG_DECLARE(icon_photo12)
LV_IMG_DECLARE(icon_photo13)
LV_IMG_DECLARE(icon_photo14)

extern const lv_img_dsc_t* img_weather_list[];

/* Exported functions -------------------------------------------------------------------*/
void style_init(void);

#ifdef __cplusplus
}
#endif

#endif
