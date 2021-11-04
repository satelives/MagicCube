/**
 * @file    weather.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-29
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "weather.h"
#include "app.h"
#include "widget.h"
#include "ui/ui.h"

#define WEATHER_LOG_ENABLE 1

#if WEATHER_LOG_ENABLE
#define WEATHER_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define WEATHER_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define WEATHER_LOG(format, ...)
#define WEATHER_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
#define WEATHER_SWITCH_TIME     (10000)    // 10s切换一次今天与预报

#define INFO_FONT_SIZE  (22)
#define INFO_Y_OFFSET   (4)

// weather
#define POS_ICON_X      (0)
#define POS_ICON_Y      (0)
#define POS_TEXT1_X     (10)
#define POS_TEXT1_Y     (140)
#define POS_TEXT2_X     (POS_TEXT1_X)
#define POS_TEXT2_Y     (POS_TEXT1_Y + INFO_FONT_SIZE + INFO_Y_OFFSET)
#define POS_TEXT3_X     (POS_TEXT1_X)
#define POS_TEXT3_Y     (POS_TEXT2_Y + INFO_FONT_SIZE + INFO_Y_OFFSET)

/* Private variables --------------------------------------------------------------------*/
static widget_t widget_weather = {0};

static lv_obj_t *screen_weather = NULL;
static lv_obj_t *img_icon;
static lv_obj_t *label_text1;
static lv_obj_t *label_text2;
static lv_obj_t *label_text3;
// static lv_obj_t *label_text4;
// static lv_obj_t *label_text5;
// static lv_obj_t *label_text6;

/* Extern variables ---------------------------------------------------------------------*/
extern APP* app;
extern const char *forecast_sort_text_list[];

// style
extern lv_style_t style_default;
extern lv_style_t style_time;
extern lv_style_t style_oppo_sans_14;

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/
void widget_weather_process(void);

/**
 * @brief   ui初始化
 */
