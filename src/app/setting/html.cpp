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
#include "app.h"

/* Private typedef ----------------------------------------------------------------------*/
/* Private variables --------------------------------------------------------------------*/
#define HTML_TITLE  "大熊给啾啾做的 MagicCube"

#define SETING_CSS ".input {display: block;margin-top: 10px;}"                                          \
                   ".input span {width: 200px;float: left;float: left;height: 36px;line-height: 36px;}" \
                   ".input input {height: 30px;width: 200px;border: 1px solid #ccc;}"                   \
                   ".btn {margin-top: 15px;text-align: center;border-radius:10px; background-color: #4CAF50; color: #ffffff;border: none;padding: 10px 20px;cursor: pointer;}" // margin-left: 100px;

#define WIFI_SETTING_CONTENT                                                                                                       \
    "<form method=\"GET\" action=\"saveWifi\">"                                                                            \
    "<label class=\"input\"><span>WiFi名称</span><input type=\"text\"name=\"ssid\"value=\"%s\"></label>"                 \
    "<label class=\"input\"><span>WiFi密码</span><input type=\"text\"name=\"key\"value=\"%s\"></label>"                 \
    "</label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"保存\">"                                    \
    "</form>"

/* Extern variables ---------------------------------------------------------------------*/
extern APP* app;

/* Private functions --------------------------------------------------------------------*/
String webpage = "";
String webpage_header = "";
String webpage_footer = "";

/* Exported functions -------------------------------------------------------------------*/
void init_page_header()
{
    webpage_header =  F("<!DOCTYPE html><html>");
    webpage_header += F("<head>");
    webpage_header += F("<title> 大熊给啾啾做的 MagicCube </title>"); // NOTE: 1em = 16px
    webpage_header += F("<meta http-equiv='Content-Type' name='viewport' content='user-scalable=yes,initial-scale=1.0,width=device-width; text/html; charset=utf-8' />");
    webpage_header += F("<style>");
    webpage_header += F(SETING_CSS);
    webpage_header += F("body{max-width:65%;margin:0 auto;font-family:yahei;font-size:105%;text-align:center;color:#33a3dc;background-color:#FFFFFF;}");
    webpage_header += F("ul{list-style-type:none;margin:0.1em;padding:0;border-radius:5px;overflow:hidden;background-color:#F1FDFF;font-size:1em;}");
    webpage_header += F("li{float:left;border-radius:0.375em;border-right:0.06em solid #00BCD4;}last-child {border-right:none;font-size:85%}");
    webpage_header += F("li a{display: block;border-radius:0.375em;padding:0.44em 0.44em;text-decoration:none;font-size:85%}");
    webpage_header += F("li a:hover{background-color:#2DB84D;font-size:85%}");
    webpage_header += F("section {font-size:0.88em;}");
    webpage_header += F("h1{color:#2DB84D;font-size:1em;padding:0.2em 0.2em;}");
    webpage_header += F("h2{color:orange;font-size:1.0em;}");
    webpage_header += F("h3{font-size:0.8em;}");
    webpage_header += F("table{font-family:arial,sans-serif;font-size:0.9em;border-collapse:collapse;width:85%;}");
    webpage_header += F("th,td {border:0.06em solid #dddddd;text-align:left;padding:0.3em;border-bottom:0.06em solid #dddddd;}");
    webpage_header += F("tr:nth-child(odd) {background-color:#eeeeee;}");
    // webpage_header += F(".rcorners_n {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:20%;color:white;font-size:75%;}");
    // webpage_header += F(".rcorners_m {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:50%;color:white;font-size:75%;}");
    // webpage_header += F(".rcorners_w {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:70%;color:white;font-size:75%;}");
    webpage_header += F(".column{float:left;width:50%;height:45%;}");
    webpage_header += F(".row:after{content:'';display:table;clear:both;}");
    webpage_header += F("*{box-sizing:border-box;}");
    webpage_header += F("footer{text-align:center;padding:0.3em 0.3em;font-size:60%;}");
    // webpage_header += F("button{border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:20%;color:white;font-size:130%;}");
    // webpage_header += F(".buttons {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:15%;color:white;font-size:80%;}");
    // webpage_header += F(".buttonsm{border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:9%; color:white;font-size:70%;}");
    // webpage_header += F(".buttonm {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:15%;color:white;font-size:70%;}");
    // webpage_header += F(".buttonw {border-radius:0.5em;background:#558ED5;padding:0.3em 0.3em;width:40%;color:white;font-size:70%;}");
    webpage_header += F("a{font-size:75%;}");
    webpage_header += F("p{font-size:75%;}");

    webpage_header += F("</style></head><body>");

    webpage_header += F("<h1>大熊给啾啾做的 MagicCube ");
    webpage_header += String(APP_VERSION) + "</h1>";
    webpage_header += F("<ul>");
    webpage_header += F("<li><a href='/'>网络设置</a></li>"); // Lower Menu bar command entries
    webpage_header += F("<li><a href='/reboot'>重启小魔方</a></li>");
    webpage_header += F("<li><a href='/'>待开发</a></li>");
    // webpage_header += F("<li><a href='/upload'>Upload</a></li>");
    // webpage_header += F("<li><a href='/delete'>Delete</a></li>");
    webpage_header += F("</ul>");
}

void init_page_footer()
{
    webpage_footer = "<footer>&copy; Seven 2021</footer>";
    webpage_footer += F("</body></html>");
}

String get_wifi_page()
{
    // 指定 target='_blank' 设置新建页面
    char buf[1024];

    sprintf(buf, WIFI_SETTING_CONTENT, app->wifi.get_ssid().c_str(), app->wifi.get_password().c_str());

    return String(buf);
}

String get_reboot_page()
{
    String content =

        "<h3>重启小魔方</h3>"
        "<form action='/reboot' method='post'>"
        "<br><input class=\"btn\" type=\"submit\" name=\"reboot\" value=\"重启\"><br></form>";

    return String(content);
}

