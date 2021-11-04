/**
 * @file    setting.cpp
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-25
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "setting.h"
#include <WebServer.h>
#include "app.h"
#include "ui/ui.h"
#include "html.h"
#include "drivers/config.h"

#define SETTING_LOG_ENABLE  1

#if SETTING_LOG_ENABLE
#define SETTING_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define SETTING_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define SETTING_LOG(format, ...)
#define SETTING_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
#define POS_LOGO_X          (56)
#define POS_LOGO_Y          (10)

#define POS_TEXT_OFFSET_Y   (25)

#define POS_TXT1_X          (32)
#define POS_TXT1_Y          (160)
#define POS_TXT2_X          (POS_TXT1_X)
#define POS_TXT2_Y          (POS_TXT1_Y + POS_TEXT_OFFSET_Y)
#define POS_TXT3_X          (POS_TXT1_X)
#define POS_TXT3_Y          (POS_TXT2_Y + POS_TEXT_OFFSET_Y)

/* Private variables --------------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------------------*/
extern APP* app;
extern String webpage;
extern String webpage_header;
extern String webpage_footer;

extern lv_style_t style_default;
extern lv_style_t style_oppo_sans_14;

/* Private functions --------------------------------------------------------------------*/
WebServer server(80);

static widget_t widget_setting = {0};

lv_obj_t *screen_setting = NULL;
static lv_obj_t *img_logo;
static lv_obj_t *label_txt1;
static lv_obj_t *label_txt2;
static lv_obj_t *label_txt3;

/* Exported functions -------------------------------------------------------------------*/

void Send_HTML(const String &content)
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.send(200, "text/html", "");

    server.sendContent(webpage_header);
    server.sendContent(content);
    server.sendContent(webpage_footer);

    server.sendContent("");
    server.client().stop(); // Stop is needed because no content length was sent
}

void save_wifi_config(void)
{

    Send_HTML("<h1>保存成功，重启后生效</h1>");
    //获取输入的WIFI账户和密码
    app->config.wifi_ssid = server.arg("ssid");
    app->config.wifi_key = server.arg("key");
    config_save("Setting", &app->config);

    Serial.println(app->config.wifi_ssid);
    Serial.println(app->config.wifi_key);
}

void get_homepage()
{
    Send_HTML(get_wifi_page());
}

void page_reboot()
{
    if (server.args() > 0)
    {

        Serial.println("rebot:" + server.arg(0));
        Send_HTML("<h1>小魔方即将重启...</h1>");
        ESP.restart();
    }
    else
    {
        Send_HTML(get_reboot_page());
    }
}


void start_web_config()
{
    init_page_header();
    init_page_footer();

    //首页
    server.on("/", get_homepage);
    server.on("/saveWifi", save_wifi_config);
    server.on("/reboot", page_reboot);

    // server.on("/upload", File_Upload);
    // server.on("/delete", File_Delete);
    // server.on("/delete_result", delete_result);
    // server.on("/setting", Setting);
    // server.on(
    //     "/fupload", HTTP_POST,
    //     []()
    //     { server.send(200); },
    //     handleFileUpload);

    server.begin();
    // MDNS.addService("http", "tcp", 80);
    Serial.println("HTTP server started");
}

/**
 * @brief   控制函数
 * @param   cmd             text
 * @param   args            text
 */
gt_err_t widget_setting_control(uint32_t cmd, void *args)
{
    switch (cmd)
    {
    case KEY_BACK:
        app->widget.launcher_control(LAUNCHER_EXIT, NULL);
        break;
    default:
        break;
    }

    return GT_EOK;
}

/**
 * @brief   初始化
 */
void widget_setting_init(void)
{
    // 初始化页面
    screen_setting = lv_obj_create(NULL);
    lv_obj_add_style(screen_setting, &style_default, 0);

    // 图标
    img_logo = lv_img_create(screen_setting);
    lv_obj_set_pos(img_logo, POS_LOGO_X, POS_LOGO_Y);
    lv_obj_align(img_logo, LV_ALIGN_TOP_MID, 0, 30);
    lv_img_set_src(img_logo, &image_bilibili);

    label_txt1 = lv_label_create(screen_setting);
    lv_obj_add_style(label_txt1, &style_oppo_sans_14, 0);
    lv_obj_set_pos(label_txt1, POS_TXT1_X, POS_TXT1_Y);
    // lv_obj_align(label_txt2, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text(label_txt1, "管理页面");

    label_txt2 = lv_label_create(screen_setting);
    lv_obj_add_style(label_txt2, &style_default, 0);
    lv_obj_set_pos(label_txt2, POS_TXT2_X, POS_TXT2_Y);
    // lv_obj_align(label_txt2, LV_ALIGN_TOP_MID, 0, 0);
    // lv_label_set_text(label_txt2, " ");

    label_txt3 = lv_label_create(screen_setting);
    lv_obj_add_style(label_txt3, &style_default, 0);
    lv_obj_set_pos(label_txt3, POS_TXT3_X, POS_TXT3_Y);
    // lv_obj_align(label_status, LV_ALIGN_CENTER, 0, 0);
    // lv_label_set_text(label_txt3, " ");

    if (app->wifi.is_connected())
    {
        // lv_label_set_text(label_txt2, String("wifi:" + app->wifi.get_ap_ssid()).c_str());
        lv_label_set_text(label_txt2, String("web : " + app->wifi.get_local_ip()).c_str());
        lv_label_set_text(label_txt3, " ");
    }
    else
    {
        app->wifi.open_ap();
        lv_label_set_text(label_txt2, String("wifi : " + app->wifi.get_ap_ssid()).c_str());
        lv_label_set_text(label_txt3, String("web : " + app->wifi.get_ap_ip()).c_str());
    }

    lv_scr_load(screen_setting);
}

/**
 * @brief   运行函数
 */
void widget_setting_process(void)
{
}

/**
 * @brief   退出
 */
void widget_setting_exit(void)
{
    lv_obj_del(screen_setting);
}

/**
 * @brief   后台服务
 */
void setting_process(void)
{
    server.handleClient();
}

/**
 * @brief   Set the ting init object
 */
void setting_init(void)
{
    start_web_config();

    // 注册 widget
    widget_setting.id = WIDGET_SETTING;
    widget_setting.name = "SETTING";
    widget_setting.icon = &icon_setting;
    widget_setting.init = widget_setting_init;
    widget_setting.process = widget_setting_process;
    widget_setting.exit = widget_setting_exit;
    widget_setting.control = widget_setting_control;
    widget_setting.peroid = 100;
    widget_setting.args = 0;
    app->widget.add(&widget_setting);

    Serial.println("setting init");
}
