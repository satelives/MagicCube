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

#include "display.h"
#include "TFT_eSPI.h"
#include "lvgl.h"
#include "../ui/ui.h"

/* Private typedef ----------------------------------------------------------------------*/
static const int screenWidth = 240;
static const int screenHeight = 240;

static TFT_eSPI tft = TFT_eSPI();
static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

/* Extern variables ---------------------------------------------------------------------*/
/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

void lv_debug(const char* dsc)
{
    Serial.printf("%s\r\n", dsc);
    Serial.flush();
}

void tft_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/**
 * @brief   初始化
 */
void Display::init()
{
    // ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
    // ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);

    Serial.println("display init...");

    // ---------- init ftf
    tft.begin();
    // tft.setRotation(0);
    tft.setRotation(4); /* mirror */

    // TFT_eSprite spr = TFT_eSprite(&tft);
    // spr.createSprite(TFT_WIDTH, TFT_HEIGHT);
    tft.fillScreen(TFT_BLACK);
    // tft.setRotation(4);
    // tft.setCursor(10, 10); //设置行号
    // // tft.setTextColor(TFT_GREEN); //设置字体颜色
    // tft.setTextSize(4); //设置字体大小(现在是3号，默认1号)
    // tft.print("Seven"); //显示USB
    // tft.setTextSize(5);
    // tft.setTextColor(TFT_GREEN, TFT_BLACK);

    // ---------- init lvgl
    lv_init();
    // lv_log_register_print_cb(lv_debug);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*Initialize the display*/


    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = tft_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // /*Initialize the (dummy) input device driver*/
    // // static lv_indev_drv_t indev_drv;
    // // lv_indev_drv_init(&indev_drv);
    // // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // // indev_drv.read_cb = my_touchpad_read;
    // // lv_indev_drv_register(&indev_drv);

    // 放置仪表盘控件
    // lv_obj_t *label = lv_label_create(lv_scr_act());
    // lv_label_set_text(label, "Hello Arduino! (V8.0.X)");
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    style_init();
}

void Display::process()
{
    lv_task_handler();
}

void Display::set_rotation(uint16_t rotation)
{
    // if (rotation == 0)
    // {
    //     tft.setRotation(4);
    // }
    // else if (rotation == 180)
    // {
    //     lv_disp_set_rotation(NULL, LV_DISP_ROT_90);
    // }
}

// void Display::setBackLight(float duty)
// {
// 	duty = constrain(duty, 0, 1);
// 	duty = 1 - duty;
// 	ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
// }
