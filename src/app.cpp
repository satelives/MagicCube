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
#include "app.h"
#include "app/setting/setting.h"
#include "app/startup/startup.h"
#include "app/clock/clock.h"
#include "app/weather/weather.h"
#include "app/photo/photo.h"
#include "app/inspire/inspire.h"

#define APP_LOG_ENABLE  1

#if APP_LOG_ENABLE
#define APP_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define APP_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define APP_LOG(format, ...)
#define APP_LOG_NEWLINE(format, ...)
#endif

/* Exported constants -------------------------------------------------------------------*/
/*  GPIO Map
    22-I2C_SCL
    21-I2C_SDA
    17
    16
    TDO
    SD0
    27
    25
    32
    TDI
    4
    0
    2
    SD1
    CLK
    VP
    26-TFT_DC
    18-TFT_SCLK
    19-TFT_MISO
    23-TFT_MOSI
    5-TFT_CS
    TCK
    SD3
    VN
    33-TFT_RST
    34-EXT-POWER_DETECT
*/

#define EXTERN_POWER_DETECT_PIN     (34)
#define IMU_REVERSE_ACELL           (10000) // 翻转案件
#define IMU_INPUT_ACELL             (7000)  // 按键重力加速度阈值
#define IMU_INPUT_GYRO              (2000)  // 按键加速度阈值

/* Private variables --------------------------------------------------------------------*/
static widget_t default_widget;

/* Extern variables ---------------------------------------------------------------------*/
APP *app = new APP();

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   调试打印时间
 */
void printLocalTime()
{
    char s[51];
    strftime(s, 50, "%F %H:%M:%S", &app->rtc.time);
    Serial.println(s);
}

/**
 * @brief   系统状态更新 1000ms
 */
void APP::system_status_monitor(void)
{
    // printLocalTime();
    wifi.monitor();
    rtc.update();

    if (wifi.is_connected())
    {
        weather.update();   // 天气数据更新
        genius.process();   // 天猫精灵数据处理
    }

    // http服务： 配置
    setting_process();
}

/**
 * @brief   传感器数据处理 200ms
 */
