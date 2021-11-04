/**
 * @file    inspire.cpp
 * @brief   倒计时组件
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-11-02
 *
 * (C) COPYRIGHT 2021 chery.zhang
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "inspire.h"
#include "app.h"
#include "widget.h"
#include "ui/ui.h"

#define INSPIRE_LOG_ENABLE  1

#if INSPIRE_LOG_ENABLE
#define INSPIRE_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define INSPIRE_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define INSPIRE_LOG(format, ...)
#define INSPIRE_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
#define POS_MIN_X           (108)
#define POS_MIN_Y           (70)
#define POS_SEC_X           (90)
#define POS_SEC_Y           (95)

/* Extern variables ---------------------------------------------------------------------*/
extern APP *app;
extern lv_style_t style_default;
extern lv_style_t style_oppo_sans_14;

/* Private variables --------------------------------------------------------------------*/
static widget_t widget_inspire = {GT_NULL};

static lv_obj_t *screen_inspire = GT_NULL;
static lv_obj_t *img_timeout = GT_NULL;
static lv_obj_t *arc_sec = GT_NULL;
static lv_obj_t *label_sec = GT_NULL;
static lv_obj_t *label_min = GT_NULL;

static bool     running = false;
static uint16_t time_min = 0;
static uint16_t time_sec = 60;
static uint16_t total_sec = 60;

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/


/**
 * @brief   组件处理函数
 */
void widget_inspire_process(void)
{
    if (running)
    {
        if (time_sec)
        {
            int16_t percentage;
            time_sec--;

            if (time_min == 0 && time_sec == 0)
            {
                percentage = 1000;
            }
            else
            {
                percentage = 1000 - (uint16_t)(((float)(time_min * 60 + time_sec) / total_sec) * 1000);
            }

            // INSPIRE_LOG_NEWLINE(percentage);
            lv_arc_set_value(arc_sec, percentage);
            lv_label_set_text_fmt(label_sec, "%02d", time_sec);
        }
        else
        {
            if (time_min)
            {
                time_min--;
                time_sec = 59;
                int16_t percentage = 1000 - (uint16_t)(((float)(time_min * 60 + time_sec) / total_sec) * 1000);

                lv_arc_set_value(arc_sec, percentage);
                lv_label_set_text_fmt(label_min, "%02d", time_min);
                lv_label_set_text_fmt(label_sec, "%02d", time_sec);
            }
            else
            {
                lv_obj_add_flag(arc_sec, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(label_min, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(label_sec, LV_OBJ_FLAG_HIDDEN);

                lv_obj_clear_flag(img_timeout, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
    else
    {

    }

}

/**
 * @brief   组件初始化
 */
void widget_inspire_init(void)
{
    running = false;
    time_min = 1;
    time_sec = 0;

    screen_inspire = lv_obj_create(NULL);
    lv_obj_add_style(screen_inspire, &style_default, 0);

    img_timeout = lv_img_create(screen_inspire);
    // lv_obj_set_pos(img_timeout, 0, 0);
    lv_img_set_src(img_timeout, &image_timeout);
    lv_obj_align(img_timeout, LV_ALIGN_CENTER, 0, 0);

    // 倒计时环
    arc_sec = lv_arc_create(screen_inspire);
    lv_arc_set_rotation(arc_sec, 270);
    lv_arc_set_range(arc_sec, 0, 1000);   // 100%
    lv_arc_set_bg_angles(arc_sec, 0, 360);
    lv_arc_set_mode(arc_sec, LV_ARC_MODE_REVERSE);
    lv_obj_set_size(arc_sec, 190, 190);
    lv_obj_align(arc_sec, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_style(arc_sec, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc_sec, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_arc_color(arc_sec, lv_color_black(), 0);

    // 剩余分钟数
    label_min = lv_label_create(screen_inspire);
    lv_obj_add_style(label_min, &style_default, 0);
    lv_obj_set_pos(label_min, POS_MIN_X, POS_MIN_Y);
    lv_label_set_text(label_min, "00");

    // 剩余秒数
    label_sec = lv_label_create(screen_inspire);
    lv_obj_add_style(label_sec, &style_time, 0);
    lv_obj_set_pos(label_sec, POS_SEC_X, POS_SEC_Y);
    // lv_obj_set_align(label_sec, LV_ALIGN_CENTER);
    lv_label_set_text(label_sec, "01");

    lv_obj_add_flag(img_timeout, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(label_min, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(arc_sec, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(label_sec, LV_OBJ_FLAG_HIDDEN);

    lv_scr_load(screen_inspire);
}

/**
 * @brief   组件退出
 */
void widget_inspire_exit(void)
{
    lv_obj_del(screen_inspire);
}

/**
 * @brief   组件控制
 */
gt_err_t widget_inspire_control(uint32_t cmd, void *args)
{
    switch (cmd)
    {
    case KEY_LEFT:
        if (!running)
        {
            if (time_min > 1)
            {
                time_min--;
                lv_label_set_text_fmt(label_sec, "%02d", time_min);
            }
        }
        break;
    case KEY_RIGHT:
        if (!running)
        {
            time_min++;
            lv_label_set_text_fmt(label_sec, "%02d", time_min);
        }
        break;
    case KEY_FRONT:
        if (!running)
        {
            running = true;
            time_min--;
            time_sec = 60;
            total_sec = (time_min) * 60 + 60;
            lv_label_set_text(label_sec, "60");
            lv_label_set_text_fmt(label_min, "%02d", time_min);

            lv_obj_add_flag(img_timeout, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(arc_sec, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(label_min, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(label_sec, LV_OBJ_FLAG_HIDDEN);
        }
        break;
    case KEY_BACK:
        if (running)
        {
            running = false;
        }
        else
        {
            app->widget.launcher_control(LAUNCHER_EXIT, NULL);
        }
        break;
    case INSPIRE_START:
        {
            running = true;
            total_sec = 60;
            time_min = 0;
            time_sec = 60;
            lv_label_set_text(label_sec, "60");
            lv_label_set_text_fmt(label_min, "%02d", time_min);

            lv_obj_add_flag(img_timeout, LV_OBJ_FLAG_HIDDEN);

            lv_obj_clear_flag(arc_sec, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(label_min, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(label_sec, LV_OBJ_FLAG_HIDDEN);
        }
        break;
    default:
        break;
    }

    return GT_EOK;
}


/**
 * @brief   初始化
 */
void inspire_init(void)
{
    widget_inspire.id = WIDGET_INSPIRE;
    widget_inspire.name = "INSPIRE";
    widget_inspire.icon = &icon_inspire;
    widget_inspire.init = widget_inspire_init;
    widget_inspire.process = widget_inspire_process;
    widget_inspire.exit = widget_inspire_exit;
    widget_inspire.control = widget_inspire_control;
    widget_inspire.peroid = 1000;     // 1S

    app->widget.add(&widget_inspire);
}