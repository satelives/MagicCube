/**
 * @file    genius.cpp
 * @brief   天猫精灵后台服务
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-11-02
 *
 * (C) COPYRIGHT 2021 chery.zhang
 *
 */

/* Includes -----------------------------------------------------------------------------*/
#include "genius.h"
#include "app.h"

#define GENIUS_LOG_ENABLE 1

#if GENIUS_LOG_ENABLE
#define GENIUS_LOG(format, ...)         Serial.print(format, ##__VA_ARGS__)
#define GENIUS_LOG_NEWLINE(format, ...) Serial.println(format, ##__VA_ARGS__)
#else
#define GENIUS_LOG(format, ...)
#define GENIUS_LOG_NEWLINE(format, ...)
#endif

/* Private typedef ----------------------------------------------------------------------*/
#define CONNECT_INTERVAL        (30)        // 重连间隔（秒）
#define PING_INTERVAL           (60)        // ping间隔（秒）

/* Private variables --------------------------------------------------------------------*/
static const char* hostname = "bemfa.com";
static const uint16_t hostport = 8344;
static String private_key = "2a175cb10754f9db17d6fea459f2d370";
static String topic = "cube002";

static uint32_t ticks = 0;

/* Extern variables ---------------------------------------------------------------------*/
extern APP *app;

/* Private functions --------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/

/**
 * @brief   在消息字符串中提取指定的数值
 * @param   msg             消息
 * @param   key             关键字
 * @param   break_key       停止关键字
 * @param   result          提取结果
 * @return  true
 * @return  false
 */
static bool get_value(String msg, String key, char break_charset, String *result)
{
    if (msg.length() < key.length())
    {
        return false;
    }

    int key_index = msg.indexOf(key);
    // GENIUS_LOG_NEWLINE("msg:" + String(msg.length()) + " key:" + String(key.length()) + " idx:" + String(key_index));
    if (key_index > 0)
    {
        int value_len = 0;
        for (int i = key_index + key.length(); i < msg.length(); i++)
        {
            if (msg[i] == break_charset)
            {
                break;
            }
            value_len++;
        }

        *result = msg.substring(key_index + key.length(), key_index + key.length() + value_len);
        // GENIUS_LOG_NEWLINE("value:" + *result + " len:" + String(value_len));

        return true;
    }

    return false;
}


/**
 * @brief   是否已经连接
 */
bool GENIUS::isConnected(void)
{
    return TCPclient.connected() && Connected;
}

/**
 * @brief   发送消息
 */
size_t GENIUS::send_msg(String msg)
{
    if (TCPclient.connected())
    {
        GENIUS_LOG_NEWLINE("send msg:" + msg);
        return TCPclient.print(msg);
    }

    GENIUS_LOG_NEWLINE("send error, tcp not connected");
    return 0;
}

/**
 * @brief   连接
 */
bool GENIUS::connect(void)
{
    GENIUS_LOG_NEWLINE("connect to tm genius");
    Connected = false;
    if (TCPclient.connect(hostname, hostport))
    {
        String request = "cmd=1&uid=" + private_key + "&topic=" + topic + "\r\n";
        if (send_msg(request) > 0)
        {
            Connected = true;
            GENIUS_LOG_NEWLINE("connect ok");
        }
    }

    return Connected;
}

/**
 * @brief   处理
 */
void GENIUS::process(void)
{
    if (isConnected())
    {
        if (TCPclient.available())
        {
            Connected = true;   // 收到任何数据即为在线

            // 读取所有缓存
            while (TCPclient.available())
            {
                recieved_msg += (char)TCPclient.read();
            }
            TCPclient.flush();

            // 提取消息
            if (recieved_msg.endsWith("\n"))
            {
                recieved_msg.trim();
                GENIUS_LOG_NEWLINE("rec:" + recieved_msg + " len:" + String(recieved_msg.length()));
                String msg = "";
                if (get_value(recieved_msg, "msg=", '&', &msg))
                {
                    if (msg.indexOf("on") >= 0)
                    {
                        app->widget.launch(WIDGET_INSPIRE);
                        app->widget.control(0xFF, 0);
                    }

                    GENIUS_LOG_NEWLINE("msg:" + msg);
                }
                recieved_msg = "";
            }
        }
        else
        {
            if (ticks == 0) // 心跳
            {
                ticks = PING_INTERVAL;
                send_msg("ping\r\n");
            }
            else
            {
                ticks--;
            }
        }
    }
    else
    {
        // 重连
        if (ticks == 0)
        {
            ticks = CONNECT_INTERVAL;
            connect();
        }
        else
        {
            ticks--;
        }
    }
}
