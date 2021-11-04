/**
 * @file    wether.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-21
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "ui.h"
#include "lvgl.h"

// #define IMU_DEBUG
// https://devapi.qweather.com/v7/weather/now?location=101280604&key=1defc3463b4947f98c1c9adb9420c7ed

/* Private typedef ----------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------------*/
lv_style_t style_default;
lv_style_t style_time;
lv_style_t style_oppo_sans_14;

const lv_img_dsc_t* img_weather_list[] =
{
    &image_sunny,
    &image_sunnyNight,
    &image_Cloudy,
    &image_LittleCloudyNight,
    &image_Overcast,
    &image_Overcast,
    &image_Isolated,
    &image_IsolatedNight,
    &image_HeavyRain,
    &image_HeavyRain,
    &image_Storm,
    &image_Storm,
    &image_Snow,
    &image_Snow,
    &image_sunny,
    &image_sunny
};

const  lv_img_dsc_t* img_photo_list[] =
{
    &image_photo1,
    &image_photo2,
    &image_photo3,
    &image_photo4,
    &image_photo5,
    &image_photo6,
    &image_photo7,
    &image_photo8,
    &image_photo9,
    &icon_photo10,
    &icon_photo11,
    &icon_photo12,
    &icon_photo13,
    &icon_photo14,
};

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   初始化
 */
void style_init(void)
{
    // 默认风格
    // lv_style_reset(&style_default);
    lv_style_init(&style_default);
    lv_style_set_bg_color(&style_default, lv_color_black());
    lv_style_set_text_font(&style_default, &lv_font_montserrat_16);
    lv_style_set_text_opa(&style_default, LV_OPA_COVER);
    lv_style_set_text_color(&style_default, lv_color_white());
    lv_style_set_arc_color(&style_default, lv_palette_main(LV_PALETTE_LIGHT_GREEN));

    // lv_style_reset(&style_time);
    lv_style_init(&style_time);
    lv_style_set_text_font(&style_time, &lv_font_montserrat_48);
    lv_style_set_text_opa(&style_time, LV_OPA_COVER);
    lv_style_set_text_color(&style_time, lv_color_white());

    // lv_style_reset(&style_oppo_sans_14);
    lv_style_init(&style_oppo_sans_14);
    lv_style_set_text_font(&style_oppo_sans_14, &OPPOSans_14);
    lv_style_set_text_opa(&style_oppo_sans_14, LV_OPA_COVER);
    lv_style_set_text_color(&style_oppo_sans_14, lv_color_white());
}
