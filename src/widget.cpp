/**
 * @file    app.c
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-28
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "widget.h"
#include "task.h"
#include "ui/ui.h"

#define WIDGET_LOG_ENABLE 1

#if WIDGET_LOG_ENABLE
#define WIDGET_LOG(format, ...)         Serial.print(format, ##__VA_ARGS__)
#define WIDGET_LOG_NEWLINE(format, ...) Serial.println(format, ##__VA_ARGS__)
#else
#define WIDGET_LOG(format, ...)
#define WIDGET_LOG_NEWLINE(format, ...)
#endif

/* Exported constants -------------------------------------------------------------------*/
#define WIDGET_TASK_PID (3)

/* Private variables --------------------------------------------------------------------*/
static widget_t widget_weather = {0};

lv_obj_t *screen_launcher = NULL;
lv_obj_t *img_icon = NULL;
lv_obj_t *label_titel = NULL;

// style
extern lv_style_t style_default;
extern lv_style_t style_time;
extern lv_style_t style_oppo_sans_14;

/* Extern variables ---------------------------------------------------------------------*/
/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   Construct a new WIDGET::WIDGET object
 */
WIDGET::WIDGET()
{
}

/**
 * @brief   Destroy the WIDGET::WIDGET object
 */
WIDGET::~WIDGET()
{
}

/**
 * @brief   查找 widget
 * @param   id              text
 * @param   widget          text
 * @return  true
 * @return  false
 */
bool WIDGET::find_widget(uint8_t id, widget_t **widget)
{
    widget_t* node;

    if (widgets == GT_NULL)
    {
        return false;
    }

    node = widgets;
    while (node)
    {
        WIDGET_LOG_NEWLINE("find widget:" + String(node->id) + "," + node->name);
        if (node->id == id)
        {
            *widget = node;
            WIDGET_LOG_NEWLINE("widget found");
            return true;
        }
        node = node->next;
    }

    return false;
}

/**
 * @brief   当没有 widget 运行时，切换到此函数
 */
void WIDGET::default_task(void)
{

}

/**
 * @brief   当前是否有widget在运行
 * @return  true
 * @return  false
 */
bool WIDGET::is_widget_running(void)
{
    return (widget_running == GT_NULL ? false : true);
}

/**
 * @brief   添加 widget 组件
 * @param   widget          text
 * @return  true
 * @return  false
 */
gt_err_t WIDGET::add(widget_t *widget)
{
    widget_t *node;

    if (widget == GT_NULL)
    {
        return GT_ERROR;
    }

    if (widgets == GT_NULL)
    {
        widgets = widget;
        widgets->prev = GT_NULL;
        widgets->next = GT_NULL;
        WIDGET_LOG_NEWLINE("widget added:" + widget->name);
        return GT_EOK;
    }
    else
    {
        node = widgets;
        while (node)
        {
            if (node->next == GT_NULL)
            {
                node->next = widget;
                widget->prev = node;
                widget->next = GT_NULL;
                WIDGET_LOG_NEWLINE("widget added:" + widget->name);
                return GT_EOK;
            }
            node = node->next;
        }
    }

    WIDGET_LOG_NEWLINE("widget add NG" + widget->name);

    return GT_ERROR;
}

/**
 * @brief   添加 widget 组件
 * @param   widget          text
 * @return  true
 * @return  false
 */
gt_err_t WIDGET::remove(widget_id_t id)
{
    widget_t *widget = GT_NULL;

    if (find_widget(id, &widget))
    {
        WIDGET_LOG_NEWLINE("widget remove:" + widget->name);
        // 有上一个节点
        if (widget->prev != GT_NULL)
        {
            // 有下一个节点
            if (widget->next != GT_NULL)
            {
                widget->next->prev = widget->prev;
                widget->prev->next = widget->next;
            }
            // 无下一个节点
            else
            {
                widget->prev->next = GT_NULL;
            }
        }
        // 无上一个节点
        else
        {
            // 有下一个节点
            if (widget->next != GT_NULL)
            {
                widget->next->prev = GT_NULL;
            }
            else
            {
                // 无上一个节点也无下一个节点
                return GT_ERROR;
            }
        }
        WIDGET_LOG_NEWLINE("widget remove ok:" + widget->name);
        return GT_EOK;
    }


    WIDGET_LOG_NEWLINE("widget remove fail:" + widget->name);

    return GT_ERROR;
}

/**
 * @brief   启动指定的 widget
 * @param   id              text
 * @return  true
 * @return  false
 */
gt_err_t WIDGET::launch(widget_t *widget)
{
    if (widget != GT_NULL)
    {
        // 如果上个程序尚未退出，执行退出
        if (widget_running != GT_NULL)
        {
            if (widget_running->exit != GT_NULL)
            {
                WIDGET_LOG_NEWLINE("exit:" + widget_running->name);
                widget_running->exit();
            }
        }

        launcher_widget = widget;
        widget_running = widget;

        // 加载新调度周期
        task_set_period(WIDGET_TASK_PID, widget_running->peroid);
        // 加载新回调
        if (task_set_entry(WIDGET_TASK_PID, widget_running->process) == GT_EOK)
        {
            // WIDGET_LOG_NEWLINE("ok" + String((uint32_t)widget_running->process));
        }

        // 初始化
        if (widget_running->init != GT_NULL)
        {
            WIDGET_LOG_NEWLINE("init:" + widget_running->name);
            widget_running->init();
        }

        // // 加载界面
        // WIDGET_LOG_NEWLINE("load");
        // if (widget_running->ui != GT_NULL)
        // {
        //     lv_scr_load(widget_running->ui);
        // }
        // else
        // {
        //     WIDGET_LOG_NEWLINE("no lv_scr_load");
        // }

        return GT_EOK;
    }

    return GT_ERROR;
}

