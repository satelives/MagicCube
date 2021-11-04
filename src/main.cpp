/**
 * @file    main.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-21
 *
 * (C) COPYRIGHT 2021 FTT
 * // "${workspaceFolder}**",
 * // http://api2.jirengu.com/getWeather.php?city=%E5%8D%97%E5%B1%B1
 */

/* Includes -----------------------------------------------------------------------------*/
#include <Arduino.h>
#include "task.h"
#include "app.h"
#include "Esp.h"

/* Private typedef ----------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------------------*/
extern APP *app;

/* Private variables --------------------------------------------------------------------*/
/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   系统状态任务，1S
 */
void task_1HZ_entry(void)
{
    app->system_status_monitor();
}

/**
 * @brief   传感器任务，200ms
 */
void task_5HZ_entry(void)
{
    app->sensor_monitor();
}

/**
 * @brief   GUI刷新任务，5ms
 */
void task_gui_entry(void)
{
    app->display.process();
}

TASK_DECLARE(task_1Hz, task_1HZ_entry, 1000);       // 1000ms
TASK_DECLARE(task_5Hz, task_5HZ_entry, 200);        // 200ms
TASK_DECLARE(task_gui, task_gui_entry, 5);          // 5ms
TASK_DECLARE(task_widget, NULL, 100);               // 100ms

void setup()
{
    EspClass esp;
    Serial.begin(115200);

    Serial.println("system init...");
    Serial.println("CPU:" + String(esp.getCpuFreqMHz()));
    Serial.println("FLASH Size:" + String(esp.getFlashChipSize()));
    Serial.println("FLASH Speed:" + String(esp.getFlashChipSpeed()));

    // 创建任务
    task_creat(0, &task_1Hz);
    task_creat(1, &task_5Hz);
    task_creat(2, &task_gui);
    task_creat(3, &task_widget);

    Serial.println("app init...");
    app->init();

    Serial.println("app start");
}

void loop()
{
    task_scheduler_start();
}
