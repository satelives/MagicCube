/**
 * @file    task.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-28
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/
#ifndef __TASK_H
#define __TASK_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Exported constants -------------------------------------------------------------------*/
#define TASK_NAME_MAX   (8)

/* Exported types -----------------------------------------------------------------------*/
typedef void (*task_entry_t)(void);

typedef struct task_desc_t
{
    uint32_t pid;
    const char* name;
    uint32_t period;
    uint32_t ticks;
    task_entry_t entry;
    struct task_desc_t* next;
}task_t;

#define TASK_DECLARE(_name_, _entry_, _period_) \
task_t _name_ = {0, #_name_, _period_, GT_NULL, _entry_, GT_NULL}

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

gt_err_t task_creat(uint32_t pid, task_t *new_task);
gt_err_t task_callback_register(task_entry_t entry);    // 每次都会执行
gt_err_t task_set_period(uint32_t pid, uint32_t period);
gt_err_t task_set_entry(uint32_t pid, task_entry_t entry);
void task_scheduler_start(void);

#ifdef __cplusplus
}
#endif

#endif