void APP::sensor_monitor(void)
{
    static uint16_t auto_run = 0;

    is_extern_power = (bool)digitalRead(EXTERN_POWER_DETECT_PIN);

    // imu数据更新及体感控制
    if (app->imu.update())
    {
        IMU_DATA accel = app->imu.getAccel();
        IMU_DATA gyro = app->imu.getGyro();

        // 体感触发判断
        if (app->key.event_tmp == KEY_NONE)
        {
            app->key.holding_cnt = 0;
            // 倒立
            if (accel.z > IMU_REVERSE_ACELL)
            {
                app->key.event_tmp = KEY_REVERSE;
            }
            // 向左倾斜
            else if ((accel.x > IMU_INPUT_ACELL) && (gyro.y > IMU_INPUT_GYRO))
            {
                app->key.event_tmp = KEY_LEFT;
            }
            // 向右倾斜
            else if ((accel.x < -IMU_INPUT_ACELL) && (gyro.y < -IMU_INPUT_GYRO))
            {
                app->key.event_tmp = KEY_RIGHT;
            }
            // 向前倾斜
            else if ((accel.y < -IMU_INPUT_ACELL) && (gyro.x > -IMU_INPUT_GYRO))
            {
                app->key.event_tmp = KEY_FRONT;
            }
            // 向后倾斜
            else if ((accel.y > IMU_INPUT_ACELL) && (gyro.x < IMU_INPUT_GYRO))
            {
                app->key.event_tmp = KEY_BACK;
            }

        }
        // 持续时间判断
        else
        {
            auto_run = 0;
            bool holding = false;
            // 倒立
            if (accel.z > IMU_REVERSE_ACELL)
            {
                holding = true;
                if (app->key.holding_cnt++ >= KEY_REVERSE_CNT)
                {
                    APP_LOG_NEWLINE("KEY REV");
                    app->key.holding_cnt = 0;
                    app->key.event = app->key.event_tmp;
                    app->key.event_tmp = KEY_NONE;
                    // 进入wifi重置状态
                    reset_wifi = true;
                    widget.launch(WIDGET_SETTING);
                }
            }
            // 向左倾斜
            else if((app->key.event_tmp = KEY_LEFT) && (accel.x > IMU_INPUT_ACELL))
            {
                holding = true;
                if (app->key.holding_cnt++ >= KEY_L_R_CNT)
                {
                    APP_LOG_NEWLINE("KEY R");
                    app->key.event = app->key.event_tmp;
                    app->key.event_tmp = KEY_NONE;
                    if (!widget.is_widget_running())
                    {
                        widget.launcher_control(LAUNCHER_PREV, GT_NULL);
                    }
                    else
                    {
                        widget.control(KEY_LEFT, GT_NULL);
                    }
                }
            }
            // 向右倾斜
            else if ((app->key.event_tmp = KEY_RIGHT) && (accel.x < -IMU_INPUT_ACELL))
            {
                holding = true;
                if (app->key.holding_cnt++ >= KEY_L_R_CNT)
                {
                    APP_LOG_NEWLINE("KEY L");
                    app->key.event = app->key.event_tmp;
                    app->key.event_tmp = KEY_NONE;
                    if (!widget.is_widget_running())
                    {
                        widget.launcher_control(LAUNCHER_NEXT, GT_NULL);
                    }
                    else
                    {
                        widget.control(KEY_RIGHT, GT_NULL);
                    }
                }
            }
            // 向前倾斜
            else if (accel.y < -IMU_INPUT_ACELL)
            {
                holding = true;
                if (app->key.holding_cnt++ >= KEY_F_B_CNT)
                {
                    APP_LOG_NEWLINE("KEY F");
                    app->key.event = app->key.event_tmp;
                    app->key.event_tmp = KEY_NONE;
                    if (widget.is_widget_running())
                    {
                        widget.control(KEY_FRONT, GT_NULL);
                    }
                    else
                    {
                        widget.launcher_control(LAUNCHER_ENTER, GT_NULL);
                    }
                }
            }
            // 向后倾斜
            else if (accel.y > IMU_INPUT_ACELL)
            {
                holding = true;
                if (app->key.holding_cnt++ >= KEY_F_B_CNT)
                {
                    APP_LOG_NEWLINE("KEY B");
                    app->key.event = app->key.event_tmp;
                    app->key.event_tmp = KEY_NONE;
                    if (widget.is_widget_running())
                    {
                        widget.control(KEY_BACK, GT_NULL);
                    }
                }
            }

            if (!holding)
            {
                app->key.event_tmp = KEY_NONE;
            }
        }
    }

    // 若10S内没有任何操作且无组件运行，默认切换至时钟界面
    if (!widget.is_widget_running())
    {
        auto_run++;
        if (auto_run > 50) // 10S
        {
            auto_run = 0;
            widget.launch(WIDGET_CLOCK);
        }
    }
}

/**
 * @brief   app初始化
 */
void APP::init(void)
{
    // 配置IO
    pinMode(EXTERN_POWER_DETECT_PIN, INPUT_PULLDOWN);

    // 读取配置
    config_read("Setting", &config);

    // 初始化显示
    display.init();

    // 显示启动页面
    show_startup(true);

    // 初始化其他外设
    rtc.init(true);
    imu.init();

    // 初始化应用
    clock_init();
    weather_init();
    photo_init();
    inspire_init();
    setting_init();

    // 根据配置信息连接wifi
    if (config.is_vaild == 0xAA55AA55)
    {
        wifi.connect(config.wifi_ssid, config.wifi_key);
    }
    else
    {
        wifi.connect(config.default_ssid, config.default_key);
    }

    // 隐藏启动界面
    show_startup(false);

    // 启动主界面
    widget.launcher_init();
    widget.launcher_control(LAUNCHER_INIT, GT_NULL);
}

APP::APP()
{
    // widget.add(&default_widget);
}

APP::~APP()
{

}
