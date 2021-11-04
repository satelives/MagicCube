/**
 * @file    task.c
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-28
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "task.h"

/* Exported constants -------------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------------------*/
// task_t root_task = {0};
task_t *task_list = GT_NULL;
task_entry_t task_cb = GT_NULL;

/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   task创建
 * @param   new_task        task指针
 * @return  gt_err_t
 */
gt_err_t task_creat(uint32_t pid, task_t *new_task)
{
    task_t* node;

    // if((new_task == GT_NULL) || (new_task->entry == GT_NULL))
    if(new_task == GT_NULL)
    {
        return GT_ERROR;
    }

    if (task_list == GT_NULL)
    {
        task_list = new_task;
        task_list->pid = pid;
        task_list->next = GT_NULL;
    }
    else
    {
        node = task_list;
        while(node)
        {
            if(node->next == GT_NULL)
            {
                node->next = new_task;
                new_task->pid = pid;
                new_task->next = GT_NULL;
                break;
            }
            node = node->next;
        }
    }

    return GT_EOK;
}

/**
 * @brief   task 回调注册
 * @param   entry           函数入口
 * @return  gt_err_t
 */
gt_err_t task_callback_register(task_entry_t entry)
{
    if (entry)
    {
        task_cb = entry;
        return GT_EOK;
    }

    return GT_EINVAL;
}

/**
 * @brief   修改运行周期
 * @param   pid             text
 * @param   period          text
 * @return  gt_err_t
 */
gt_err_t task_set_period(uint32_t pid, uint32_t period)
{
    task_t *node;

    node = task_list;
    while (node)
    {
        if (node->pid == pid)
        {
            node->period = period;
            node->ticks = millis();
            return GT_EOK;
        }
        node = node->next;
    }

    return GT_EINVAL;
}

/**
 * @brief   修改运行周期
 * @param   pid             text
 * @param   period          text
 * @return  gt_err_t
 */
gt_err_t task_set_entry(uint32_t pid, task_entry_t entry)
{
    task_t *node;

    node = task_list;
    while (node)
    {
        if (node->pid == pid)
        {
            node->entry = entry;
            node->ticks = millis();
            return GT_EOK;
        }
        node = node->next;
    }

    return GT_EINVAL;
}

/**
 * @brief   task 回调释放
 */
void task_callback_free(void)
{
    task_cb = GT_NULL;
}

/**
 * @brief   task 调度
 */
void task_scheduler_start(void)
{
    task_t* task;
    uint32_t ticks;

    task = task_list;
    while(1)
    {
        ticks = millis();

        if(ticks >= task->ticks)
        {
            task->ticks = ticks + task->period;
            if (task->entry)
            {
                task->entry();
            }
        }

        if(task->next)
        {
            task = task->next;
        }
        else
        {
            task = task_list;
        }

        if (task_cb)
        {
            task_cb();
        }
    }
}
