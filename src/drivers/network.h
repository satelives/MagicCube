/**
 * @file    network.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-20
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/

#ifndef __NETWORK_H
#define __NETWORK_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include <WiFi.h>

/* Exported constants -------------------------------------------------------------------*/
#define CONNECT_RETRY_INTERVAL  (20)    // S

/* Exported types -----------------------------------------------------------------------*/
class Network
{
private:
    bool last_connect_status;
    uint16_t retry_cnt = 0;
    String _ssid;
    String _password;
    String ap_ssid;
    String ap_password;

public:
    Network();

    bool is_enable = false;

    wifi_mode_t getMode(void);

    // station 模式
    bool is_connected();
    String get_ssid();
    String get_password();
    String get_local_ip();
    void connect(String ssid, String password);
    int search(String *result);
    void monitor(void);

    // ap 模式
    String hostname;
    IPAddress ap_local_ip;
    IPAddress ap_gateway;
    IPAddress ap_subnet;
    IPAddress ap_dns;

    String get_ap_ssid();
    String get_ap_ip();
    bool open_ap(String ssid = "MagicCube", String password = "", int channel = 1, int ishidden = 0, int max_connection = 1);

    // 其他接口
    int http_get(String url, String *result);
    long getTimestamp();

};

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

#endif
