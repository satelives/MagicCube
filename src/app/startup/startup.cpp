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
#include "startup.h"
#include "app.h"
#include "widget.h"
#include "ui/ui.h"

#define STARTUP_LOG_ENABLE  1

#if STARTUP_LOG_ENABLE
#define STARTUP_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define STARTUP_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define STARTUP_LOG(format, ...)
#define STARTUP_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/

#define POS_LOGO_X          (56)
#define POS_LOGO_Y          (10)

#define POS_TEXT_OFFSET_Y   (24)
#define POS_VERSION_X       (84)
#define POS_VERSION_Y       (200)
#define POS_STATUE_X        (88)
#define POS_STATUE_Y        (POS_VERSION_Y + POS_TEXT_OFFSET_Y)

/* Extern variables ---------------------------------------------------------------------*/
extern APP *app;
extern lv_style_t style_default;
extern lv_style_t style_oppo_sans_14;

/* Private variables --------------------------------------------------------------------*/
static lv_obj_t *screen_startup = GT_NULL;
static lv_obj_t *img_logo = GT_NULL;
static lv_obj_t *label_version = GT_NULL;

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   初始化
 */
void show_startup(bool enable)
{
    if (enable)
    {
        // 初始化页面
        screen_startup = lv_obj_create(NULL);
        lv_obj_add_style(screen_startup, &style_default, 0);

        // 图标
        img_logo = lv_img_create(screen_startup);
        lv_obj_set_pos(img_logo, POS_LOGO_X, POS_LOGO_Y);
        lv_obj_align(img_logo, LV_ALIGN_TOP_MID, 0, 10);
        lv_img_set_src(img_logo, &image_startup);

        label_version = lv_label_create(screen_startup);
        lv_obj_add_style(label_version, &style_oppo_sans_14, 0);
        lv_obj_set_pos(label_version, POS_VERSION_X, POS_VERSION_Y);
        lv_obj_align(label_version, LV_ALIGN_BOTTOM_MID, 0, -10);
        lv_label_set_text(label_version, "啾啾小熊的小魔方");

        lv_scr_load(screen_startup);
        lv_task_handler();
    }
    else
    {
        if (screen_startup != GT_NULL)
        {
            lv_obj_del(screen_startup);
            screen_startup = GT_NULL;
        }
    }
}
