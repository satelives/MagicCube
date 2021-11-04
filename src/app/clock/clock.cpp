/**
 * @file    clock.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-29
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "clock.h"
#include "../weather/weather.h"
#include "app.h"
#include "widget.h"
#include "ui/ui.h"

#define CLOCK_LOG_ENABLE 1

#if CLOCK_LOG_ENABLE
#define CLOCK_LOG(format, ...)          Serial.print(format, ##__VA_ARGS__)
#define CLOCK_LOG_NEWLINE(format, ...)  Serial.println(format, ##__VA_ARGS__)
#else
#define CLOCK_LOG(format, ...)
#define CLOCK_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
typedef enum
{
    CLOCK_STYLE_WEATHER = 0,
    CLOCK_STYLE_ARC,
    CLOCK_STYLE_MAX,
} clock_style_t;
#define WEATHER_SWITCH_TIME     (15) // 1min 切换一次今天与预报

// remain clock
#define POS_HOUR_MIN_X      (95)
#define POS_HOUR_MIN_Y      (70)
#define POS_SEC_X           (90)
#define POS_SEC_Y           (95)

// weather clock
#define INFO_FONT_SIZE (22)
#define INFO_Y_OFFSET (4)

#define POS_DATE_X (0)
#define POS_DATE_Y (20)
#define POS_TIME_X (20)
#define POS_TIME_Y (40)
#define POS_ICON_X (0)
#define POS_ICON_Y (111)
#define POS_TEXT_X (130)
#define POS_TEXT_Y (140)
#define POS_TEMP_X (POS_TEXT_X)
#define POS_TEMP_Y (POS_TEXT_Y + INFO_FONT_SIZE + INFO_Y_OFFSET)
#define POS_HUMI_X (POS_TEXT_X)
#define POS_HUMI_Y (POS_TEMP_Y + INFO_FONT_SIZE + INFO_Y_OFFSET)

/* Private variables --------------------------------------------------------------------*/
static int8_t clock_style = CLOCK_STYLE_ARC;
static widget_t widget_clock = {0};
static uint16_t weather_switch_cnt = 0;

lv_obj_t *screen_clock = GT_NULL;

static lv_obj_t *label_hour_min;
static lv_obj_t *label_sec;
static lv_obj_t *arc_min;
static lv_obj_t *arc_sec;

static lv_obj_t *label_date;
static lv_obj_t *label_time;
static lv_obj_t *label_text1;
static lv_obj_t *label_text2;
static lv_obj_t *label_text3;
static lv_obj_t *img_icon;

static int min_old;

/* Extern variables ---------------------------------------------------------------------*/
extern APP *app;
extern const char *forecast_sort_text_list[];
extern const lv_img_dsc_t *img_weather_list[];

// style
extern lv_style_t style_default;
extern lv_style_t style_time;
extern lv_style_t style_oppo_sans_14;

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   更新时间
 * @param   time            text
 */
void weatherclock_update_time(const char *time)
{
    lv_label_set_text(label_time, time);
}

/**
 * @brief   更新日期
 * @param   data            text
 */
void weatherclock_update_date(const char *data)
{
    lv_label_set_text(label_date, data);
}

/**
 * @brief   更新今天天气
 * @param   index           text
 * @param   today           text
 */
void weatherclock_update_today(void)
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
    lv_label_set_text(label_text1, app->weather.today.text.c_str());
    lv_label_set_text_fmt(label_text2, "%d° | %2d%%", app->weather.today.temp, app->weather.today.humidity);
    lv_label_set_text_fmt(label_text3, "%s %s级",
                          app->weather.today.windDir.c_str(),
                          app->weather.today.windScale.c_str());
}

/**
 * @brief   更新3天天气预报
 * @param   forecast        text
 */
void weatherclock_update_forecast(void)
{
    // 更新天气图标
    if (app->weather.forecast_index[0] >= Code_Sunny && app->weather.forecast_index[0] <= Code_Max)
    {
        if (lv_obj_has_flag(img_icon, LV_OBJ_FLAG_HIDDEN))
        {
            lv_obj_clear_flag(img_icon, LV_OBJ_FLAG_HIDDEN);
        }
        if (app->rtc.is_night)
        {
            lv_img_set_src(img_icon, img_weather_list[app->weather.forecast_index[0] * 2 + 1]);
        }
        else
        {
            lv_img_set_src(img_icon, img_weather_list[app->weather.forecast_index[0] * 2]);
        }
    }

    lv_label_set_text_fmt(label_text1, "%s %d~%d°",
                          forecast_sort_text_list[app->weather.forecast_index[0]],
                          app->weather.forecast[0].tmp_min,
                          app->weather.forecast[0].tmp_max);

    lv_label_set_text_fmt(label_text2, "%s %d~%d°",
                          forecast_sort_text_list[app->weather.forecast_index[1]],
                          app->weather.forecast[1].tmp_min,
                          app->weather.forecast[1].tmp_max);

    lv_label_set_text_fmt(label_text3, "%s %d~%d°",
                          forecast_sort_text_list[app->weather.forecast_index[2]],
                          app->weather.forecast[2].tmp_min,
                          app->weather.forecast[2].tmp_max);
}