/**
 * @brief   启动指定的 widget
 * @param   id              text
 * @return  true
 * @return  false
 */
gt_err_t WIDGET::launch(widget_id_t id)
{
    widget_t *widget = GT_NULL;

    if (id >= WIDGET_MAX)
    {
        return GT_ERROR;
    }

    if (find_widget(id, &widget))
    {
        return launch(widget);
    }

    WIDGET_LOG_NEWLINE("not found");
    return GT_ERROR;
}

/**
 * @brief   停止运行指定的 widget
 * @param   id              text
 * @return  true
 * @return  false
 */
gt_err_t WIDGET::exit(widget_t *widget)
{
    if (widget != GT_NULL)
    {
        if (widget != GT_NULL)
        {
            if (widget->exit != GT_NULL)
            {
                WIDGET_LOG_NEWLINE("exit:" + widget->name);
                widget->exit();
            }
        }
        launcher_widget = widget_running;
        widget_running = GT_NULL;
        task_set_period(WIDGET_TASK_PID, 1000); // 1000ms
        task_set_entry(WIDGET_TASK_PID, GT_NULL);

        launcher_switch_widget(launcher_widget);
        return GT_EOK;
    }

    return GT_ERROR;
}

/**
 * @brief   停止运行指定的 widget
 * @param   id              text
 * @return  true
 * @return  false
 */
gt_err_t WIDGET::exit(widget_id_t id)
{
    widget_t *widget = GT_NULL;

    if (find_widget(id, &widget))
    {
        return exit(widget);
    }

    return GT_ERROR;
}

/**
 * @brief   控制函数
 * @param   cmd             text
 * @param   args            text
 * @return  gt_err_t
 */
gt_err_t WIDGET::control(uint32_t cmd, void* args)
{
    if (widget_running != GT_NULL)
    {
        if (widget_running->control != GT_NULL)
        {
            return widget_running->control(cmd, args);
        }
    }

    return GT_ERROR;
}

/**
 * @brief   获取当前运行的 widget id
 * @return  int
 */
int WIDGET::get_current_widget_id(void)
{
    if (widget_running != NULL)
    {
        return widget_running->id;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief   launcher 显示图标
 * @param   widget          text
 */
void WIDGET::launcher_switch_widget(widget_t *widget)
{
    if (widget->icon != GT_NULL)
    {
        lv_img_set_src(img_icon, widget->icon);
        lv_obj_set_align(img_icon, LV_ALIGN_CENTER);
    }
    else
    {
        lv_img_set_src(img_icon, &image_sunny);
        lv_obj_set_align(img_icon, LV_ALIGN_CENTER);
    }

    if (!widget->name.isEmpty())
    {
        lv_label_set_text(label_titel, widget->name.c_str());
    }

    // lv_scr_load_anim(screen_launcher, LV_SCR_LOAD_ANIM_NONE, 300, 300, false);
    lv_scr_load(screen_launcher);
}


void WIDGET::launcher_control(uint32_t cmd, void* args)
{
    if (launcher_widget == GT_NULL)
    {
        if (widgets != GT_NULL)
        {
            launcher_widget = widgets;
        }
        else
        {
            return;
        }
    }

    switch (cmd)
    {
    // 下一个
    case LAUNCHER_NEXT:
        if (launcher_widget->next != GT_NULL)
        {
            launcher_widget = launcher_widget->next;
            WIDGET_LOG_NEWLINE("widget next:" + launcher_widget->name);
            launcher_switch_widget(launcher_widget);
        }
        // 返回第一个
        else if (widgets != GT_NULL)
        {
            launcher_widget = widgets;
            WIDGET_LOG_NEWLINE("widget reset:" + launcher_widget->name);
            launcher_switch_widget(launcher_widget);
        }

        break;
    // 上一个
    case LAUNCHER_PREV:
        if (launcher_widget->prev != GT_NULL)
        {
            launcher_widget = launcher_widget->prev;
            WIDGET_LOG_NEWLINE("widget prev:" + launcher_widget->name);
            launcher_switch_widget(launcher_widget);
        }
        else
        {
            // 返回最后一个
            while (launcher_widget->next)
            {
                launcher_widget = launcher_widget->next;
            }
            WIDGET_LOG_NEWLINE("widget reset:" + launcher_widget->name);
            launcher_switch_widget(launcher_widget);
        }
        break;
    // 进入
    case LAUNCHER_ENTER:
        WIDGET_LOG_NEWLINE("widget enter:" + launcher_widget->name);
        launch(launcher_widget);
        break;
    // 退出
    case LAUNCHER_EXIT:
        WIDGET_LOG_NEWLINE("widget exit:" + launcher_widget->name);
        exit(widget_running);
        break;
    // 默认显示第一个
    case LAUNCHER_INIT:
    default:
        if (widgets != GT_NULL)
        {
            launcher_widget = widgets;

            WIDGET_LOG_NEWLINE("widget init:" + launcher_widget->name);
            launcher_switch_widget(launcher_widget);
        }
        break;
    }
}

/**
 * @brief   launcher init
 */
void WIDGET::launcher_init(void)
{
    screen_launcher = lv_obj_create(NULL);

    // 初始化页面
    lv_obj_add_style(screen_launcher, &style_default, 0);

    img_icon = lv_img_create(screen_launcher);
    lv_obj_align(img_icon, LV_ALIGN_CENTER, 0, -20);

    label_titel = lv_label_create(screen_launcher);
    lv_obj_add_style(label_titel, &style_default, 0);
    // lv_obj_set_pos(label_titel, POS_DATE_X, POS_DATE_Y);
    lv_obj_align(label_titel, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_label_set_text(label_titel, " ");

    widget_running = GT_NULL;
}
