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

#include "network.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

/* Private typedef ----------------------------------------------------------------------*/
#define NETWORK_LOG_ENABLE  1

#if NETWORK_LOG_ENABLE
#define NETWORK_LOG(format, ...)            Serial.print(format, ##__VA_ARGS__)
#define NETWORK_LOG_NEWLINE(format, ...)    Serial.println(format, ##__VA_ARGS__)
#else
#define NETWORK_LOG(format, ...)
#define NETWORK_LOG_NEWLINE(format, ...)
#endif

/* Extern variables ---------------------------------------------------------------------*/
/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/
// http://restapi.amap.com/v3/ip?key=e673da4f70707f787c7b00443211602b&ip=61.144.172.254
// {"status":"1","info":"OK","infocode":"10000","province":"广东省","city":"深圳市","adcode":"440300","rectangle":"113.9629412,22.4627142;114.2106056,22.61394155"}
/**
 * @brief   wifi 是否连接
 * @return  true
 * @return  false
 */
bool Network::is_connected()
{
    // bool status = WiFi.isConnected();

    return WiFi.isConnected();
}

/**
 * @brief   获取当前SSID
 * @return  String
 */
String Network::get_ssid()
{
    if (is_connected())
    {
        return WiFi.SSID();
    }
    else
    {
        return _ssid;
    }
}

/**
 * @brief   获取当前密码
 * @return  String
 */
String Network::get_password()
{
    return _password;
}

/**
 * @brief   wifi 连接
 * @param   ssid            名称
 * @param   password        密码
 */
void Network::connect(String ssid, String password)
{
    last_connect_status = false;
    is_enable = true;
    _ssid = ssid;
    _password = password;

    NETWORK_LOG_NEWLINE("connecting to " + ssid + " password: " + password);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(_ssid.c_str(), _password.c_str());

    int timeout = 0;
    while (!WiFi.isConnected() && (timeout <= 50)) // 5S
    {
        delay(100);
        NETWORK_LOG(".");
        timeout++;
    }

    if (!is_connected())
    {
        NETWORK_LOG_NEWLINE("\nWiFi connect fail");
    }
    else
    {
        NETWORK_LOG_NEWLINE("\nWiFi connected");
    }
}

/**
 * @brief   扫描wifi
 * @return  String
 */
int Network::search(String *result)
{
    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        *result = "wifi no found";
    }
    else
    {
        for (int i = 0; i < n; ++i)
        {
            // *result += String(i + 1);
            // *result += String(",");
            *result += WiFi.SSID(i);
            *result += " (" + String(WiFi.RSSI(i)) + "dbm)";
            *result += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*";
            *result += "\n";
        }
    }

    NETWORK_LOG_NEWLINE(*result);
    return n;
}

/**
 * @brief   获取当前IP
 * @return  String
 */
String Network::get_local_ip()
{
    return WiFi.localIP().toString();
}

/**
 * @brief   获取AP模式下的IP
 * @return  String
 */
String Network::get_ap_ip()
{
    return WiFi.softAPIP().toString();
}

/**
 * @brief   获取AP模式下的IP
 * @return  String
 */
String Network::get_ap_ssid()
{
    return ap_ssid;
}

/**
 * @brief   Construct a new Network:: Network object
 */
Network::Network()
{
    _ssid        = "Seven";
    _password    = "zhang28jian28";
    hostname     =  "MagicCube";
    ap_local_ip  =  *(new IPAddress(192, 168,   10, 2)); // Set your server's fixed IP address here
    ap_gateway   =  *(new IPAddress(192, 168,   10, 1)); // Set your network Gateway usually your Router base address
    ap_subnet    =  *(new IPAddress(255, 255, 255, 0)); // Set your network sub-network mask here
    ap_dns       =  *(new IPAddress(192, 168,   10, 1)); // Set your network DNS usually your Router base address
}

/**
 * @brief   开启AP模式
 * @param   ssid            名称
 * @param   password        密码
 * @param   channel         信道
 * @param   ishidden        是否隐藏
 * @param   max_connection  最大连接数
 */
bool Network::open_ap(String ssid, String password, int channel, int ishidden, int max_connection)
{
    bool result = true;

    ap_ssid = ssid;
    ap_password = password;

    result &= WiFi.mode(WIFI_AP);
    result &= WiFi.setHostname(hostname.c_str());
    result &= WiFi.softAPConfig(ap_local_ip, ap_gateway, ap_subnet);
    result &= WiFi.softAP(ssid.c_str(), password.c_str(), channel, ishidden, max_connection);

    NETWORK_LOG_NEWLINE("wifi ap start");

    // 设置域名
    if (MDNS.begin(hostname.c_str()))
    {
        NETWORK_LOG_NEWLINE("MDNS responder started");
    }
    // MDNS.addService("http", "tcp", 80);

    return result;
}

wifi_mode_t Network::getMode(void)
{
    return WiFi.getMode();
}

void Network::monitor(void)
{
    if (WiFi.getMode() == WIFI_MODE_STA)
    {
        if (WiFi.isConnected())
        {
            if (last_connect_status == false)
            {
                last_connect_status = true;

                NETWORK_LOG("WiFi connected,ip:");
                NETWORK_LOG_NEWLINE(WiFi.localIP());
                // WiFi.setSleep()
            }
        }
        else
        {
            last_connect_status = false;
            if (retry_cnt >= CONNECT_RETRY_INTERVAL)
            {
                WiFi.disconnect();
                retry_cnt = 0;
                NETWORK_LOG_NEWLINE("WiFi retry :" + _ssid);
                WiFi.begin(_ssid.c_str(), _password.c_str());
            }
            else
            {
                retry_cnt++;
            }
        }
    }
    // else if (WiFi.getMode() == WIFI_MODE_AP)
    // {

    // }
}

/**
 * @brief   http get
 * @param   url             text
 * @param   result          text
 * @return  int
 */
int Network::http_get(String url, String *result)
{
    if (WL_CONNECTED != WiFi.status())
        return 0;

    int httpCode;
    HTTPClient http;
    std::unique_ptr<WiFiClient> client(new WiFiClient);

    http.setTimeout(3000);
    http.begin(*client, url);
    httpCode = http.GET();
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            *result = http.getString();

            // NETWORK_LOG_NEWLINE(*result);
        }
    }
    else
    {

    }
    http.end();

    return httpCode;
}

/**
 * @brief   获取网络时间
 * @param   url             text
 * @return  long long
 */
long Network::getTimestamp()
{
    String url = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";
    String payload = "";
    String time = "";
    long long Timestamp = 0;

    int httpCode = http_get(url, &payload);

    // NETWORK_LOG_NEWLINE("getTimestamp " + String(httpCode));
    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            // {"api":"mtop.common.getTimestamp","v":"*","ret":["SUCCESS::接口调用成功"],"data":{"t":"1635232771871"}}
            int time_index = (payload.indexOf("data")) + 12;
            time = payload.substring(time_index, payload.length() - 3);
            // 以网络时间戳为准
            Timestamp = atoll(time.c_str());
            Timestamp = Timestamp / 1000;
#if NETWORK_LOG_ENABLE
            char s[50];
            snprintf(s, 50, "Timestamp=%lld", Timestamp);
            NETWORK_LOG_NEWLINE(s);
#endif

        }
    }
    else
    {

    }

    return (long)Timestamp;
}
