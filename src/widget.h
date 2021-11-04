/**
 * @file    app.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-28
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/
#ifndef __WIDGET_H
#define __WIDGET_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include "lvgl.h"

/* Exported constants -------------------------------------------------------------------*/
#define LAUNCHER_INIT   (0)
#define LAUNCHER_NEXT   (1)
#define LAUNCHER_PREV   (2)
#define LAUNCHER_ENTER  (3)
#define LAUNCHER_EXIT   (4)

/* Exported types -----------------------------------------------------------------------*/
typedef enum widget_id_desc
{
    WIDGET_CLOCK = 0,
    WIDGET_WEATHER,
    WIDGET_PHOTO,
    WIDGET_INSPIRE,
    WIDGET_SETTING,
    WIDGET_MAX,
} widget_id_t;

typedef void (*widget_fun_t)(void);
typedef gt_err_t (*widget_contrl_t)(uint32_t cmd, void *args);
typedef struct widget_desc_t
{
    widget_desc_t *prev;        // prev
    widget_desc_t *next;        // next

    uint8_t id;                 // id
    String name;                // 名称
    const lv_img_dsc_t *icon;   // icon图标
    widget_fun_t init;          // 初始化函数
    widget_fun_t process;       // 运行中函数
    widget_fun_t exit;          // 退出函数
    widget_contrl_t control;    // 控制函数
    uint32_t peroid;            // 运行周期
    uint32_t args;              // 附带参数
} widget_t;

class WIDGET
{
private:
    widget_t *launcher_widget;
    widget_t *widgets = GT_NULL;            //widget 列表
    widget_t *widget_running;               //当前运行的widget
    bool find_widget(uint8_t id, widget_t **widget);
    void default_task(void);
    void launcher_switch_widget(widget_t *widget);

public:
    WIDGET();
    ~WIDGET();

    bool is_widget_running(void);
    gt_err_t add(widget_t *widget);
    gt_err_t remove(widget_id_t id);
    gt_err_t launch(widget_id_t id);
    gt_err_t launch(widget_t *widget);
    gt_err_t exit(widget_id_t id);
    gt_err_t exit(widget_t *widget);
    gt_err_t control(uint32_t cmd, void* args);
    int  get_current_widget_id(void);
    void launcher_init(void);
    void launcher_control(uint32_t cmd, void *args);
};

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/
void page_startup_init(void);

#endif