/**
 * @brief   运行函数
 */
void widget_clock_process(void)
{
    char str[32];

    // CLOCK_LOG_NEWLINE("clock_process");
    if (clock_style == CLOCK_STYLE_WEATHER)
    {
        strftime(str, 32, "%H:%M:%S", &app->rtc.time);
        weatherclock_update_time(str);
        strftime(str, 32, "%F %a", &app->rtc.time);
        weatherclock_update_date(str);

        if (weather_switch_cnt == 0)
        {
            if (app->weather.today_result == 200)
            {
                weatherclock_update_today();
            }
        }
        else if (weather_switch_cnt == WEATHER_SWITCH_TIME)
        {
            if (app->weather.forecast_result == 200)
            {
                weatherclock_update_forecast();
            }
        }

        if (weather_switch_cnt++ >= (WEATHER_SWITCH_TIME * 2))
        {
            weather_switch_cnt = 0;
        }
    }
    else if (clock_style == CLOCK_STYLE_ARC)
    {
        if (min_old != app->rtc.time.tm_min)
        {
            min_old = app->rtc.time.tm_min;

            lv_arc_set_value(arc_min, (app->rtc.time.tm_hour * 60 + app->rtc.time.tm_min));
            lv_label_set_text_fmt(label_hour_min, "%02d:%02d", (23 - app->rtc.time.tm_hour), (60 - app->rtc.time.tm_min));
        }

        lv_arc_set_value(arc_sec, app->rtc.time.tm_sec);
        lv_label_set_text_fmt(label_sec, "%02d", (60 - app->rtc.time.tm_sec));
    }

}

/**
 * @brief   ui初始化
 */