void weather_ui_init(void)
{
    // 初始化页面
    screen_weather = lv_obj_create(GT_NULL);
    lv_obj_add_style(screen_weather, &style_default, 0);

    // 图标
    img_icon = lv_img_create(screen_weather);
    lv_obj_set_pos(img_icon, POS_ICON_X, POS_ICON_Y);
    lv_img_set_src(img_icon, img_weather_list[0]);
    lv_obj_align(img_icon, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_flag(img_icon, LV_OBJ_FLAG_HIDDEN);

    // 文本1
    label_text1 = lv_label_create(screen_weather);
    lv_obj_add_style(label_text1, &style_oppo_sans_14, 0);
    lv_obj_set_pos(label_text1, POS_TEXT1_X, POS_TEXT1_Y);
    lv_label_set_long_mode(label_text1, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label_text1, " ");

    // 文本2
    label_text2 = lv_label_create(screen_weather);
    lv_obj_add_style(label_text2, &style_oppo_sans_14, 0);
    lv_label_set_long_mode(label_text2, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_pos(label_text2, POS_TEXT2_X, POS_TEXT2_Y);
    lv_label_set_text(label_text2, " ");

    // 文本3
    label_text3 = lv_label_create(screen_weather);
    lv_obj_add_style(label_text3, &style_oppo_sans_14, 0);
    lv_label_set_long_mode(label_text3, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_pos(label_text3, POS_TEXT3_X, POS_TEXT3_Y);
    lv_label_set_text(label_text3, " ");

    // widget_weather_process();

    WEATHER_LOG_NEWLINE("screen_weather");
    lv_scr_load(screen_weather);
}


void weather_update_today(void)
{
    // 更新天气图标
    if (app->weather.today_index >= Code_Sunny && app->weather.today_index <= Code_Max)
    {
        if (lv_obj_has_flag(img_icon, LV_OBJ_FLAG_HIDDEN))
        {
            lv_obj_clear_flag(img_icon, LV_OBJ_FLAG_HIDDEN);
        }
        if (app->rtc.is_night)
        {
            lv_img_set_src(img_icon, img_weather_list[app->weather.today_index * 2 + 1]);
        }
        else
        {
            lv_img_set_src(img_icon, img_weather_list[app->weather.today_index * 2]);
        }
    }

    // 更新天气描述
    lv_label_set_text_fmt(label_text1, "今天 %s", app->weather.today.text.c_str());
    lv_label_set_text_fmt(label_text2, "气温 %d° 湿度 %2d%%", app->weather.today.temp, app->weather.today.humidity);
    lv_label_set_text_fmt(label_text3, "%s %s级",
                          app->weather.today.windDir.c_str(),
                          app->weather.today.windScale.c_str());
}

void weather_update_forecast(void)
{
    static uint8_t day = 0;

    // 更新天气图标
    if (app->weather.forecast_index[day] >= Code_Sunny && app->weather.forecast_index[day] <= Code_Max)
    {
        if (lv_obj_has_flag(img_icon, LV_OBJ_FLAG_HIDDEN))
        {
            lv_obj_clear_flag(img_icon, LV_OBJ_FLAG_HIDDEN);
        }

        lv_img_set_src(img_icon, img_weather_list[app->weather.forecast_index[day] * 2]);
    }

    // 天气描述
    switch (day)
    {
    case 0:
        lv_label_set_text_fmt(label_text1, "明天 %s 晚上 %s\n",
                              app->weather.forecast[day].textDay.c_str(),
                              app->weather.forecast[day].textNight.c_str());
        break;
    case 1:
        lv_label_set_text_fmt(label_text1, "后天 %s 晚上 %s\n",
                              app->weather.forecast[day].textDay.c_str(),
                              app->weather.forecast[day].textNight.c_str());
        break;
    case 2:
        lv_label_set_text_fmt(label_text1, "大后天 %s\n", app->weather.forecast[day].textDay.c_str());
        break;
    default:
        break;
    }

    // 温度
    lv_label_set_text_fmt(label_text2, "气温 %d~%d° 湿度 %d%%",
                          app->weather.forecast[day].tmp_min,
                          app->weather.forecast[day].tmp_min,
                          app->weather.forecast[day].humidity);
    // 风向
    lv_label_set_text_fmt(label_text3, "%s %s级",
                        app->weather.forecast[day].windDir.c_str(),
                        app->weather.forecast[day].windScale.c_str());

    day++;
    if (day >= 3)
    {
        day = 0;
    }
}

/**
 * @brief   运行函数
 */
void widget_weather_process(void)
{
    static uint8_t flag = 0;

    if (flag == 0)
    {
        if (app->weather.today_result == 200)
        {
            weather_update_today();
        }
        else
        {
            lv_label_set_text_fmt(label_text1, "No Data...");
        }
    }
    else
    {
        if (app->weather.forecast_result == 200)
        {
            weather_update_forecast();
        }
        else
        {
            lv_label_set_text_fmt(label_text1, "No Data...");
        }
    }

    flag++;
    if (flag >= 4)
    {
        flag = 0;
    }
}

/**
 * @brief   退出函数
 */
void widget_weather_init(void)
{
    weather_ui_init();
}

/**
 * @brief   退出函数
 */
void widget_weather_exit(void)
{
    lv_obj_del(screen_weather);
}

/**
 * @brief   组件控制
 */
gt_err_t widget_weather_control(uint32_t cmd, void *args)
{
    switch (cmd)
    {
    case KEY_LEFT:
    case KEY_RIGHT:
        break;
    case KEY_BACK:
        app->widget.launcher_control(LAUNCHER_EXIT, NULL);
        break;
    default:
        break;
    }

    return GT_EOK;
}

/**
 * @brief   初始化
 */
void weather_init(void)
{
    widget_weather.id = WIDGET_WEATHER;
    widget_weather.name = "WEATHER";
    widget_weather.icon = &icon_weather;
    widget_weather.init = widget_weather_init;
    widget_weather.process = widget_weather_process;
    widget_weather.exit = widget_weather_exit;
    widget_weather.control = widget_weather_control;
    widget_weather.peroid = WEATHER_SWITCH_TIME; // 10s
    Serial.println("add widget_weather");

    app->widget.add(&widget_weather);
}
