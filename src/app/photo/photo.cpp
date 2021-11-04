/**
 * @file    photo.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-11-01
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "photo.h"
#include "app.h"
#include "widget.h"
#include "ui/ui.h"

#define PHOTO_LOG_ENABLE  1

#if PHOTO_LOG_ENABLE
#define PHOTO_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define PHOTO_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define PHOTO_LOG(format, ...)
#define PHOTO_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
#define AUTO_NEXT_PICTURE_CNT   (30)    // 轮播间隔

#define POS_PHOTO_X             (56)
#define POS_PHOTO_Y             (10)

#define POS_TEXT_X              (84)
#define POS_TEXT_Y              (200)

/* Extern variables ---------------------------------------------------------------------*/
extern APP *app;
extern lv_style_t style_default;
extern lv_style_t style_oppo_sans_14;

extern const  lv_img_dsc_t* img_photo_list[];

/* Private variables --------------------------------------------------------------------*/
static widget_t widget_photo = {0};
static lv_obj_t *screen_photo = GT_NULL;
static lv_obj_t *img_photo = GT_NULL;
static lv_obj_t *label_txt = GT_NULL;
static uint8_t auto_next_picture_cnt = 0;

static const char* text_list[] =
{
    "余生路上有你真好",
    "这是啾啾和大熊呀",
    "有我在 不用怕",
    "静守时光 以待流年",
    "你是我今生的新娘",
    "麻雀虽小 大熊的宝",
    "奶奶 爷爷把小台灯做出来啦",
    "好好工作 别TM走神",
    "啾啾在干啥呢",
    "啾啾在吃啥呢",
    "下次睡觉别把腿放我身上",
    "来 让大熊抱抱",
    "大熊最宠啾啾啦",
    "大熊说今晚给啾啾做饭",
    "大熊给啾啾遮风挡雨",
    "啾啾要给大熊做饭",
    "老婆 今晚咱吃啥?",
    "老婆 笑一个",
    "我是牛粪你是花",
    "老婆想我了没",
    "今天不许摸熊屁股",
};

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   photo 切换
 */
void widget_photo_process(void)
{
    if (auto_next_picture_cnt >= AUTO_NEXT_PICTURE_CNT)
    {
        auto_next_picture_cnt = 0;

        uint8_t index;
        index = (uint8_t)random(0, 13);

        lv_img_set_src(img_photo, img_photo_list[index]);

        index = (uint8_t)random(0, ARRAY_SIZE(text_list) - 1);
        lv_label_set_text(label_txt, text_list[index]);
    }
    auto_next_picture_cnt++;
}

/**
 * @brief   初始化
 */
void widget_photo_init(void)
{
    // 初始化页面
    screen_photo = lv_obj_create(NULL);
    lv_obj_add_style(screen_photo, &style_default, 0);

    // 图片
    img_photo = lv_img_create(screen_photo);
    lv_obj_align(img_photo, LV_ALIGN_TOP_MID, 0, 5);
    // lv_img_set_src(img_photo, img_photo_list[0]);

    // 文字
    label_txt = lv_label_create(screen_photo);
    lv_obj_add_style(label_txt, &style_oppo_sans_14, 0);
    // lv_obj_set_pos(label_txt, POS_VERSION_X, POS_VERSION_Y);
    lv_obj_align(label_txt, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_label_set_long_mode(label_txt, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label_txt, " ");

    auto_next_picture_cnt = AUTO_NEXT_PICTURE_CNT;
    widget_photo_process();
    lv_scr_load(screen_photo);
}

/**
 * @brief   photo 控制
 * @param   cmd             text
 * @param   args            text
 */
gt_err_t widget_photo_control(uint32_t cmd, void *args)
{
    switch (cmd)
    {
    case KEY_LEFT:
    case KEY_RIGHT:
        auto_next_picture_cnt = AUTO_NEXT_PICTURE_CNT;
        widget_photo_process();
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
 * @brief   photo 退出
 */
void widget_photo_exit(void)
{
    lv_obj_del(screen_photo);
}

/**
 * @brief   初始化
 */
void photo_init(void)
{
    widget_photo.id = WIDGET_PHOTO;
    widget_photo.name = "SWEET HEART";
    widget_photo.icon = &icon_photo;
    widget_photo.init = widget_photo_init;
    widget_photo.process = widget_photo_process;
    widget_photo.exit = widget_photo_exit;
    widget_photo.control = widget_photo_control;
    widget_photo.peroid = 1000; // 1S

    Serial.println("add widget_photo");

    app->widget.add(&widget_photo);
}