void clock_ui_init(uint8_t style)
{
    min_old = 0xff;

    // 初始化页面
    if (screen_clock == GT_NULL)
    {
        screen_clock = lv_obj_create(GT_NULL);
        lv_obj_add_style(screen_clock, &style_default, 0);
    }

    if (style == CLOCK_STYLE_ARC)
    {
        // 时分
        label_hour_min = lv_label_create(screen_clock);
        lv_obj_add_style(label_hour_min, &style_default, 0);
        lv_obj_set_pos(label_hour_min, POS_HOUR_MIN_X, POS_HOUR_MIN_Y);
        lv_label_set_text(label_hour_min, "00:00");
        // 秒
        label_sec = lv_label_create(screen_clock);
        lv_obj_add_style(label_sec, &style_time, 0);
        lv_obj_set_pos(label_sec, POS_SEC_X, POS_SEC_Y);
        // lv_obj_set_align(label_sec, LV_ALIGN_CENTER);
        lv_label_set_text(label_sec, "00");

        arc_min = lv_arc_create(screen_clock);
        lv_arc_set_rotation(arc_min, 270);
        lv_arc_set_range(arc_min, 0, 1440);
        lv_arc_set_bg_angles(arc_min, 0, 360);
        lv_arc_set_mode(arc_min, LV_ARC_MODE_REVERSE);
        lv_obj_set_size(arc_min, 190, 190);
        lv_obj_align(arc_min, LV_ALIGN_CENTER, 0, 0);
        lv_obj_remove_style(arc_min, NULL, LV_PART_KNOB);
        lv_obj_clear_flag(arc_min, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_color(arc_min, lv_color_black(), 0);

        arc_sec = lv_arc_create(screen_clock);
        lv_arc_set_rotation(arc_sec, 270);
        lv_arc_set_range(arc_sec, 0, 60);
        lv_arc_set_bg_angles(arc_sec, 0, 360);
        lv_arc_set_mode(arc_sec, LV_ARC_MODE_REVERSE);
        lv_obj_set_size(arc_sec, 230, 230);
        lv_obj_align(arc_sec, LV_ALIGN_CENTER, 0, 0);
        lv_obj_remove_style(arc_sec, NULL, LV_PART_KNOB);
        lv_obj_clear_flag(arc_sec, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_color(arc_sec, lv_color_black(), 0);
    }
    else if (style == CLOCK_STYLE_WEATHER)
    {
        // 日期
        label_date = lv_label_create(screen_clock);
        lv_obj_add_style(label_date, &style_default, 0);
        lv_obj_set_pos(label_date, POS_DATE_X, POS_DATE_Y);
        lv_obj_set_align(label_date, LV_ALIGN_TOP_MID);
        lv_label_set_text(label_date, "0000-00-00 000");
        // 时间
        label_time = lv_label_create(screen_clock);
        lv_obj_add_style(label_time, &style_time, 0);
        lv_obj_set_pos(label_time, POS_TIME_X, POS_TIME_Y);
        lv_label_set_text(label_time, "00:00:00");
        // 图标
        img_icon = lv_img_create(screen_clock);
        lv_obj_set_pos(img_icon, POS_ICON_X, POS_ICON_Y);
        lv_img_set_src(img_icon, img_weather_list[0]);
        lv_obj_add_flag(img_icon, LV_OBJ_FLAG_HIDDEN);
        // 天气文字
        label_text1 = lv_label_create(screen_clock);
        lv_obj_add_style(label_text1, &style_oppo_sans_14, 0);
        lv_obj_set_pos(label_text1, POS_TEXT_X, POS_TEXT_Y);
        lv_label_set_text(label_text1, " ");
        // 温度, 湿度
        label_text2 = lv_label_create(screen_clock);
        lv_obj_add_style(label_text2, &style_oppo_sans_14, 0);
        lv_obj_set_pos(label_text2, POS_TEMP_X, POS_TEMP_Y);
        lv_label_set_text(label_text2, " ");
        // 预报
        label_text3 = lv_label_create(screen_clock);
        lv_obj_add_style(label_text3, &style_oppo_sans_14, 0);
        lv_obj_set_pos(label_text3, POS_HUMI_X, POS_HUMI_Y);
        lv_label_set_text(label_text3, " ");
    }

    if (app->rtc.is_vaild)
    {
        widget_clock_process();
    }
    weather_switch_cnt = 0;

    CLOCK_LOG_NEWLINE("screen_clock");
    lv_scr_load(screen_clock);
}

/**
 * @brief   ui反初始化
 */
void clock_ui_deinit(uint8_t style)
{
    lv_obj_del(screen_clock);
    screen_clock = GT_NULL;
}

/**
 * @brief   进入
 */
void widget_clock_init(void)
{
    clock_style = CLOCK_STYLE_WEATHER;
    clock_ui_init(clock_style);
}

/**
 * @brief   退出
 */
void widget_clock_exit(void)
{
    clock_ui_deinit(clock_style);
}

/**
 * @brief   控制函数
 */
gt_err_t widget_clock_control(uint32_t cmd, void *args)
{
    switch (cmd)
    {
    case KEY_RIGHT:
        clock_ui_deinit(clock_style);
        clock_style++;
        if (clock_style == CLOCK_STYLE_MAX)
        {
            clock_style = CLOCK_STYLE_WEATHER;
        }

        clock_ui_init(clock_style);
        if (screen_clock != GT_NULL)
        {
            lv_scr_load(screen_clock);
        }

        Serial.println(clock_style);
        break;
    case KEY_LEFT:
        clock_ui_deinit(clock_style);
        if (clock_style > CLOCK_STYLE_WEATHER)
        {
            clock_style--;
        }
        else
        {
            clock_style = CLOCK_STYLE_MAX - 1;
        }
        clock_ui_init(clock_style);

        if (screen_clock != GT_NULL)
        {
            lv_scr_load(screen_clock);
        }
        Serial.println(clock_style);
        break;
    case KEY_BACK:
        app->widget.launcher_control(LAUNCHER_EXIT, NULL);
        break;

    default:
        break;
    }

    return true;
}

/**
 * @brief   初始化
 */
void clock_init(void)
{
    // clock_style = CLOCK_STYLE_WEATHER;
    // clock_ui_init(clock_style);

    widget_clock.id = WIDGET_CLOCK;
    widget_clock.name = "CLOCK";
    widget_clock.icon = &icon_clock;
    widget_clock.init = widget_clock_init;
    widget_clock.process = widget_clock_process;
    widget_clock.exit = widget_clock_exit;
    widget_clock.control = widget_clock_control;
    widget_clock.peroid = 1000; // 1S

    Serial.println("add widget_clock");

    app->widget.add(&widget_clock);
}
